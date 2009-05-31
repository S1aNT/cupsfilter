#!/usr/bin/perl
use DBI;

# имя базы данных
$dbname = "CupsLog";
# имя пользователя
$username = "postgres";
# пароль
$password = "pg";
# имя или IP адрес сервера
$dbhost = "localhost";
# порт
$dbport = "5432";
# опции
$dboptions = "-e";
# терминал
$dbtty = "ansi";


$dbh = DBI->connect("dbi:PgPP:dbname=$dbname;host=$dbhost;port=$dbport;options=$dboptions;tty=$dbtty","$username","$password",{PrintError => 0});

if ($DBI::err != 0) {
	print $DBI::errstr."\n";
	exit($DBI::err);
}

$dbh->disconnect();
