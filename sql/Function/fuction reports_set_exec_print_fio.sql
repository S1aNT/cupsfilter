--DROP FUNCTION reports_set_exec_print_fio(integer,character varying,character varying);
CREATE OR REPLACE FUNCTION reports_set_exec_print_fio(integer,character varying,character varying,character varying)
  RETURNS INTEGER AS
$BODY$
    DECLARE
     find_id INTEGER;
     exec_id INTEGER;
     rep_id     ALIAS for $1;
     user_type  ALIAS for $2;
     field_val 	ALIAS for $3;
     phone	ALIAS for $4;
    BEGIN                                                                                                 
    
	find_id = (SELECT id FROM reports WHERE cups_data_log_id = rep_id);
	IF (find_id IS NULL) THEN
		INSERT INTO debug_log (inf_str) VALUES ('Ошибка вставки записи. В таблице reports не существует записи с указывающие на  таблицу cups_data_log запись N='||rep_id);
		RETURN 0;
	END IF;
	-- set_executor_fio
	exec_id = (SELECT id FROM executors WHERE fio = field_val and user_role=user_type and telephone =phone);
	IF (exec_id IS NULL) THEN
		INSERT INTO executors (fio,user_role,telephone) VALUES (field_val,user_type,phone);
		exec_id = (SELECT id FROM executors WHERE fio = field_val and user_role=user_type and telephone = phone);
	END IF;
	BEGIN
		IF (user_type='executor') THEN
			UPDATE reports SET executor_id = exec_id WHERE reports.id = find_id;	
		END IF;
		IF (user_type='pressman') THEN
			UPDATE reports SET pressman_id = exec_id WHERE reports.id = find_id;	
		END IF;
		RETURN 1;
	EXCEPTION WHEN unique_violation THEN
		RETURN 0;
			-- do nothing, and loop to try the UPDATE again
	END;
	
    END;
$BODY$
  LANGUAGE 'plpgsql' VOLATILE
  COST 100;
ALTER FUNCTION reports_set_exec_print_fio(integer,character varying,character varying,character varying) OWNER TO postgres;
