--DROP FUNCTION get_printer_id(character varying,inet,character varying);

CREATE OR REPLACE FUNCTION get_printer_id(character varying,inet,character varying)
  RETURNS integer AS
$BODY$
    BEGIN
	RETURN (select id from printers where printer_name=$1 and ip_adress=$2 and mandat_id=get_mandat_id($3)); 
    END;
$BODY$
  LANGUAGE 'plpgsql' VOLATILE
  COST 100;
ALTER FUNCTION get_printer_id(character varying,inet,character varying) OWNER TO postgres;

