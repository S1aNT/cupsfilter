DROP FUNCTION set_reports_short_info(
	-- begin standart CUPS data
		inet, 		   -- printer_ip 	1
		character varying, -- printer_name 	2
		character varying, -- mandat		3
		character varying, -- cups_user		4
		character varying, -- jobTitle		5
		integer,	   -- page_copy 	6
		character varying, -- status		7
		character varying -- info_str		8
		);
		
--STATUS,printer_ip,printer_name,MANDAT,user,jobTitle,info_str,
CREATE OR REPLACE FUNCTION set_reports_short_info(
	-- begin standart CUPS data
		inet, 		   -- printer_ip 	1
		character varying, -- printer_name 	2
		character varying, -- mandat		3
		character varying, -- cups_user		4
		character varying, -- jobTitle		5
		integer,	   -- page_copy 	6
		character varying, -- status		7
		character varying -- info_str		8
		)
	--end standart CUPS data
  RETURNS integer AS
$BODY$
    DECLARE
	prn_id INTEGER;
	rep_id INTEGER;
	-- ќбъ€вление псевдонимов дл€ двух аргументов функции,
	ip 	  ALIAS for $1;
	p_name    ALIAS for $2;
	mandat    ALIAS for $3;
	cups_name ALIAS for $4;
	jobTitle  ALIAS for $5;
	pageCopy  ALIAS for $6;
	stat 	  ALIAS for $7;
	i_str	  ALIAS for $8;
			
    BEGIN
	prn_id :=set_printer_id(p_name,ip,mandat); 
	rep_id := (SELECT id FROM reports WHERE printer_id=prn_id and doc_name=jobTitle and page_copy=pageCopy and cups_user=cups_name);
	
	IF rep_id IS NULL THEN
		INSERT INTO reports (printer_id,doc_name,page_copy,cups_user,status,info_str) 
			     values (prn_id,JobTitle,pageCopy,cups_name,stat,i_str);
		rep_id := (SELECT id FROM reports WHERE printer_id=prn_id and doc_name=jobTitle and page_copy=pageCopy and cups_user=cups_name);	
	END IF;
	
	RETURN rep_id;

    END;
$BODY$
  LANGUAGE 'plpgsql' VOLATILE
  COST 100;
ALTER FUNCTION  set_reports_short_info(
	-- begin standart CUPS data
		inet, 		   -- printer_ip 	1
		character varying, -- printer_name 	2
		character varying, -- mandat		3
		character varying, -- cups_user		4
		character varying, -- jobTitle		5
		integer,	   -- page_copy 	6
		character varying, -- status		7
		character varying -- info_str		8
		)
	--end standart CUPS data
	OWNER TO postgres;