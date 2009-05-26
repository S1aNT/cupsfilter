#!/usr/bin/perl  -w
use strict;
use locale;
use File::Copy "cp";
use File::Path;
use Time::localtime;
use IO::File;
use File::Temp ();
use SQLShell;
use DBI;
#use encoding 'utf8';

#use File::Temp qw/ :seekable /;
#require File::Temp;

#----------------------- CONFIG ------------------------------------
my $DEBUG=1;							# For debuging and troubleshooting. Nice hunt !

my @argv = @ARGV;			#keep hands off the argument array
my $SPOOL = "/tmp";			#where are my speedy memory drive?
my $DEBUG_FILE = "/var/log/cups/tech_filter_debug.log";
my $FPAGEDIR   ="/var/log/cups/firstpages"; #каталог в котором храняться первые страницы документа
my $DATE_BIN = "/bin/date";				#path to date
my $PDF_BIN="/usr/bin/ps2pdf";			#path to ps2pdf binary
my $PSSELECT_BIN ="/usr/bin/psselect"; 	#path to psselect binary
my $ENABLE ="/usr/bin/cupsenable"; 			#path to enable binary
my $RACF_UTIL_BIN="/usr/bin/racf.pl";	#path to racf script
my $FH_TEMP;							#File handle to temp file

my %TAGS_DATA=();						#Hash contain's parsed data from print job
my @result;								#Array of function result (Code,InfoStr)
#--------------------------------------------------------------------------------------------------
#Check and parse arguments
if ($DEBUG){
	$RACF_UTIL_BIN="racf.pl";
	$PSSELECT_BIN ="psselect.pl";
	$SPOOL ="d:\\Temp\\";
	@argv=(42,"Sla\@nt","«Сборник практических заданий по ТСП для слушателей \\ учебной группы!»",5,"JUID:42","sample.ps");
}

my ($jobID,$userName,$jobTitle,$copies,$printOptions,$printFile) = @argv;

$TAGS_DATA{"user"}=lc(cleaner($userName));
$TAGS_DATA{"jobID"}=$jobID;
$TAGS_DATA{"class"}=$ENV{"CLASS"};
$TAGS_DATA{"copies"}=$copies;
$TAGS_DATA{"printer"}=$ENV{"PRINTER"};
$TAGS_DATA{"jobTitle"}= lc(cleaner($jobTitle));
$TAGS_DATA{"jobTitle"}=~ s/smbprn[0-9]*//;
$TAGS_DATA{"jobTitle"}=~ s/microsoft|word|explorer//gi;
$TAGS_DATA{"printFile"}=$printFile;
$TAGS_DATA{"device_uri"}=$ENV{"DEVICE_URI"};
$TAGS_DATA{"printOptions"}=$printOptions;

#--------------------------------------------------------------------------------------------------
# If no arguments, device discovery mode
if (!$argv[0]){
        print ("direct tech_filter \"Unknown\" \"TechnoServ Filter \"\n");
        exit 1;
}
# If too many arguments, send error
if (scalar(@argv) < 5 || scalar(@argv) > 6){
        print STDERR ("Usage: tech_filter job-id user title copies options [file]\n");
        exit 1;
}
#--------------------------------------------------------------------------------------------------
#The hard, but fine work :)
my $dbh = new SQLShell; # Connect to database
#Заполним массив данными требуемыми для соединения с БД
$dbh->options(	"CupsLog",	# имя базы данных
				"postgres",	# имя пользователя
				"pg",		# пароль
				"localhost",# имя или IP адрес сервера
				"5432",		# порт
				"-e",		# опции
				"ansi");	# терминал
$dbh->connect();		
if ($DEBUG){
	$dbh->debug(3,"dbh.log");
	#save_debug_msg ("Test messages can't contain quotes !!\n");
}		

#analising device _uri :)
$TAGS_DATA{"device_uri"} =~ m/.*?:\/{1,2}(.*?)\/.*/gis;
if (defined $1){
	$TAGS_DATA{"printer_ip"} = $1;
	$TAGS_DATA{"printer_ip"} =~ s/:.*//;  # Pull off port number if present
}else{
	save_debug_msg ("Device URI = ".$TAGS_DATA{"device_uri"}." can't contain IP\n");
	cancel_print_job($TAGS_DATA{"jobID"},$TAGS_DATA{"printer"});
	exit 1;
}

