#!/usr/bin/perl  -w
use strict;
use File::Copy;
use File::Path;


my $DEBUG       =1;# Режим отладки

my @argv = @ARGV;			#keep hands off the argument array
my $spool = "/tmp";			#where are my speedy memory drive?
my $DEBUG_FILE = "/var/log/cups/tech_filter_debug.log";
my $FPAGEDIR   ="/var/log/cups/firstpages"; #каталог в котором храняться первые страницы документа

if ($DEBUG){
	$spool = "D:\\Temp";			#where are my speedy memory drive?
	$DEBUG_FILE = "D:\\Temp\\tech_filter_debug.log";
	$FPAGEDIR   ="D:\\Temp\\firstpages"; #каталог в котором храняться первые страницы документа
}

my $DATE_BIN = "/bin/date";		#path to date
my $PDF_BIN="/usr/bin/ps2pdf";			#path to ps2pdf binary
my $PSSELECT_BIN ="/usr/bin/psselect";
my $ENABLE ="/usr/bin/enable";

my $flag =0;	# Тригер печати 1 стоп печати 0 отправить файл дальше на печать

my %TAGS_DATA		= (); # Hash contains resived field
my ($jobID,$userName,$jobTitle,$copies,$printOptions,$printFile) = @argv;
### ARG CHECKING
# If no arguments, device discovery mode
if (!$argv[0]){
        print ("direct tech_filter \"Unknown\" \"TechnoServ Filter \"\n");
        exit 1;
}
# If too many arguments, send error
if (scalar(@argv) < 5 || scalar(@argv) > 6){
        print STDERR ("ERROR: Usage: tech_filter job-id user title copies options [file]\n");
        exit 1;
}
## First blood....
#Заполним хеш данными из cups
$TAGS_DATA{"class"}   = $ENV{"CLASS"};
if ($DEBUG){
        $TAGS_DATA{"printer"} = "PDF" unless $ENV{"PRINTER"};
        $TAGS_DATA{"device_uri"} = "http://127.0.0.1:631/printers/PDF";# unless $ENV{"DEVICE_URI"};
	
}else {
        $TAGS_DATA{"device_uri"} = $ENV{"DEVICE_URI"};
        $TAGS_DATA{"printer"} =  $ENV{"PRINTER"};
}

$TAGS_DATA{"jobID"}=$jobID;
$TAGS_DATA{"user"}=$userName;
$TAGS_DATA{"jobTitle"}=$jobTitle;
$TAGS_DATA{"copies"}=$copies;
$TAGS_DATA{"printOptions"}=$printOptions;
$TAGS_DATA{"printFile"}=$printFile;

$TAGS_DATA{"device_uri"} =~ m/.*?:\/{1,2}(.*?)\/.*/gis;
if (defined $1){
	$TAGS_DATA{"printerIP"} = $1;
	$TAGS_DATA{"printerIP"} =~ s/:.*//;  # Pull off port number if present
       
        #Проверка на защиту принтера
        #Запрос к cups_log.printers Если в БД нет такого принтера то запуск скрипта проверки мандата принтера
        $TAGS_DATA{"printer_protect"}=check_printer($TAGS_DATA{"printer"},$TAGS_DATA{"printerIP"});
}else {
        $TAGS_DATA{"ERROR"}.= "Unable check printer.Not network printer usage or not valid printer IP adress\n";
        $flag = cancelPrintJob(\$TAGS_DATA{"jobID"},\$TAGS_DATA{"printer"});
	exit 1;
}

if ($DEBUG){
	debug2file(\%TAGS_DATA);
}

