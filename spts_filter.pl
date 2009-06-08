#!/usr/bin/perl  -w
use strict;
use locale;
use utf8;
use Encode 'from_to';
use Encode;
use File::Copy "cp";
use File::Path;
use Time::localtime;
use IO::File;
use File::Temp;
use SQLShell;
use DBI;
use feature "switch";

#use File::Temp qw/ :seekable /;
#require File::Temp;

#----------------------- CONFIG ------------------------------------
my $DEBUG=1;							# For debuging and troubleshooting. Nice hunt !

my @argv = @ARGV;			#keep hands off the argument array
my $SPOOL = "/tmp";			#where are my speedy memory drive?
my $LOG_FILE = "/var/log/cups/spts_filter.log";
my $DEBUG_DB_FILE = "/var/log/cups/spts_db_filter.log";
my $FPAGEDIR   ="/var/log/cups/firstpages"; #каталог в котором хранятся первые страницы документа
my $DATE_BIN = "/bin/date";				#path to date
my $PDF_BIN="/usr/bin/ps2pdf";			#path to ps2pdf binary
my $PSSELECT_BIN ="/usr/bin/psselect"; 	#path to psselect binary
my $ENABLE ="/usr/bin/cupsenable"; 			#path to enable binary
my $RACF_UTIL_BIN="/usr/bin/racf.pl";	#path to racf script
my $FH_TEMP;							#File handle to temp file

my %TAGS_DATA=();						#Hash contain's parsed data from print job
my $REPORT_ID;							# Для любого задания вначале формируется предварительяная запись в таблице отчетов, 
										# которая потом дополянется сведениями. 
my @result;								#Array of function result (Code,InfoStr)




#--------------------------------------------------------------------------------------------------

#Check and parse arguments
if ($DEBUG){
	$RACF_UTIL_BIN="racf.pl";
	$PSSELECT_BIN ="psselect.pl";
	$SPOOL ="d:\\Temp\\";
	$DEBUG_DB_FILE = "spts_db_filter.log";
	@argv=(78,'PDF writer \@nt8','Пример модного документа с числом копий = 8',7,"JUID:2","sample.ps");
}

my ($jobID,$userName,$jobTitle,$copies,$printOptions,$printFile) = @argv;

$TAGS_DATA{"user"}=lc(cleaner($userName));
$TAGS_DATA{"jobID"}=$jobID;
$TAGS_DATA{"class"}=$ENV{"CLASS"} if (defined $ENV{"CLASS"});
$TAGS_DATA{"copies"}=$copies;
$TAGS_DATA{"printer"}=$ENV{"PRINTER"} if (defined $ENV{"PRINTER"});
$TAGS_DATA{"jobTitle"}= lc(cleaner($jobTitle));
$TAGS_DATA{"jobTitle"}=~ s/smbprn[0-9]*//;
$TAGS_DATA{"jobTitle"}=~ s/microsoft|word|explorer//gi;
$TAGS_DATA{"printFile"}=$printFile;
$TAGS_DATA{"device_uri"}=$ENV{"DEVICE_URI"} if (defined $ENV{"DEVICE_URI"});
$TAGS_DATA{"printOptions"}=$printOptions;


#--------------------------------------------------------------------------------------------------
# If no arguments, device discovery mode
if (!$argv[0]){
        print STDERR ("direct tech_filter \"Unknown\" \"TechnoServ Filter \"\n");
        exit 0;
}
# If too many arguments, send error
if (scalar(@argv) < 5 || scalar(@argv) > 6){
        print STDERR ("Usage: tech_filter job-id user title copies options [file]\n");
        exit 1;
}
#--------------------------------------------------------------------------------------------------
#The hard, but fine work :)
# Safe create temp file
$FH_TEMP = File::Temp->new( TEMPLATE => 'cups_jobXXXXX',
                                DIR => $SPOOL,
                                SUFFIX => '.~tmp');

$TAGS_DATA{"TMP_FILENAME"} = $FH_TEMP->filename();

my $dbh = new SQLShell; # Connect to database
#Заполним массив данными требуемыми для соединения с БД
$dbh->options(	"CupsLog",	# имя базы данных
				"postgres",	# имя пользователя
				"pg",		# пароль
				"localhost",# имя или IP адрес сервера
				"5432",		# порт
				"-e",		# опции
				"ansi");	# терминал
