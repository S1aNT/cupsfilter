--DROP FUNCTION get_firstpages_id(character varying);

CREATE OR REPLACE FUNCTION get_firstpages_id(character varying)
  RETURNS integer AS
$BODY$
    DECLARE 
	fp_id INTEGER; -- ID записи о первой странице в Pdf
    BEGIN
	fp_id :=(select id from firstpages where fp_filename=$1); 
	IF fp_id IS NULL THEN	
		INSERT INTO firstpages (fp_filename) VALUES ($1);
		RETURN (select id from firstpages where fp_filename=$1); 
	END IF;
	RETURN fp_id;
    END;
$BODY$
  LANGUAGE 'plpgsql' VOLATILE
  COST 100;
ALTER FUNCTION get_firstpages_id(character varying) OWNER TO postgres;


	
