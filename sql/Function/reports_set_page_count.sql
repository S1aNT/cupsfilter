--DROP FUNCTION reports_set_page_count(integer,integer);
CREATE OR REPLACE FUNCTION reports_set_page_count (integer,integer)
  RETURNS INTEGER AS
$BODY$
    DECLARE
     find_id INTEGER;
     rep_id     ALIAS for $1;
     field_val 	ALIAS for $2;
    
    BEGIN
	-- full check 
	find_id = (SELECT id FROM reports WHERE id = rep_id);
	IF (find_id IS NULL) THEN
		INSERT INTO debug_log (inf_str) VALUES ('������ ������� ������. � ������� reports �� ���������� ������ � ����������� ��  ������� cups_data_log ������ N='||rep_id);
		RETURN 0;
	END IF;
	-- ���� �������� punkt
	UPDATE  reports SET page_count = field_val WHERE reports.id = find_id;
	
	RETURN 1;
    END;
$BODY$
  LANGUAGE 'plpgsql' VOLATILE
  COST 100;
ALTER FUNCTION reports_set_page_count (integer,integer) OWNER TO postgres;