@result =get_printers_mandat($TAGS_DATA{"printer_ip"},$TAGS_DATA{"printer"});
if ($result[0]){
	$TAGS_DATA{"MANDAT"}=$result[1];
}else{
	save_debug_msg ($result[1]);# TODO must be -> "Printers with IP = ".$TAGS_DATA{"printer_ip"}.",name =".$TAGS_DATA{"printer"}." has not Mandat !\n";
	cancel_print_job($TAGS_DATA{"jobID"},$TAGS_DATA{"printer"});
	exit 1;
}
# Safe create temp file
$FH_TEMP = File::Temp->new( TEMPLATE => 'cups_jobXXXXX',
                                DIR => $SPOOL,
                                SUFFIX => '.~tmp');

$TAGS_DATA{"TMP_FILENAME"} = $FH_TEMP->filename();
                            
@result =parse_file($TAGS_DATA{"printFile"},$FH_TEMP);
if ($result[0]){#check permission to print
    my @tmp=($TAGS_DATA{"printer_ip"},$TAGS_DATA{"MANDAT"},$TAGS_DATA{"protect"});
	my $perm = $dbh->just_do($TAGS_DATA{"printer"},\@tmp);
	if ($perm->[0][0]){
		send2stdout();
	}else{
		save_debug_msg ("Can't determine the rights assigned to printer ".$TAGS_DATA{"printer"}."with ".$TAGS_DATA{"printer_ip"}." and ".$TAGS_DATA{"MANDAT"}.", document level ".$TAGS_DATA{"key_protect"});
		cancel_print_job($TAGS_DATA{"jobID"},$TAGS_DATA{"printer"});
		exit 1;
	}
}else{
	cancel_print_job($TAGS_DATA{"jobID"},$TAGS_DATA{"printer"});
}

@result =get_fp_pdfname_and_make($TAGS_DATA{"jobID"},$TAGS_DATA{"user"},$TAGS_DATA{"jobTitle"},$TAGS_DATA{"TMP_FILENAME"});
if ($result[0]){
	$TAGS_DATA{"FP_PDF_NAME"}=$result[1];
}else{
	save_debug_msg ($result[1]);
	cancel_print_job($TAGS_DATA{"jobID"},$TAGS_DATA{"printer"});
	exit 1;
}

@result =save2base();# Запишем в базу собранные данные. Пишем в базу независимо от настроения фильтра и услови завершения работы
unless ($result[0]){
	save_debug_msg ($result[1]);
	cancel_print_job($TAGS_DATA{"jobID"},$TAGS_DATA{"printer"});
	exit 1;
}
exit 0;

END{
	$dbh->disconnect();
}
#----------------------------ENTANGLED sub's :) ----------------------------------------------------------------------
sub save2base{
	#Ags: nothing
	#Returns array containing code operation,info string;
	return; 
}
sub save_debug_msg{
	#Ags: Info string
	#Returns: bothing
	my ($info_str)=@_;
	chomp($info_str);
	unless ($dbh){
		print STDERR $info_str,"\n";
	}else{
		$dbh->insert("debug_log","info_str",$dbh->{DBH}->quote($info_str));
	}
}


