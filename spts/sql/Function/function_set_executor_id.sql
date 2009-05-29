-- Function: set_executor_id(character varying, character varying, character varying, character varying)

-- DROP FUNCTION set_executor_id(character varying, character varying, character varying, character varying);

CREATE OR REPLACE FUNCTION set_executor_id(last_n character varying, firts_n character varying, middle_n character varying, tlp character varying)
  RETURNS integer AS
$BODY$
    DECLARE
        user_id INTEGER;
    BEGIN
    user_id=(SELECT id FROM executors WHERE last_name=$1 and first_name=$2 and middle_name =$3);
    IF user_id IS NULL THEN
	-- вставим в таблицу данные
	INSERT INTO executors (last_name,first_name,middle_name,telephone) VALUES ($1,$2,$3,$4);
	user_id=(SELECT id FROM executors WHERE last_name=$1 and first_name=$2 and middle_name =$3);
    END IF;
    RETURN user_id;
    
   END;$BODY$
  LANGUAGE 'plpgsql' VOLATILE
  COST 100;
ALTER FUNCTION set_executor_id(character varying, character varying, character varying, character varying) OWNER TO postgres;