#---------------------------------------------------------------------------------------
#Разберем файл и отправим его дальше с записью в базу или прервем задание на печать
if (create_and_parse_file(\%TAGS_DATA)){ # Создали временный файл и разобрали его
	first_page_gen(\%TAGS_DATA); # Сформируем первую страницу документа
	if ($TAGS_DATA{"printer_protect"}){#Принтер защищаемый
		unless (exists($TAGS_DATA{"protected"})){#Документ не имеет гриф секр.
			$TAGS_DATA{"NSD"}="Попытка печати на защищаемом принтете не грифованного документа";
                        $flag = cancelPrintJob(\$TAGS_DATA{"jobID"},\$TAGS_DATA{"printer"});
		}
	}else { # Принтер не защишаемый
                if (exists($TAGS_DATA{"protected"})){#Документ имеет гриф
			TAGS_DATA{"NSD"}="Попытка печати на не защищаемом принтете грифованного документа";
			$flag = cancelPrintJob(\$TAGS_DATA{"jobID"},\$TAGS_DATA{"printer"});
		}
        }
}else {
        $flag =  cancelPrintJob(\$TAGS_DATA{"jobID"},\$TAGS_DATA{"printer"});
}

unless ($flag){
	file2stdout(\$TAGS_DATA{"tempFile"}); # Печатем файл если не прерывали задание
}

if ($DEBUG){
	debug2file(\%TAGS_DATA);
}else {
	save2base(\%TAGS_DATA);# запись в базу собранных данных ВСЕГДА !!!
}

if (-e $TAGS_DATA{"tempFile"}) {
	unlink($TAGS_DATA{"tempFile"});# Удалим временный файл
}

exit 0;
#----------------------------------------------------

sub file2stdout{
        # Args: pointer to tempFile
 	# Returns: nothing, just print file to stdout
        my($fn)=@_;
	if (defined $$fn){
		open (PSFILE, "<$$fn") or die "CUPS_FILTER. ERROR: Can't open $fn: $!\n";
		while (<PSFILE>){
		        print $_;
		}
		close PSFILE;	
	}else{
		die "CUPS_FILTER. File name not defined !";
	}
}

sub create_and_parse_file{
        # Args: pointer to hash ->TAGS_DATA
 	# Returns: 1 Если создание временного файла и разбор поступающих данных, произошли успено
        my($href)=@_;
	if ($DEBUG){
		$$href{"create_and_parse_file"}="start";
	}
	#TODO Добавить проверку существования ключей в хеше
	# Establish a temp file to which to spool
	unless (exists $$href{"userName"}){
		$$href{"userName"}="empty";
	}
	my $jid = $$href{"jobID"};
	my $uid = $$href{"userName"};
	my $source;
	$jid =~ s/\W//g;        # sanity check
	$uid =~ s/\W//g;        # sanity check
	my $printFile=$$href{"printFile"};
	my $tempFile = "$spool/$jid-$uid-cupsjob$$";
	open (OUT, ">$tempFile") or die "CUPS_FILTER. ERROR: Can't write $tempFile: $!\n";
	
	#find file source...STDIN, or as argument and open it
	unless (defined $printFile){
		$source = "STDIN";#Используем в качестве источника стандартный ввод
        }else{
		open ($source, "< $printFile") or die "CUPS_FILTER. ERROR: Can't open printjob $printFile: $!\n";
	}
	while(<$source>){
		#Разберем каждую строчку на предмет наличия нужных данных
		m/\%\%<(.*?)>(.*?)<\/(.*?)>/gmis;
		if ((defined $1) and (defined $2) and (defined $3) and ($1 eq $3)){
			unless (exists($$href{$1})){
				$$href{$1}=$2;# заносим в хеш найденный элемент
			}else {
				$$href{"error"}.="\nError TAGS_DATA - [key->$1, value->$2] is dublicat!";
			}
		}			
		print OUT "$_";
	}
	if (defined $printFile){
		close $source;
	}
	close (OUT);
	$$href{"tempFile"}= $tempFile;
	return 1;
}

