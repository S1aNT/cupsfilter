--DROP FUNCTION reports_set_exec_print_fio(integer,character varying,character varying);
CREATE OR REPLACE FUNCTION reports_set_exec_print_fio(integer,character varying,character varying,character varying)
  RETURNS INTEGER AS
$BODY$
    DECLARE
     find_id INTEGER;
     exec_id INTEGER;
     rep_id     ALIAS for $1;
     user_fio 	ALIAS for $2;
     user_type  ALIAS for $3;
     phone	ALIAS for $4;
    BEGIN                                                                                                 
    
	find_id = (SELECT id FROM reports WHERE id = rep_id);
	IF (find_id IS NULL) THEN
		INSERT INTO debug_log (inf_str) VALUES ('������ ������� ������. � ������� reports �� ���������� ������ � ����������� ��  ������� cups_data_log ������ N='||rep_id);
		RETURN 0;
	END IF;
     BEGIN	
	IF (user_type='executor') THEN	
		-- set_executor_fio
		exec_id = (SELECT id FROM executors WHERE fio = user_fio and telephone =phone);
		IF (exec_id IS NULL) THEN
			INSERT INTO executors (fio,telephone) VALUES (user_fio,phone);
			exec_id = (SELECT id FROM executors WHERE fio = user_fio and telephone = phone);
		END IF;	
		UPDATE reports SET executor_id = exec_id WHERE reports.id = find_id;
	END IF;
	
	IF (user_type='pressman') THEN
		-- set_executor_fio
		exec_id = (SELECT id FROM pressman WHERE fio = user_fio);
		IF (exec_id IS NULL) THEN
			INSERT INTO pressman (fio) VALUES (user_fio);
			exec_id = (SELECT id FROM pressman WHERE fio = user_fio);
		END IF;	
		UPDATE reports SET pressman_id = exec_id WHERE reports.id = find_id;	
	END IF;
	RETURN 1;
     EXCEPTION WHEN unique_violation THEN
	RETURN 0; 		-- do nothing, and loop to try the UPDATE again
     END;
    END;
$BODY$
  LANGUAGE 'plpgsql' VOLATILE
  COST 100;
ALTER FUNCTION reports_set_exec_print_fio(integer,character varying,character varying,character varying) OWNER TO postgres;
