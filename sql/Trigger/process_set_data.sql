--DROP FUNCTION process_set_data() 

CREATE OR REPLACE FUNCTION process_set_data() 
RETURNS TRIGGER AS $get_id$
DECLARE
    pr_id INTEGER;
    fp_id INTEGER;
    BEGIN
         IF (TG_OP = 'INSERT') THEN
	    pr_id :=(select printers.id from printers where printers.printer_name=NEW.printer);
	    if  (pr_id IS NULL)THEN
		-- в моей базе нет такого принтера, но так как на уровень выше была проверка, то запишем в таблицу принтеров и вернем ID
		INSERT INTO printers (printer_name,descr) values (NEW.printer,'Принтер был добавлен не через интерфейс CUPS');
		pr_id :=(select printers.id from printers where printers.printer_name=NEW.printer);
	    END IF;
	    
            INSERT INTO reports (printer_id,cups_user,doc_name,page_copy,cups_data_log_id,jobs_id) 
		VALUES  (pr_id,NEW.user_name,NEW.jobtitle,NEW.copyes,NEW.id,NEW.job);
            RETURN NEW;
        END IF;
        RETURN NULL; -- result is ignored since this is an AFTER trigger
    END;
$get_id$ LANGUAGE plpgsql;

