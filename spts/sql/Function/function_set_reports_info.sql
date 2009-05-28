--DROP FUNCTION set_reports_short_info(inet,character varying,character varying,character varying,character varying,integer,character varying,character varying);

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
	dt_marker timestamp with time zone;
    BEGIN
	dt_marker:= now();
	prn_id :=get_printer_id($2,$1,$3); -- Получим id Принтера {если принтера с такими параметрами в БД нет, то создадим новый, но мандат будет = 'NOT FOUND' }
	rep_id := (SELECT id FROM reports WHERE printer_id=prn_id and doc_name=$5 and page_copy=$6 and cups_user=$4 and DT=dt_marker);
	IF rep_id IS NULL THEN
		--INSERT INTO reports (printer_id,doc_name,page_copy,cups_user,status,info_str,DT) values (prn_id,$5,$6,$4,$7,$8,$9);
		--rep_id := (SELECT id FROM reports WHERE printer_id=prn_id and doc_name=$5 and page_copy=$6 and cups_user=$4 and DT=dt_marker);	
		
	END IF;
	RETURN rep_id;

    END;
$BODY$
  LANGUAGE 'plpgsql' VOLATILE
  COST 100;
ALTER FUNCTION set_reports_short_info(inet,character varying,character varying,character varying,character varying,integer,character varying,character varying) OWNER TO postgres;

--insert into reports (printer_id,doc_name,page_copy,cups_user) values (get_printer_id($3,$2,$4),get_firstpages_id($8),$7);