sub get_printers_mandat{
	#Ags printer_ip,printer_name;
	#Returns array containing code operation,info string;
	my($printer_ip,$printer_name)=@_;
	
	#TODO add check IP valid
	if ($printer_ip =~ m/^(\d\d?\d?).(\d\d?\d?).(\d\d?\d?).(\d{1,})$/){
		unless($1 <= 255 && $2 <= 255 && $3 <= 255 && $4 <= 255){
        	return (0,"One of the octets is out of range.  All octets must contain a number between 0 and 255 \n");
    	}	
	}else{
    	return (0,"IP Address $printer_ip  -->  NOT IN VALID FORMAT! \n");
	}
	
 	my $pid = open (F,"$RACF_UTIL_BIN $printer_ip $printer_name|") or die "Can't find and execute $!\n";
    my @res = <F>;
    close (F);
    if (defined $res[1]){
    	return (1,$res[1]);	
    }
    return (0,"Printers with IP = $printer_ip,name =$printer_name has't Mandat !\n"); 	
}
sub parse_file{
	#Arg: printFile, tmp_filehandle;
	#Returns: array containing code operation,info string;
	my ($printFile,$fh_tmp)=@_;
	my $source=\*STDIN;#Используем в качестве источника стандартный ввод
	my $err_str;	
	#find file source...STDIN, or as argument and open it
	if (defined $printFile){
		open ($source, "< $printFile") or die "CUPS_FILTER. ERROR: Can't open printjob $printFile: $!\n";
	}
	while(<$source>){
		#Read block from <start_data> to </start_data>  
		if (/<start_data>/i.../<\/start_data>/i){
			#Find tags and save to hash. Tags special format %%<key_TAG-NAME>TAG-VALUE</key_TAG-NAME>
			if (m/^\%{2}<key_(.*?)>(.*?)<\/key_(.*?)>$/gi){
				unless (exists($TAGS_DATA{$1})){
					$TAGS_DATA{$1}=$2;# заносим в хеш найденный элемент
				}else {
					#print STDERR $_,"=",$1,"-",$2,"-",$3,"\n";
					$err_str="\nError TAGS_DATA - [key->$1, value->$2] is dublicat!";
					$TAGS_DATA{"parse_error"}.=$err_str;
				}
			}
		}
		print $fh_tmp "$_";
	}
	if (defined $printFile){
		close $source;
	}
	if (defined $err_str){
		return (0,$err_str);
	}else{
		return (1,"succsesfull parsing file")	
	}
}

sub get_fp_pdfname_and_make_pdf{
	#Arg: jobID,user,jobTitle,tmp_filename;
	#Returns: array containing code operation,info string;
	my ($jobID,$user,$jobTitle,$tmp_filename)=@_;
	
	$jobTitle = substr($jobTitle,0,12); # 
    # Put the pieces together to create a usable filename
    $TAGS_DATA{"fp_pdf_filename"}= join('-',$jobID,$user,$jobTitle);
    
    my $tm = localtime();
    my ($day,$month,$year,$hh,$mm) = ($tm->mday,$tm->mon,$tm->year,$tm->hour,$tm->min);
    $year+=1900;
    $month+=1;
    if (length($month) == 1){
        $day='0'.$month;
    }
    if (length($day) == 1){
        $day='0'.$day;
    }
    if (length($hh) == 1){
        $hh='0'.$hh;
    }
    if (length($mm) == 1){
        $mm='0'.$mm;
    }
    # Присоединяем диск
    my $dir_name=join('/',$FPAGEDIR,$year,$month,$day);
    # Проверяем, есть ли такая директория, если нет -- создаем
	unless(-d $dir_name) {
		mkpath($dir_name);
	}
    $TAGS_DATA{"fp_pdf_filename"}=$dir_name."//".$hh.$mm."_".$TAGS_DATA{"fp_pdf_filename"}.".pdf";
    #my $pid = open (F,"$PSSELECT_BIN -p1 $tmp_filename | $PDF_BIN - ".$TAGS_DATA{"fp_pdf_filename"}."&") or die "Can't find and execute $!\n";
    system (F,"$PSSELECT_BIN ".$TAGS_DATA{"fp_pdf_filename"}."&") or die "Can't find and execute $!\n";
    return (1,$TAGS_DATA{"fp_pdf_filename"});
}

sub cancelPrintJob{
	# Args: jobid, queuename
    # Returns: nothing, deletes job and re-enables queue
    my ($jobid,$queue) = @_;
    #TODO add check defined $jobid,$queue !!!
    
    sleep 1;
    if (defined $jobid && defined $queue){
    	unless ($DEBUG){
    		system("/usr/bin/lprm","$jobid");
    		sleep 1;
    		system("$ENABLE","$queue");
    	}
    }	
}

sub cleaner{
	#Arg: Dirty string
	#Returns clear string or nothing
	
	my($t)=@_;
    
    #Чистим на всякий случай от html тегов
    $t=~s/\&quot;/ /g;
    $t=~s/\&gt;/ /g;
    $t=~s/\&lt;/ /g;
    $t=~s/\&apos;/ /g;
    $t=~s/\&shy;//g;
    # Убираем все символы кроме алфавитных символов с учетом локали
   $t =~ s/[^\pL]+//g;      
    # Убираем лишние пробелы
    $t =~ s/\s+/ /g;
    $t =~ s/^\s+//;
    $t =~ s/\s+$//;    
	return $t;
}
#------------------------------------------------------------------------------------------------------------------------
