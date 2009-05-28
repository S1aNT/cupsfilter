--DROP FUNCTION get_firstpages_id(character varying);

CREATE OR REPLACE FUNCTION get_firstpages_id(character varying)
  RETURNS integer AS
$BODY$
    BEGIN
	RETURN (select id from firstpages where fp_filename=$1); 
    END;
$BODY$
  LANGUAGE 'plpgsql' VOLATILE
  COST 100;
ALTER FUNCTION get_firstpages_id(character varying) OWNER TO postgres;