eval {
	$dbh->connect();
	$dbh->_doexec("SET CLIENT_ENCODING TO 'UTF-8'");
};
if ($@){
	print STDERR $@,"\n";
	save2log($@);
	cancel_print_job($TAGS_DATA{"jobID"},$TAGS_DATA{"printer"});
	exit 1;
}else{
	# main working block 
	$dbh->debug(3,"$DEBUG_DB_FILE") if ($DEBUG);
	$REPORT_ID = save_cups_data2base($TAGS_DATA{"printer"},$TAGS_DATA{"user"},$TAGS_DATA{"jobTitle"},$TAGS_DATA{"copies"},'','',$TAGS_DATA{"jobID"});
	@result = parse_file($TAGS_DATA{"printFile"},$FH_TEMP);
	
	#Отправим дальше на печать 
	file2stdout($FH_TEMP);
	
	if ($result[0]){
		@result =get_fp_pdfname_and_make($TAGS_DATA{"jobID"},$TAGS_DATA{"user"},$TAGS_DATA{"jobTitle"},$TAGS_DATA{"TMP_FILENAME"});
		$TAGS_DATA{"FP_PDF_NAME"}=$result[1];
	}else{
		save_debug_msg ($result[1]) if ($DEBUG || !$result[0]);
	}
	save_data2base($REPORT_ID);
	
}
exit 0;

#--------------------------------------------------------------------------------------------------

sub save_data2base{
	#Args: REPORT_ID
	#Returns: nothing;
	my($rep_id) =@_;
	my $k;
	
	unless (exists $TAGS_DATA{"executor_telephone"}){
		$TAGS_DATA{"executor_telephone"}='000';
	};
	
   	foreach $k (keys %TAGS_DATA) {
   		$TAGS_DATA{$k} = digit2utf ($TAGS_DATA{$k}) if ($TAGS_DATA{$k} =~ /(\d{1,4};;)+/);
   		my $v =$TAGS_DATA{$k};
   		
		#$v =digit2utf($v) if ($v =~/\d{1,4};;/);
		 
   		my @tmp  =($rep_id,$v);
   		print "Key $k: value $v \n" if ($DEBUG);
       	given (lc($k)){
       		when ("fp_pdf_name") {
       			$dbh->just_do("reports_set_fp_pdf_name",\@tmp);
			}
       		when ("document_level") {
       			$dbh->just_do("reports_set_doc_level",\@tmp);
			}
			when ("copy_number"){
				$dbh->just_do("reports_set_copy_number",\@tmp);
			}
			when ("punkt"){
				$dbh->just_do("reports_set_punkt",\@tmp);
			}
			when ("page_count"){
				$dbh->just_do("reports_set_page_count",\@tmp);
			}
			when ("executor_fio"){
				push @tmp,'executor';
				push @tmp,$TAGS_DATA{"executor_telephone"};
				$dbh->just_do("reports_set_exec_print_fio",\@tmp);
			}
			when ("printed_fio"){
				push @tmp,'pressman';
				push @tmp,$TAGS_DATA{"executor_telephone"};
				$dbh->just_do("reports_set_exec_print_fio",\@tmp);
			}
			when ("inv_number"){
				$dbh->just_do("reports_set_inv_number",\@tmp);
			}
			when ("mb_number"){
				$dbh->just_do("reports_set_mb_number",\@tmp);
			}			
			when (/recivers_\d{1}/){
				$dbh->just_do("reports_set_recivers",\@tmp);
			}
			when (/add_info_\d{1}/){
				push @tmp,$k;
				$dbh->just_do("reports_set_add_info",\@tmp);
			}
   		}
   	}
}


sub digit2utf{
	#Arg: digit_str example 1069;;1090;;1086;;32;;1084;;1086;;1081;;32;;1076;;1086;;1082;;1091;;1084;;1077;;1085;;1090;;44;;32;;1089;;1090;;1088;;46;;321
	#Returns: wide char string
	my ($str)=@_;
	my $t = pack("U*",split (';;',$str));
	#$t= encode( "UTF-8", $t); 
	return $t;
}


sub save_cups_data2base{
	#Args:printer_name,cups_user,jobTitle,page_copy,status,info_str,jobsID
	#Returns: record_id
	my (@tmp) =@_;
	my @res =get_id_firstRowCow( $dbh->just_do("reports_set_cups_data",\@tmp));
	if ($res[0]){
		return $res[1];
	}else{
		return;
	}
}
sub get_id_firstRowCow { 
    #Arg: $Ptr to array of rows
    #Returns: array [1/0 succesfull or error,value of First Row Cow]
    my ($array_ref) =@_;
    if (defined $array_ref->[0][0]){
	return (1,$array_ref->[0][0]);
    }else{
          return (0,"Cell not found in base");
     }
}

sub datetime2string{
	#Arg:nothing
	#Returns: date time string
	my @months_names=("JANUARY","FEBRUARY","MARCH","APRIL","MAY","JUNE","JULY","AUGUST","SEPTEMBER","OCTOBER","NOVEMBER","DECEMBER");
	
	my $tm = localtime();
    my ($day,$month,$year,$hh,$mm) = ($tm->mday,$tm->mon,$tm->year,$tm->hour,$tm->min);
    $year+=1900;
    if (length($day) == 1){
        $day='0'.$day;
    }
    if (length($hh) == 1){
        $hh='0'.$hh;
    }
    if (length($mm) == 1){
        $mm='0'.$mm;
    }
    my $date =join('/',$year,$months_names[$month],$day);
    my $time =join(':',$hh,$mm);
    return join ('/',$date,$time); 
}

