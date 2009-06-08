--DROP FUNCTION reports_set_recivers (integer,character varying);
CREATE OR REPLACE FUNCTION reports_set_recivers (integer,character varying)
  RETURNS INTEGER AS
$BODY$
    DECLARE
     find_id INTEGER;
     receve_id INTEGER;	
     rel_id INTEGER;
     rep_id     ALIAS for $1;
     field_val 	ALIAS for $2;
    
    BEGIN
	find_id = (SELECT id FROM reports WHERE id = rep_id);
	IF (find_id IS NULL) THEN
		INSERT INTO debug_log (inf_str) VALUES ('Ошибка вставки записи. В таблице reports не существует записи с указывающие на  таблицу cups_data_log запись N='||rep_id);
		RETURN 0;
	END IF;

	-- Ищем значение recivers
	receve_id = (SELECT id FROM list_recivers WHERE adress = field_val);
	
	IF (receve_id IS NULL) THEN
		INSERT INTO  list_recivers (adress) VALUES (field_val);
		receve_id=(SELECT id FROM list_recivers WHERE adress = field_val);
	END IF;
	
      BEGIN  
	rel_id = (SELECT list_recivers_id FROM rel_list_recivers2reports WHERE list_recivers_id=receve_id and reports_id=find_id);
	IF (rel_id IS NULL ) THEN
		INSERT INTO rel_list_recivers2reports (list_recivers_id,reports_id) VALUES (receve_id,find_id);
	END IF;
		
	RETURN 1;
	EXCEPTION WHEN unique_violation THEN	-- do nothing, and loop to try the UPDATE again
	 RETURN 0;
	END;
    END;
$BODY$
  LANGUAGE 'plpgsql' VOLATILE
  COST 100;
ALTER FUNCTION reports_set_recivers (integer,character varying) OWNER TO postgres;
