--DROP FUNCTION get_reports_id(inet,character varying,character varying,character varying,character varying,integer);

--STATUS,printer_ip,printer_name,MANDAT,user,jobTitle,info_str,
CREATE OR REPLACE FUNCTION get_reports_id(
	-- begin standart CUPS data
		inet, 		   -- printer_ip 
		character varying, -- printer_name 
		character varying, -- mandat
		character varying, -- cups_user
		character varying, -- jobTitle
		integer		   -- page_copy 
		)
	--end standart CUPS data
  RETURNS integer AS
$BODY$
    BEGIN
	RETURN (SELECT id FROM reports WHERE printer_id=get_printer_id($2,$1,$3) and doc_name=$5 and page_copy=$6 and cups_user=$4);

    END;
$BODY$
  LANGUAGE 'plpgsql' VOLATILE
  COST 100;
ALTER FUNCTION get_reports_id(inet,character varying,character varying,character varying,character varying,integer) OWNER TO postgres;

--insert into reports (printer_id,doc_name,page_copy,cups_user) values (get_printer_id($3,$2,$4),get_firstpages_id($8),$7);