sub save2log{
	#Arg: info_str
	#Returns: nothing
	my($info_str)= @_;
	open (LOG,">> $LOG_FILE") or die "can't open file $LOG_FILE: $!";
	print LOG "--------------------------Date ", datetime2string()," ---------------------\n";
	print LOG $info_str,"\n";
	print LOG "---------------------------------------------------------------------------\n";
	close (LOG);
}

sub file2stdout{
    # Args: pointer to tempFile
 	# Returns: nothing, just print file to stdout
    my($fn)=@_;
	if (defined $fn){
		open (PSFILE, "<$fn") || croak( "CUPS_FILTER. ERROR: Can't open $fn: $!\n");
		while (<PSFILE>){
		        print $_;
		}
		close PSFILE;	
	}else{
		croak("CUPS_FILTER. File name not defined !");
	}
}

sub parse_file{
	#Arg: printFile, tmp_filehandle;
	#Returns: array containing code operation,info string;
	my ($printFile,$fh_tmp)=@_;
	my $source=\*STDIN;#Используем в качестве источника стандартный ввод
	my $err_str;	
	#find file source...STDIN, or as argument and open it
	if (defined $printFile){
		open ($source, "< $printFile") or croak ("CUPS_FILTER. ERROR: Can't open printjob $printFile: $!\n");
	}
	while(<$source>){
		#Read block from <log_data> to </log_data>  
		if (/<log_data>/i.../<\/log_data>/i){
			#Find tags and save to hash. Tags special format %%<key_TAG-NAME>TAG-VALUE</key_TAG-NAME>
			if (m/^\%{2}<key_(.*?)>(.*?)<\/key_(.*?)>$/gi){
				unless (exists($TAGS_DATA{$1})){
					
					$TAGS_DATA{$1}=$2;# заносим в хеш найденный элемент
				}else {
					print STDERR $_,"=",$1,"-",$2,"-",$3,"\n";
					$err_str="\nError TAGS_DATA - [key->$1, value->$2] is dublicat!";
					save_debug_msg ($err_str) if ($DEBUG);
					$TAGS_DATA{"parse_error"}.=$err_str;
				}
			}
		}
		#TODO поиск числа страниц в файле конструкция вида %%Pages: 39
		if (m/^\%{2}Pages\:\s*(\d{1,})\.*$/){
			$TAGS_DATA{"PAGE_COUNT"}=$1;# заносим в хеш число страниц в документе
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

sub get_fp_pdfname_and_make{
	#Arg: jobID,user,jobTitle,tmp_filename;
	#Returns: array containing code operation,info string;
	my ($jobID,$user,$jobTitle,$tmp_filename)=@_;
	
	$jobTitle = substr($jobTitle,0,25); # 
    # Put the pieces together to create a usable filename
    my $fp_pdf_filename= join('-',$jobID,$user,$jobTitle);
    
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
    $fp_pdf_filename="$dir_name/$hh.$mm"."_".$fp_pdf_filename.".pdf";
    if ($DEBUG){
    	print "$PSSELECT_BIN -p1 $tmp_filename | $PDF_BIN - $fp_pdf_filename &","\n"; 
    }else{
    	system ("$PSSELECT_BIN -p1 $tmp_filename | $PDF_BIN - $fp_pdf_filename &") or croak ("Can't find and execute $PSSELECT_BIN \n");
    }
    
    return (1,$fp_pdf_filename);
}
sub cancel_print_job{
	# Args: jobid, queuename
    # Returns: nothing, deletes job and re-enables queue
    my($jobid,$queue) = @_;
    #TODO add check defined $jobid,$queue !!!
    
    sleep 1;
    if (defined $jobid && defined $queue){
    	system("/usr/bin/lprm","$jobid");
    	sleep 1;
    	system("$ENABLE","$queue");
    }	
}
sub cleaner{
	#Arg: Dirty string
	#Returns clear string or nothing
	
	my($t)=@_;
    
    #Чистим на всякий случай от html тегов
    $t =~ s/\&([^(amp;|lt;|gt;|nbsp;|&quot;|&apos;)])/ /g; 
    # Убираем все символы кроме алфавитных символов с учетом локали
    #$t =~s /[^\s\d\.\-а-яa-z]+//gsi;
    $t =~s /[^\pL\s\d]+//gsi;     
    # Убираем лишние пробелы
    $t =~ s/\s+/ /g;
    $t =~ s/^\s+//;
    $t =~ s/\s+$//;    
	return $t;
}
