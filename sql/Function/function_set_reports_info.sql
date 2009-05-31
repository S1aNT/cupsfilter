/*
DROP FUNCTION set_reports_info(
		inet, 		   -- printer_ip 	1
		character varying, -- printer_name 	2
		character varying, -- mandat		3
		character varying, -- cups_user		4
		character varying, -- jobTitle		5
		integer,	   -- page_copy 	6
		character varying, -- status		7
		character varying, -- info_str		8
		character varying, -- firstpages_path	9
		integer,	   -- copy number	10
		character varying, -- doc_level_name	11
		character varying, -- mb_number		12
		integer,	   -- page_count	13
		character varying, -- execut_last_name  14
		character varying, -- execut_first_name 15
		character varying, -- execut_middle_name 16
		character varying  -- execut_teephone	17
		);
*/		

CREATE OR REPLACE FUNCTION set_reports_info(
		inet, 		   -- printer_ip 	1
		character varying, -- printer_name 	2
		character varying, -- mandat		3
		character varying, -- cups_user		4
		character varying, -- jobTitle		5
		integer,	   -- page_copy 	6
		character varying, -- status		7
		character varying, -- info_str		8
		character varying, -- firstpages_path	9
		integer,	   -- copy number	10
		character varying, -- doc_level_name	11
		character varying, -- mb_number		12
		integer,	   -- page_count	13
		character varying, -- execut_last_name  14
		character varying, -- execut_first_name 15
		character varying, -- execut_middle_name 16
		character varying  -- execut_teephone	17
		)
	
  RETURNS integer AS
$BODY$
    DECLARE
	prn_id INTEGER;
	rep_id INTEGER;
	fp_id INTEGER;
	dl_id INTEGER;
	ex_id INTEGER;
	-- ќбъ€вление псевдонимов дл€ двух аргументов функции,
	ip 	  ALIAS for $1;
	p_name    ALIAS for $2;
	mandat    ALIAS for $3;
	cups_name ALIAS for $4;
	jobTitle  ALIAS for $5;
	pageCopy  ALIAS for $6;
	stat 	  ALIAS for $7;
	i_str	  ALIAS for $8;
	copy_Num  ALIAS for $10;
	mb_Num	  ALIAS for $12;
	page_Cnt  ALIAS for $13;
	execut_last_name  ALIAS for $14;
	execut_first_name ALIAS for $15;
	execut_middle_name ALIAS for $16;
	execut_teephone	ALIAS for $17;
			
    BEGIN
	prn_id :=set_printer_id(p_name,ip,mandat); 
	fp_id  :=get_firstpages_id($9);
	dl_id  :=get_doclevel_id($11);
	ex_id  :=set_executor_id(execut_last_name,execut_first_name,execut_middle_name,execut_teephone);
	rep_id := (SELECT id FROM reports
			WHERE printer_id=prn_id 
				and doc_name=jobTitle 
				and page_copy=pageCopy 
				and cups_user=cups_name 
				and firstpages_id =fp_id 
				and doc_level_id = dl_id 
				and executor_id =ex_id 
				and copy_number=Copy_num 
				and mb_number = mb_Num
				and page_count =page_Cnt
				and status=stat
		   );
	IF rep_id IS NULL THEN
		INSERT INTO reports (printer_id,doc_name,page_copy,cups_user,status,info_str) 
			     values (prn_id,JobTitle,pageCopy,cups_name,stat,i_str);
		rep_id := (SELECT id FROM reports
			WHERE printer_id=prn_id 
				and doc_name=jobTitle 
				and page_copy=pageCopy 
				and cups_user=cups_name 
				and firstpages_id =fp_id 
				and doc_level_id = dl_id 
				and executor_id =ex_id 
				and copy_number=Copy_num 
				and mb_number = mb_Num
				and page_count =page_Cnt
				and status=stat
		   );
	END IF;
	
	RETURN rep_id;

    END;
$BODY$
  LANGUAGE 'plpgsql' VOLATILE
  COST 100;
ALTER FUNCTION  set_reports_info(
	-- begin standart CUPS data
		inet, 		   -- printer_ip 	1
		character varying, -- printer_name 	2
		character varying, -- mandat		3
		character varying, -- cups_user		4
		character varying, -- jobTitle		5
		integer,	   -- page_copy 	6
		character varying, -- status		7
		character varying, -- info_str		8
		character varying, -- firstpages_path	9
		integer,	   -- copy number	10
		character varying, -- doc_level_name	11
		character varying, -- mb_number		12
		integer,	   -- page_count	13
		character varying, -- execut_last_name  14
		character varying, -- execut_first_name 15
		character varying, -- execut_middle_name 16
		character varying  -- execut_teephone	17
		)
	--end standart CUPS data
	OWNER TO postgres;