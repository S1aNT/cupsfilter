jQuery(document).ready(function(){
	var printersel='create_sel.php?field=printer';
        var markersel='create_sel.php?field=marker';
        var cups_sel='create_sel.php?field=cupsuser';
        var executorsel='create_sel.php?field=executor';
        var pressmansel='create_sel.php?field=pressman';
         
        jQuery("#tblcontents").jqGrid({
	url:'getdata.php',
	datatype: 'json',
	mtype: 'POST',
	height: 200,
        datatype: 'local', 
	colNames:['#','Дата','Принтер','Гриф','Название документа','№ МБ','Инв. номер','Пункт','№ копии','Кол-во стр.','Кол-во копий','CupsUser','Исполнитель','Отпечатал','Статус док-та','Примечание'], 
	colModel:[  
		{name:'id',index:'id', search:false, editable:false, width:35},  
		{name:'datetime',index:'datetime', formatter:'date',formatoptions:{srcformat: 'Y-m-d',newformat: 'd.m.Y'},width:80, align:'right'},  
		{name:'printer_name',index:'printer_name',search:true,edittype:'select',surl:printersel, width:90},  
		{name:'docmarker',index:'docmarker', edittype:'select',surl:markersel,width:90, align:'right'},  
		{name:'docname',index:'docname', width:250,align:'center',sortable:false},  
		{name:'mb_number',index:'mb_number', width:90, align:'right'},  
		{name:'inv_number',index:'inv_number', width:90, align:'right'},  
		{name:'punkt',index:'punkt', width:90, align:'right'},  
		{name:'copynumber',index:'copynamber', width:80, align:'right'},  
		{name:'pagecount',index:'pagecount', width:80, align:'right'},  
		{name:'pagecopy',index:'pagecopy', width:80, align:'right'},  
		{name:'cupsuser',index:'cupsuser',edittype:'select',surl:cups_sel, width:80, sortable:false}, 
		{name:'executor_fio',index:'executor_fio',edittype:'select',surl:executorsel,width:150, sortable:false},
		{name:'pressman_fio',index:'pressman_fio',edittype:'select',surl:pressmansel,width:150, sortable:false}, 
		{name:'status',index:'status', search:false,width:100, align:'right'},  
		{name:'infostr',index:'infostr',search:false, width:180, sortable:false}
	], 
	rowNum:10, 
	rowList:[10,20,30,50], 
	pager: jQuery('#tblcontentsPager'), 
	sortname: 'id', 
	viewrecords: true, 
        imgpath: 'themes/basic/images',
        caption: 'Параметры отчета сервера защищенной печати',
	sortorder: "asc",
	subGrid: true, 
	subGridRowExpanded: function(subgrid_id, row_id) { 
	    // we pass two parameters, subgrid_id is a id of the div tag created within a table, the row_id is the id of the row 
	    // If we want to pass additional parameters to the url we can use the method getRowData(row_id) - which returns associative array in type name-value here we can easy construct the following 
	    var subgrid_table_id; 
	      subgrid_table_id = subgrid_id+"_t"; 
	      jQuery("#"+subgrid_id).html("<table id='"+subgrid_table_id+"' class='scroll'></table>"); 
              
              jQuery("#"+subgrid_table_id).jqGrid({ 
	          url:"subgrid.php?id="+row_id, 
        	  datatype: "json", 
	          colNames: ['rep_id','list_id','Получатели'], 
	          colModel: [ 
        		{name:'rep_id',index:'rep_id',width:80,key:true}, 
	            	{name:'list_id',index:'list_id',width:80}, 
		        {name:'adress',index:'adress',width:200,align:'right',sortable:false} 
                        ], 
          	rowNum:20, 
        	imgpath: 'themes/basic/images',
          	sortname: 'rep_id', 
          	sortorder: "asc" 
       	     });
	}

       
	})

	jQuery("#srccontents").filterGrid("tblcontents",{ 
        	gridModel:true, 
	        gridNames:true,   
                searchButton:"Искать",
                clearButton:"Очистить",
        	formtype:"vertical", 
	        enableSearch: true, 
        	enableClear: true, 
	        autosearch: false
	      }); 
	jQuery("#tblcontents").setGridParam({datatype:'json'});  
      	var ts = $('#srccontents')[0];  
      	ts.triggerSearch();
	});

$(function() {
	$('#sg_datetime').datepicker({
		changeMonth: true,
		changeYear: true
	});
});
