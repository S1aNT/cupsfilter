#!/usr/bin/perl  -w
use strict;
use DBI;

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

    if ($DBI::errstr) {
        $error_str="CUPS_FILTER: Erorr $DBI::errstr\n";
        print STDERR $error_str;
        return (0,$error_str);
    }else{
        $dbh->begin_work();
        $sth = $dbh->prepare($query);
        $rv = $sth->execute();
        $dbh->commit();
        if (!defined $rv) {
            $error_str="CUPS_FILTER: При выполнении запроса $query\n Ввозникла ошибка:".$dbh->errstr."\n";
	    print STDERR $error_str;
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


sub get_id_firstRowCow { 
    #Arg: $Ptr to array of rows
    #Returns: array [1/0 succesfull or error,printers_id]
    my ($array_ref) =@_;
    if (defined $array_ref->[0][0]){
	return (1,$array_ref->[0][0]);
    }else{
          return (0,"Printer not found in base");
     }
} 

my $ip="192.168.11.151";
my $printer_name="PDF";
my $mandat="SYSPROG";
my @res;
my $q ="select id,ip,mandat,printer_name from printers where ip='$ip' and printer_name='$printer_name' and mandat ='$mandat'";
#@res=sql_execute($q,\&get_id_firstRowCow);

# Запишем в БД данные о пути к тервой странице и вернем ID записи
my $filename='/var/log/cups/firstpages/2009/05/19/20.21.23_slant-test44_doc.pdf';
$q ="INSERT INTO firstpages (fp_filename)VALUES('$filename')";
@res=sql_execute($q);
$q ="SELECT ID FROM firstpages WHERE fp_filename='$filename'";
@res=sql_execute($q,\&get_id_firstRowCow);
print @res;