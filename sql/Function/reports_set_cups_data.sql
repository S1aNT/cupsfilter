-- DROP FUNCTION reports_set_cups_data(character varying, character varying, character varying, integer, character varying, character varying, integer);

CREATE OR REPLACE FUNCTION reports_set_cups_data(character varying, character varying, character varying, integer, character varying, character varying, integer)
  RETURNS integer AS
$BODY$
    DECLARE
	rep_id INTEGER;
	-- Объявление псевдонимов для  аргументов функции,
	p_name    ALIAS for $1;
	c_user ALIAS for $2;
	j_title  ALIAS for $3;
	p_copy  ALIAS for $4;
	stat 	  ALIAS for $5;
	i_str	  ALIAS for $6;
	jid	  ALIAS for $7;
			
    BEGIN
	rep_id := (SELECT id FROM reports 
			WHERE cups_printer_name=p_name and cups_user=c_user and page_copy=p_copy 
				and doc_name=j_title and jobs_id=jid );
	
	IF (rep_id IS NULL )THEN
		INSERT INTO reports (jobs_id,cups_printer_name,cups_user,doc_name,page_copy,status,info_str) 
			     values (jid,p_name,c_user,j_title,p_copy,stat,i_str);
		rep_id := (SELECT id FROM reports 
				WHERE cups_printer_name=p_name and cups_user=c_user and page_copy=p_copy 
					and doc_name=j_title and jobs_id=jid );
		
	END IF;
	RETURN rep_id;
    END;
$BODY$
  LANGUAGE 'plpgsql' VOLATILE
  COST 100;
ALTER FUNCTION reports_set_cups_data(character varying, character varying, character varying, integer, character varying, character varying, integer) OWNER TO postgres;
