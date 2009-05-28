#!/usr/bin/perl  -w
use strict;
use DBI;

my $DEBUG=1;
my $RACF_UTIL_BIN;

my @argv = @ARGV;			#keep hands off the argument array
my ($printer_ip,$printer_name)=@argv;

### ARG CHECKING
# If no arguments, print help
if (!$argv[0] || scalar(@argv)> 2){
        print ("Usage racf2base.pl PRINTER_IP PRINTER_NAME\n");
        exit 1;
}

if ($DEBUG){
	$RACF_UTIL_BIN="racf.pl";
	$printer_ip="192.168.11.151";
	$printer_name="PDF";
	
	#for(keys(%ENV)) {
    #   print (STDERR "$_ = $ENV{$_}\t");
    #}
}else {
        $RACF_UTIL_BIN="/usr/bin/racf.pl";
}

my @result = save_mandat2base($printer_ip,$printer_name);
print "Output $result[1] with code $result[0]";

#TODO Написать общую функцию записи в лог
#TODO need use SQLShell.pm !!!
##----------------------------------------------------------------------

sub save_mandat2base{
	#Args: printer_ip,printer_name
	#Returns 1/0 succsesfull or errors
	my($printer_ip,$printer_name)=@_;
	my $mandat=get_data_from_racf($printer_ip,$printer_name);
	if (defined $mandat){
		#Find printer in base
		my $q="SELECT id FROM printers WHERE ip='$printer_ip' and printer_name='$printer_name'";
		my @res=sql_execute($q,\&get_id_firstRowCow);	
		if ($res[0]){
			$q="UPDATE printers SET mandat='$mandat' WHERE ip='$printer_ip' and printer_name='$printer_name'";
			@res = sql_execute($q);
			return @res;  
		}else {
			#Add printer to base
			$q="INSERT INTO printers (ip,printer_name,mandat)VALUES('$printer_ip','$printer_name','$mandat')";
			@res = sql_execute($q);
			return @res;
		}
	}
	return (0,"unable save printer's mandat into base!");
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
sub get_data_from_racf{
    #Arg: printer_ip,printer_name
    #Returns Mandat or UNDEF 
    my ($printer_ip,$printer_name)=@_;
    my $pid = open (F,"$RACF_UTIL_BIN $printer_ip $printer_name|") or die "Can't find and execute $!\n";
    my @res = <F>;
    close (F);
    if (defined $res[1]){
    	return $res[1];	
    }
    return; 
}

sub sql_execute{
    #Arg: $query_str,Ptr to function
    #Returns: array [1/0 succesfull or error,description]
    my ($query,$func)=@_;
    my @result;
    my $dbname = "CupsLog";# имя базы данных
    my $username = "postgres";# имя пользователя
    my $password = "pg";# пароль
    my $dbhost = "localhost";# имя или IP адрес сервера
    my $dbport = "5432";# порт
    my $dboptions = "-e";# опции
    my $dbtty = "ansi";# терминал
    my $sth;
    my $rv;
    my $error_str;
    
    my $dbh = DBI->connect("dbi:PgPP:dbname=$dbname;host=$dbhost;port=$dbport;options=$dboptions;tty=$dbtty","$username","$password",{PrintError => 0, AutoCommit => 0 });
	if ($DEBUG){
		my $trace_file="dbh.log";
		$dbh->trace(2,$trace_file);
	}
	
    if ($DBI::errstr) {
        $error_str="CUPS_FILTER: Erorr $DBI::errstr\n";
        print STDERR $error_str;
        return (0,$error_str);
    }else{
    	#TODO Need refactoring !!!
    	$dbh->begin_work();
    	unless (defined $func){
    		#INSERT, UPDATE or DELETE
    		$rv=$dbh->do($query);
    	}else {
    		#SELECT
    		$sth = $dbh->prepare($query);
        	$rv = $sth->execute();
    	}
        $dbh->commit();
        if (!defined $rv) {
            $error_str="CUPS_FILTER: При выполнении запроса $query\n Ввозникла ошибка:".$dbh->errstr."\n";
	    	#print STDERR $error_str;
            return (0,$error_str);
        }else{
            if (defined $func){
				@result=$func->($sth->fetchall_arrayref());
                $sth->finish();
	    	}else{
	      		push @result,1,"Succsefull execute";
	    	}
        }
    }
    $dbh->disconnect();
    return @result;
}

