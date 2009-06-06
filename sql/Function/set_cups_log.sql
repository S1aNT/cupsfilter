/*
DROP FUNCTION set_cups_log(
		character varying, -- printer_name 	1
		character varying, -- cups_user		2
		character varying, -- jobTitle		3
		integer,	   -- page_copy 	4
		character varying, -- status		5
		character varying, -- info_str		6
		integer		   -- jobsID 		7
		);
		
*/
CREATE OR REPLACE FUNCTION set_cups_log(
		character varying, -- printer_name 	1
		character varying, -- cups_user		2
		character varying, -- jobTitle		3
		integer,	   -- page_copy 	4
		character varying, -- status		5
		character varying,  -- info_str		6
		integer		   -- jobsID 		7
		)
	
  RETURNS integer AS
$BODY$
    DECLARE
	rep_id INTEGER;
	-- Объявление псевдонимов для  аргументов функции,
	p_name    ALIAS for $1;
	c_user ALIAS for $2;
	j_title  ALIAS for $3;
	pageCopy  ALIAS for $4;
	stat 	  ALIAS for $5;
	i_str	  ALIAS for $6;
	jid	  ALIAS for $7;
			
    BEGIN
	
	rep_id := (SELECT id FROM cups_data_log WHERE printer=p_name and user_name=c_user and copyes=pageCopy and jobtitle=j_title and job=jid );
	
	IF (rep_id IS NULL )THEN
		INSERT INTO cups_data_log (job,printer,user_name,jobtitle,copyes,status,info_str) 
			     values (jid,p_name,c_user,j_title,pageCopy,stat,i_str);
		rep_id := (SELECT id FROM cups_data_log WHERE job=jid and printer=p_name and user_name=c_user and copyes=pageCopy and jobtitle=j_title);	
	END IF;
	
	RETURN rep_id;

    END;
$BODY$
  LANGUAGE 'plpgsql' VOLATILE
  COST 100;
ALTER FUNCTION  set_cups_log(
		character varying, -- printer_name 	1
		character varying, -- cups_user		2
		character varying, -- jobTitle		3
		integer,	   -- page_copy 	4
		character varying, -- status		5
		character varying, -- info_str		6
		integer		   -- jobsID 		7	
		)
	
	OWNER TO postgres;