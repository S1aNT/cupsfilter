<?php
require_once('dbdata.php');

try {
    //читаем параметры
    $curPage      = $_POST['page'];
    $rowsPerPage  = $_POST['rows'];
    $sortingField = $_POST['sidx'];
    $sortingOrder = $_POST['sord'];

    $printer_id	  = $_POST['printer_name'] !='all' ? $_POST['printer_name'] : $printer_id  = NULL;
    $executor_id  = $_POST['executor_fio'] !='all' ? $_POST['executor_fio'] : $executor_id = NULL;
    $pressman_id  = $_POST['pressman_fio'] !='all' ? $_POST['pressman_fio'] : $pressman_id = NULL;
    $marker_id    = $_POST['docmarker'] !='all' ? $_POST['docmarker'] : $marker_id = NULL;
    $cups_user	  = $_POST['cupsuser'] !='all' ? $_POST['cupsuser'] : $cups_user = NULL;
    #$date	  = $_POST['datetime'] !='all' ? $_POST['datetime'] : $date = NULL;	
    #$docname	  = $_POST['docname'] !='all' ? $_POST['docname'] : $docname = NULL;	

    $mb_number    = $_POST['mb_number'];
    $inv_number   = $_POST['inv_number'];
    $punkt        = $_POST['punkt'];
    $copynumber   = $_POST['copynumber'];
    $pagecount    = $_POST['pagecount'];
    $pagecopy     = $_POST['pagecopy'];  


    //toLog($docname);

    //подключаемся к базе
    $dbh = new PDO('pgsql:host='.$dbHost.';dbname='.$dbName, $dbUser, $dbPass);

    //указываем, мы хотим использовать utf8
    $dbh->exec('SET CHARACTER SET utf8');

    //определяем количество записей в таблице
    $rows = $dbh->query('SELECT COUNT(reports.id) AS count  FROM reports');
    $totalRows = $rows->fetch(PDO::FETCH_ASSOC);

    $firstRowIndex = $curPage * $rowsPerPage - $rowsPerPage;
    $where_str='';
    $query_str="SELECT 	reports.id,printers.printer_name, 
			       	reports.copy_number AS copynumber, document_level.marker AS docmarker,
			 	reports.doc_name AS docname, reports.dt AS datetime, reports.mb_number, 
				reports.page_count AS pagecount, reports.page_copy AS pagecopy, 
			 	reports.cups_user AS cupsuser, executors.fio AS executor_fio,
				pressman.fio AS pressman_fio, reports.inv_number AS invnumber, 
				reports.punkt, reports.jobs_id AS jobsid, reports.status, 
				reports.info_str AS infostr
			FROM reports
				JOIN printers ON reports.printer_id = printers.id
   				JOIN document_level ON reports.doc_level_id = document_level.id
				JOIN executors ON reports.executor_id = executors.id
				JOIN firstpages ON reports.firstpages_id = firstpages.id
				JOIN pressman ON reports.pressman_id = pressman.id";
    
    if (IsSet($printer_id)){

        if (!$where_str){
	   $where_str=' WHERE ';
	}else{
           $where_str.=' AND ';
	}
	$where_str.= " printers.id = '$printer_id'";
    }
    if (IsSet($marker_id)){

        if (!$where_str){
	   $where_str=' WHERE ';
	}else{
           $where_str.=' AND ';
	}
	$where_str.= " document_level.id = '$marker_id'";
    }
    if (IsSet($cups_user)){

        if (!$where_str){
	   $where_str=' WHERE ';
	}else{
           $where_str.=' AND ';
	}
	$where_str.= " reports.cups_user = '$cups_user'";
    }
    if (IsSet($executor_id)){

        if (!$where_str){
	   $where_str=' WHERE ';
	}else{
           $where_str.=' AND ';
	}
	$where_str.= " reports.executor_id= '$executor_id'";
    }
    if (IsSet($pressman_id)){

        if (!$where_str){
	   $where_str=' WHERE ';
	}else{
           $where_str.=' AND ';
	}
	$where_str.= " reports.pressman_id = '$pressman_id'";
    }
    if (IsSet($date)){

        if (!$where_str){
	   $where_str=' WHERE ';
	}else{
           $where_str.=' AND ';
	}
	#С 0 часов запрашиваемых суток
	$where_str.= " reports.datetime >= cast ('$date' as timestamp with time zone)'";
    }

    if (IsSet($docname)){
        if (!$where_str){
	   $where_str=' WHERE ';
	}else{
           $where_str.=' AND ';
	}
	#С 0 часов запрашиваемых суток
	$where_str.= " reports.doc_name LIKE '%$docname%'";
    }


    if (IsSet($mb_number)){
        if (!$where_str){
	   $where_str=' WHERE ';
	}else{
           $where_str.=' AND ';
	}
	$where_str.= " reports.mb_number= '$mb_number'";
    }
    if (IsSet($inv_number)){
        if (!$where_str){
	   $where_str=' WHERE ';
	}else{
           $where_str.=' AND ';
	}
	$where_str.= " reports.inv_number= '$inv_number'";
    }


    if (IsSet($punkt)){
        if (!$where_str){
	   $where_str=' WHERE ';
	}else{
           $where_str.=' AND ';
	}
	$where_str.= " reports.punkt= '$punkt'";
    }
    if (IsSet($copynumber)){
        if (!$where_str){
	   $where_str=' WHERE ';
	}else{
           $where_str.=' AND ';
	}
	$where_str.= " reports.copynumber= '$copynumber'";
    }
    if (IsSet($page_count)){
        if (!$where_str){
	   $where_str=' WHERE ';
	}else{
           $where_str.=' AND ';
	}
	$where_str.= " reports.page_count= '$pagecount'";
    }
    if (IsSet($page_copy)){
        if (!$where_str){
	   $where_str=' WHERE ';
	}else{
           $where_str.=' AND ';
	}
	$where_str.= " reports.page_copy= '$pagecopy'";
    }

    if ($where_str){
        $query_str.=$where_str;
    }


    $query_str.=" ORDER BY $sortingField $sortingOrder LIMIT $rowsPerPage OFFSET $firstRowIndex";
    toLog($query_str); 
    $res = $dbh->query($query_str);
//WHERE repots_dt BETWEEN '.$date_begin.' AND '.$date_end.'    
    //сохраняем номер текущей страницы, общее количество страниц и общее количество записей
    $response->page = $curPage;
    $response->total = ceil($totalRows['count'] / $rowsPerPage);
    $response->records = $totalRows['count'];

    $i=0;
    while($row = $res->fetch(PDO::FETCH_ASSOC)) {
        $response->rows[$i]['id']=$row['id'];
        $response->rows[$i]['cell']=array($row['id'],$row['datetime'],$row['printer_name'],
					  $row['docmarker'],$row['docname'],
					  $row['mb_number'],$row['inv_number'],$row['punkt'],
					  $row['copynumber'],$row['pagecount'],$row['pagecopy'],
					  $row['cupsuser'],$row['executor_fio'],$row['pressman_fio'],
					  $row['status'],$row['infostr']);
        $i++;
    }
    echo json_encode($response);
}
catch (PDOException $e) {
    echo 'Database error: '.$e->getMessage();
}

// end of getdata.php
?>
