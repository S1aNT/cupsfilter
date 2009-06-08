--DROP FUNCTION reports_set_copy_number(integer,integer);
CREATE OR REPLACE FUNCTION reports_set_copy_number(integer,integer)
  RETURNS INTEGER AS
$BODY$
    DECLARE
     find_id INTEGER;
     rep_id     ALIAS for $1;
     field_val 	ALIAS for $2;
    
    BEGIN
	find_id = (SELECT id FROM reports WHERE id = rep_id);
	IF (find_id IS NULL) THEN
		INSERT INTO debug_log (inf_str) VALUES ('Ошибка вставки записи. В таблице reports не существует записи с указывающие на  таблицу cups_data_log запись N='||rep_id);
		RETURN 0;
	END IF;
	
	-- Ищем значение copy number)
	UPDATE  reports SET copy_number = field_val WHERE reports.id = find_id;
	
	RETURN 1;
    END;
$BODY$
  LANGUAGE 'plpgsql' VOLATILE
  COST 100;
ALTER FUNCTION reports_set_copy_number(integer,integer) OWNER TO postgres;
