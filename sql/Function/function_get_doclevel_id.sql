--DROP FUNCTION get_firstpages_id(character varying);

CREATE OR REPLACE FUNCTION get_doclevel_id(character varying)
  RETURNS integer AS
$BODY$
    DECLARE 
	dl_id INTEGER; -- ID записи содержащий уровень(гриф) документа если такого нет то создаем стандартный гриф документа НУ
	undef_id INTEGER;
    BEGIN
	dl_id :=(select id from document_level where marker=$1); 
	IF dl_id IS NULL THEN	
		INSERT INTO debug_log(info_str) values ('Не верный гриф документа ' ||$1);
		undef_id := (select id from document_level where marker='НУ'); 
		IF undef_id ISNULL THEN
			-- вставим в таблицу данные
			INSERT INTO debug_log(info_str) values ('Автоматическое добавление грифа документа НУ');
			INSERT INTO document_level (marker) VALUES ('НУ');
			undef_id := (select id from document_level where marker='НУ'); 
		END IF;
		RETURN undef_id; 
	END IF;
	RETURN dl_id;
    END;
$BODY$
  LANGUAGE 'plpgsql' VOLATILE
  COST 100;
ALTER FUNCTION get_doclevel_id(character varying) OWNER TO postgres;


	
