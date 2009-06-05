--DROP FUNCTION reports_set_doc_level(integer,character varying);
CREATE OR REPLACE FUNCTION reports_set_doc_level(integer,character varying)
  RETURNS INTEGER AS
$BODY$
    DECLARE
     find_id INTEGER;
     doc_l_id INTEGER;
     rep_id     ALIAS for $1;
     field_val 	ALIAS for $2;
     
    BEGIN
	find_id = (SELECT id FROM reports WHERE cups_data_log_id = rep_id);
	IF (find_id IS NULL) THEN
		INSERT INTO debug_log (inf_str) VALUES ('Ошибка вставки записи. В таблице reports не существует записи с указывающие на  таблицу cups_data_log запись N='||rep_id);
		RETURN 0;
	END IF;
	
		-- Ищем значение грифа документа :)
		doc_l_id = (SELECT id FROM document_level WHERE marker = field_val);
		IF (doc_l_id IS NULL) THEN
			INSERT INTO document_level (marker) VALUES (field_val);
			doc_l_id=(SELECT id FROM document_level WHERE marker = field_val);
		END IF;
		
		BEGIN
			UPDATE reports SET doc_lev_id = doc_l_id WHERE reports.id = find_id;	
			RETURN 1;
		EXCEPTION WHEN unique_violation THEN
			RETURN 0;
			-- do nothing, and loop to try the UPDATE again
		END;
	
	
    END;
$BODY$
  LANGUAGE 'plpgsql' VOLATILE
  COST 100;
ALTER FUNCTION reports_set_doc_level(integer,character varying) OWNER TO postgres;
