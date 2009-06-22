<?php
require_once('dbdata.php');

try {
    //читаем параметры
    $rec_id = $_GET['id'];
    $rowsPerPage = $_GET['rows'];
    $sortingField = $_GET['sidx'];
    $sortingOrder = $_GET['sord'];
    //читаем параметры


    //подключаемся к базе
    $dbh = new PDO('pgsql:host='.$dbHost.';dbname='.$dbName, $dbUser, $dbPass);

    //указываем, мы хотим использовать utf8
    $dbh->exec('SET CHARACTER SET utf8');
    //определяем количество записей в таблице
    $rows = $dbh->query('SELECT count (reports_id) as count
				FROM list_recivers
				JOIN rel_list_recivers2reports ON rel_list_recivers2reports.list_recivers_id = list_recivers.id
			 WHERE reports_id ='.$rec_id);

    $totalRows = $rows->fetch(PDO::FETCH_ASSOC);
    $response->records = $totalRows['count'];

    $res = $dbh->query('SELECT reports_id as rep_id,list_recivers.id as list_id,adress
				FROm list_recivers JOIN rel_list_recivers2reports ON rel_list_recivers2reports.list_recivers_id = list_recivers.id
			WHERE reports_id ='.$rec_id.' ORDER BY '.$sortingField.' '.$sortingOrder);
    
    $i=0;
    while($row = $res->fetch(PDO::FETCH_ASSOC)) {
        $response->rows[$i]['id']=$row['id'];
        $response->rows[$i]['cell']=array($row['rep_id'],$row['list_id'],$row['adress']);
        $i++;
    }
    echo json_encode($response);
}
catch (PDOException $e) {
    echo 'Database error: '.$e->getMessage();
}

// end of getdata.php
?>
