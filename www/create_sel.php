<?php
require_once('dbdata.php');

try {
    //читаем параметры
    $sel_field = $_GET['field'];
    //$sel_field ='printer_name';
    //подключаемся к базе
    $dbh = new PDO('pgsql:host='.$dbHost.';dbname='.$dbName, $dbUser, $dbPass);
    //указываем, мы хотим использовать utf8
    $dbh->exec('SET CHARACTER SET utf8');
    $query_str='';
    switch ($sel_field){
	case 'printer':
	     $query_str='SELECT id as sel_id,printer_name as sel_name FROM printers';
		break;
	case 'marker':
	     $query_str='SELECT id as sel_id,marker as sel_name FROM document_level';
		break;
	case 'cupsuser':
	     $query_str='SELECT DISTINCT cups_user as sel_id,cups_user as sel_name FROM reports';
		break;
	case 'executor':
	     $query_str='SELECT id as sel_id,fio as sel_name FROM executors';
		break;
	case 'pressman':
	     $query_str='SELECT id as sel_id,fio as sel_name FROM pressman';
		break;

    }
    $res = $dbh->query($query_str);
    echo "<select>";
    echo "<option value='all'>Все</option>";
    while($row = $res->fetch(PDO::FETCH_ASSOC)) {
	echo "<option value='".$row['sel_id']."'>".$row['sel_name']."</option>";
    }
    echo "</select>";
    
}
catch (PDOException $e) {
    echo 'Database error: '.$e->getMessage();
}

?>
