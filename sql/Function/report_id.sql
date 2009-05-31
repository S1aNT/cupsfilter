CREATE OR REPLACE FUNCTION get_reports_id (
	-- begin standart CUPS data
		inet, 		   -- printer_ip 
		character varying, -- printer_name 
		character varying, -- user_fio 
		integer,	   -- page_count 
	--end standart CUPS data
		integer,	   -- page_copy 
		character varying, -- printer_mandat 
		character varying, -- doc_name 
		integer,	   -- mb_number 
		character varying, -- firstpages_path 
		character varying, -- doc_level
		character varying, -- executor_first 
		character varying, -- executor_middle 
		character varying, -- executor_last 
		character varying, -- executor_phone 
		character varying  -- descr 
	)
  RETURNS ineteger AS
$BODY$
    DECLARE
        printer_id INTEGER;
    BEGIN
        printer_id := (select printers.id 
			from printers 
				INNER JOIN printers_mandat ON (printers.mandat_id = printers_mandat.id) 
				INNER JOIN rel_mandat2doc_level ON ( rel_mandat2doc_level.mandat_id =printers_mandat.id) 
				INNER JOIN document_level ON (document_level.id = rel_mandat2doc_level.doclevel_id)
			where 
				printers.ip_adress=$1 and 	
				printers.printer_name=$1 and 
				
				printers_mandat.mandat=$3 and 
				document_level.marker=$4
			); 
        IF printer_id ISNULL THEN
            RETURN 0;
        END IF;
        RETURN 1;
    END;
$BODY$
  LANGUAGE 'plpgsql' VOLATILE
  COST 100;
ALTER FUNCTION get_reports_id (character varying, inet, character varying, character varying) OWNER TO postgres;

--select check_printer_perm('PDF','192.168.11.200','SYSPROG','ÑÑ');