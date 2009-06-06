--DROP FUNCTION reports_set_fp_pdf_name(integer,character varying);
CREATE OR REPLACE FUNCTION reports_set_fp_pdf_name (integer,character varying)
  RETURNS INTEGER AS
$BODY$
    DECLARE
     find_id INTEGER;
     fpage_id INTEGER;
     rep_id     ALIAS for $1;
     field_val 	ALIAS for $2;
    
    BEGIN
	-- full check 
	find_id = (SELECT id FROM reports WHERE cups_data_log_id = rep_id);
	IF (find_id IS NULL) THEN
		INSERT INTO debug_log (inf_str) VALUES ('Ошибка вставки записи. В таблице reports не существует записи с указывающие на  таблицу cups_data_log запись N='||rep_id);
		RETURN 0;
	END IF;
	
	-- Ищем значение fp_filename
	fpage_id = (SELECT id FROM firstpages WHERE fp_filename=field_val);
	
	IF (fpage_id IS NULL) THEN
		INSERT INTO  firstpages (fp_filename) VALUES (field_val);
		fpage_id = (SELECT id FROM firstpages WHERE fp_filename=field_val);
	END IF;

	UPDATE  reports SET firstpages_id = fpage_id WHERE reports.id = find_id;
	
	RETURN 1;
    END;
$BODY$
  LANGUAGE 'plpgsql' VOLATILE
  COST 100;
ALTER FUNCTION reports_set_fp_pdf_name (integer,character varying) OWNER TO postgres;
