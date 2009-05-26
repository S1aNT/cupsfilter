drop function get_user_id(IN last_n character varying, 
			    IN firts_n character varying, 
			    IN middle_n character varying, 
			    IN tlp character varying);
			    
CREATE FUNCTION get_user_id(IN last_n character varying, 
			    IN firts_n character varying, 
			    IN middle_n character varying, 
			    IN tlp character varying) 
RETURNS INTEGER AS'
    DECLARE
        user_id INTEGER;
    BEGIN
    
    user_id=(SELECT id FROM users WHERE last_name=$1 and first_name=$2 and middle_name =$3);
    IF NOT FOUND THEN
	-- вставим в таблицу данные
	INSERT INTO users (last_name,first_name,middle_name,telephone) VALUES ($1,$2,$3,$4);
	user_id=(SELECT id FROM users WHERE last_name=$1 and first_name=$2 and middle_name =$3);
    END IF;
    RETURN user_id;
    
   END;' 
   LANGUAGE 'plpgsql';

ALTER FUNCTION check_printer_perm(IN character varying, IN inet,IN character varying,IN character varying) OWNER TO postgres;
