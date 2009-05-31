
-------------------- Test group
/*
insert into firstpages (fp_filename) values ('d:/Как размножаются кролики.pdf');
insert into printers_mandat (mandat) values ('SYSTEM');
mandat_id := (select id from printers_mandat where mandat='SYSTEM');
insert into printers (printer_name,ip_adress,mandat_id) values ('PDF','192.168.11.200','2');


insert into reports_info (printer_name,printer_ip,printer_mandat,firstpages_path,user_fio,doc_name,mb_number,tag,page_count,page_copy)
values('PDF','192.168.11.200','SYSTEM','d:/Как размножаются кролики.pdf','Мосенцев','Как размножаются кролики.doc','мб104','СЕКРЕТНО',1,1);
select * from reports;

select id, (select id from printers_mandat where printers_mandat.id = printers.mandat_id) as mandat_id
from printers 

delete from reports_info;
insert into reports_info (printer_name,printer_ip,printer_mandat,firstpages_path,user_fio,doc_name,mb_number,tag,page_count,page_copy)
values('PDF','192.168.11.151','SYSPROG','d:/Как размножаются кролики.pdf','Мосенцев','Как размножаются кролики.doc','мб104','СЕКРЕТНО',1,1);
select * from reports_info;


select id, (select mandat from printers_mandat where printers_mandat.id = printers.mandat_id) as mandat
from printers 
--where printer_name=$1 and ip_adress=$2 
*/
--insert into printers_mandat (mandat) values('SYSPROG');
--delete from printers;
--select * from printers_mandat; --where (printers_mandat.mandat= 'SYSPROG'  OR printers_mandat.mandat='NOT FOUND')
--insert into printers (printer_name,ip_adress,mandat_id) values ('PDF','192.168.11.151',get_mandat_id('SYSPROG'));
--select * from printers;
--SELECT get_mandat_id('SYSPROG');

--delete from printers_mandat;
--select get_mandat_id ('фигня');
--delete from users
--select get_user_id('Мосенцев961','Павел','Владимирович','+7964xxxxxxx');
--select * from users  --where last_name='Мосенцев' and first_name='Павел' and middle_name ='Владимирович'
--select * from printers_mandat where (printers_mandat.mandat='1' or printers_mandat.mandat='NOT FOUND')
