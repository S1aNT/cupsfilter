--DROP FUNCTION reports_set_add_info (integer,character varying,character varying);

CREATE OR REPLACE FUNCTION reports_set_add_info (integer,character varying,character varying)
  RETURNS INTEGER AS
$BODY$
    DECLARE
     find_id INTEGER;
     addinfo_id INTEGER;
     
     rep_id     ALIAS for $1;
     field_key 	ALIAS for $2;
     field_val 	ALIAS for $3;
    
    BEGIN
	find_id = (SELECT id FROM reports WHERE id = rep_id);
	IF (find_id IS NULL) THEN
		INSERT INTO debug_log (inf_str) VALUES ('Ошибка вставки записи. В таблице reports не существует записи с указывающие на  таблицу cups_data_log запись N='||rep_id);
		RETURN 0;
	END IF;
	
	-- Ищем значение recivers
	addinfo_id = (SELECT id FROM  add_info WHERE tag_key = field_key and tag_value = field_val);
	
	IF (addinfo_id IS NULL) THEN
		INSERT INTO  add_info (tag_key,tag_value) VALUES (field_key,field_val);
		addinfo_id = (SELECT id FROM  add_info WHERE tag_key = field_key and tag_value = field_val);
	END IF;
	
	-- Обновляем таблицу связей
	UPDATE rel_add_info2reports SET add_info_id=addinfo_id,reports_id=find_id;
 	IF (FOUND) THEN 
		RETURN 1;
	END IF;
	
	BEGIN
		INSERT INTO rel_add_info2reports (add_info_id,reports_id) VALUES (addinfo_id,find_id);
		RETURN 1;
	EXCEPTION WHEN unique_violation THEN	-- do nothing, and loop to try the UPDATE again
		RETURN 0;
	END;
    END;
$BODY$
  LANGUAGE 'plpgsql' VOLATILE
  COST 100;
ALTER FUNCTION reports_set_add_info (integer,character varying,character varying) OWNER TO postgres;
