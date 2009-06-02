use SQLShell;

# Эта функция создает новую запись независимо от драйвера для работы со СУБД.
sub insert_record {
    my ($db_driver, $body_text) = @_;

    if ( !$db_driver->isa('ISQLDriver') ) {
        die "$db_driver is not an ISQLDriver";
    }
    $db_driver->insert("Records(body)", "'$body_text'");
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

my $dbh_1 = new SQLShell;

# Заполним массив данными требуемыми для соединения с БД

$dbh_1->options("CupsLog",	# имя базы данных
				"postgres",	# имя пользователя
				"pg",		# пароль
				"localhost",# имя или IP адрес сервера
				"5432",		# порт
				"-e",		# опции
				"ansi");	# терминал

$dbh_1->debug(3,"dbh.log");
$dbh_1->connect();
$dbh_1->debug(3,"dbh.log");

print $dbh_1->options();
print $dbh_1->VERSION;
print "Insert records from PgSQL:\n";

my @tmp =('PDF','slant1','сборник практических заданий по тсп для слушателей учебной группы','5','SAVE_CUPS_DATA','','42');
my $t = $dbh_1->just_do("set_cups_log",\@tmp);
my $test =  $dbh_1->just_do("check_printer_perm","PDF","192.168.11.151","SYSPROG","1");

my $info=$dbh_1->{DBH}->quote(" can't contain IP\n");

$dbh_1->insert("debug_log","info_str",$info);
 
my @res =get_id_firstRowCow($dbh_1->select("id", "printers","ip='192.168.11.151' and printer_name='PDF'"));
if ($res[0]){
	print $res[1],"\n";
}
$dbh_1->disconnect();

#print "Add new record (MySQL)\n";
#insert_record $dbh_1, "blah blah blah";


