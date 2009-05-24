#!/usr/bin/perl
#Функция fetchrow_array хороша для тех случаев, когда нужно вывести содержимое таблицы, 
#у которой неизвестна структура (количество полей и их названия). В других случаях можно использовать другие функции. 
#Допустим, что мы хотим получить из таблицы pg_tables не все поля, а только два tablename и tableowner. 
#В принципе это можно сделать слегка подправив предыдущий пример, но в этом случае, помоему, 
#будет удобней воспользоваться функцией fetchrow_hashref. Смотрите пример:

use DBI;

$dbh = DBI->connect("dbi:Pg:dbname=template1","postgres","",{PrintError => 0});

if ($DBI::err != 0) {
	print $DBI::errstr . "\n";
	exit($DBI::err);
}

$query = "SELECT * FROM pg_tables";
$sth = $dbh->prepare($query);
$rv = $sth->execute();
if (!defined $rv) {
	print "При выполнении запроса '$query' возникла ошибка: " . $dbh->errstr . "\n";
	exit 1;
}

while ($ref = $sth->fetchrow_hashref()) {
	($tablename, $tableowner) = ($ref->{'tablename'}, $ref->{'tableowner'});
	print "$tablename\t$tableowner\n";
}

$sth->finish();
$dbh->disconnect();

#Снова пояснения. Как видите, после выполнения fetchrow_hashref мы получаем
#хэш, который содержит в качестве ключей - имена полей в тех записях,
#которые нам вернул запрос, а в качестве данных - значения этих полей для
#одной записи. Все записи как и в предыдущем примере перебираются в цикле
#while, который заканчивается, когда функция fetchrow_hashref вовзращает
#пустое значение undef.
#Вот собственно наиболее популярные функции для работы с запросами вида
#SELECT. Конечно, этим возможности DBI не исчерпываются. Есть, например, ещё
#функции selectrow_array, selectall_arrayref, selectcol_arrayref и
#соответственно fetchrow_arrayref, fetchall_arrayref. В каких-то случаях,
#наверное, будет удобней воспользоваться одной из них. Тем не менее я не
#стану их описывать и отправляю желающих изучить к руководству man по DBI.