sub debug2file{
        # Args: pointer to hash ->TAGS_DATA
 	# Returns: nothing, just print some debug info
        my($href)=@_;
        my $date=`$DATE_BIN +%T%t%D`;
        
        open (DEBUG_OUT,"> $DEBUG_FILE") or die "CUPS_FILTER. Can't open file $DEBUG_FILE: $!";
        print (DEBUG_OUT "\n-------------	$date	------------------\n arguments passed to program:\n");
        my $n=0;
        foreach (@ARGV){
                print (DEBUG_OUT "$n:$_\t");
                $n++;
        }
        print (DEBUG_OUT "\nenvironment variables:\n");
        for(keys(%ENV)) {
                print (DEBUG_OUT "$_ = $ENV{$_}\t");
        }
        print (DEBUG_OUT "\n=============\n");
        my $key;
        foreach $key (keys %$href){
                print (DEBUG_OUT $key," -> ",$$href{$key},"\n");
        }
        close (DEBUG_OUT);
}
sub save2base{
        # Args: pointer to hash ->TAGS_DATA
        my($href)=@_;
        if ($DEBUG){
		$$href{"save2base"}="start";
                debug2file(\$href);
        }else{
                #запишем в базу данные
        }
}

sub first_page_gen{
        # Args: pointer to hash ->TAGS_DATA
        my($href)=@_;
	if ($DEBUG){
		$$href{"first_page_gen"}="start";
	}
        $$href{"user"} =~ s/\W//g;
        $$href{"user"} = lc($$href{"user"});
        # Develop a decent title
        my $title = $$href{"jobTitle"};	# sanitize job title
        $title = lc($title);
        $title =~ s/\W//g;
        $title =~ s/smbprn[0-9]*//;
        $title =~ s/microsoft|word|explorer//gi;
        $title = substr($title,0,12);
        # Put the pieces together to create a usable filename
        $$href{"fp_pdf_filename"}= join('-',$$href{"jobID"},$$href{"user"},"$title.pdf");        
        my @dateStamp = split('_',getTime());
        # Присоединяем диск
        my $dir_name=join('/',$FPAGEDIR ,$dateStamp[0]);
        # Проверяем, есть ли такая директория, если нет -- создаем
	unless(-d $dir_name) {
		mkpath($dir_name);
	}
        if (exists ($$href{"NSD"})){
                $dir_name.="_NSD";
        }
        my $fp_pdf_filename=join('_',$dateStamp[1],$$href{"fp_pdf_filename"});
	$fp_pdf_filename=join('/',$dir_name,$fp_pdf_filename);

	my $inputFile =$$href{"tempFile"};
        $$href{"fp_pdf_filename"}=$fp_pdf_filename;
	`$PSSELECT_BIN -p1 $inputFile | $PDF_BIN - $fp_pdf_filename &`; #TODO добавить обработчик ошибок контейнера
}

sub check_printer{
	# Args: printer, device_uri	
        # Returns:  printer_protected 1/0 protect / unprotected
	my ($printer,$printerIP) = @_;
	my $printer_protected;
	$printer_protected=1;
	
	return $printer_protected;
}

sub cancelPrintJob{
        # Args: jobid, queuename
        # Returns: nothing, deletes job and re-enables queue
        my ($jobid,$queue) = @_;
	sleep 1;
        system("/usr/bin/lprm","$$jobid");
        sleep 1;
        system("$ENABLE","$$queue");
	return 1;
}
sub getTime{
	my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime(time);
	$year = $year + 1900;
	$mon = $mon + 1;
	if ($mon < 10){
	    $mon = "0$mon";
	}
	if ($mday < 10){
	    $mday = "0$mday";
	}
	if ($hour < 10){
	    $hour = "0$hour";
	}
	if ($min < 10){
	    $min = "0$min";
	}
	if ($sec < 10){
	    $sec = "0$sec";
	}
	return "$year/$mon/$mday/_$hour.$min.$sec";
}

sub diff_time{
    my ($t1,$t2)= @_;
    my ($diff,$DD,$HH,$MM,$SS);
    $diff  = $t2 - $t1;
    $SS = $diff % 60;
    $diff  = ($diff - $SS)/60;
    $MM = $diff % 60;
    $diff  = ($diff - $MM)/60;
    $HH = $diff % 24;
    $diff  = ($diff - $HH)/24;
    $DD = $diff % 7;
    return "\nProgramm work's $DD days, $HH hours, $MM min, $SS sec";
}
 
#---------------------------- End sub -----------------------------------------------------

