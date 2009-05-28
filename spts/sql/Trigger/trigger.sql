

CREATE OR REPLACE FUNCTION process_get_id() RETURNS TRIGGER AS $get_id$
DECLARE
    pr_id INTEGER;
    fp_id INTEGER;
    BEGIN
/*
Для каждой вставленной записи в таблицу reports_info вызывается тригер, который
получает id записи из таблицы printers которой соответствует принтер с сопадающими name,ip,mandat
получает id записи из таблицы firtspages которой соответствует запись указывающая на полный путь к сохраненой первой страцнице
печатаемого документа т.е с сопадающим fp_filename
Если хотя бы один из этих параметров не определен, то запись в таблицу не вставиться
После получения этих параметров они совместно с оставшимися полями записи копируются в таблицу reports
*/    
        IF (TG_OP = 'INSERT') THEN
	    --insert into debug_log (info_str) values(NEW.printer_name);
	    pr_id :=(select printers.id 
			from printers INNER JOIN printers_mandat ON (printers.mandat_id = printers_mandat.id) 
			where printers.printer_name=NEW.printer_name and printers.ip_adress=NEW.printer_ip and printers_mandat.mandat=NEW.printer_mandat);
			
	    --insert into debug_log (info_str) values(pr_id);	
	    
	    fp_id := (select id from firstpages where fp_filename=NEW.firstpages_path); 		

	    --insert into debug_log (info_str) values(fp_id);	
	    	    
	    if  (pr_id IS NULL or fp_id IS NULL)THEN
		RETURN NULL;
	    END IF;
            INSERT INTO reports (printer_id,firstpages_id,user_fio,doc_name,mb_number,tag,page_count,page_copy,descr)
            
			VALUES  (pr_id,fp_id,NEW.user_fio,NEW.doc_name,NEW.mb_number,NEW.tag,NEW.page_count,NEW.page_copy,NEW.descr);
            RETURN NEW;
        END IF;
        RETURN NULL; -- result is ignored since this is an AFTER trigger
    END;
$get_id$ LANGUAGE plpgsql;


DROP TRIGGER get_id ON reports_info;

CREATE TRIGGER get_id
AFTER INSERT ON reports_info
    FOR EACH ROW EXECUTE PROCEDURE process_get_id();


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