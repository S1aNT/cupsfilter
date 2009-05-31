
--DROP FUNCTION set_mandat_id(character varying);
CREATE OR REPLACE FUNCTION set_mandat_id(character varying)
  RETURNS integer AS
$BODY$
    DECLARE
        mandat_id INTEGER;
        notfound_id INTEGER;
    BEGIN
    /*
	Получить id мандата если такого не существует, то по умолчанию выбирается мандат "UNDEF_MANDAT"
	 если его нет то вставить такой мандат в таблицу, записать сообщение в таблицу лога вернуть  id найденного мандата    
    */
	notfound_id := (select id from printers_mandat where printers_mandat.mandat='UNDEF_MANDAT'); 
	IF notfound_id ISNULL THEN
		-- вставим в таблицу данные
		INSERT INTO debug_log(info_str) values ('Автоматическое добавление мандата UNDEF_MANDAT');
		INSERT INTO printers_mandat (mandat) VALUES ('UNDEF_MANDAT');
		notfound_id := (select id from printers_mandat where printers_mandat.mandat='UNDEF_MANDAT'); 
	END IF;
	mandat_id := (select id from printers_mandat where printers_mandat.mandat=$1); 
        IF mandat_id ISNULL THEN
		-- вернем значение по умолчанию 
		return notfound_id;	
        END IF;
        RETURN mandat_id;
    END;
$BODY$
  LANGUAGE 'plpgsql' VOLATILE
  COST 100;
ALTER FUNCTION set_mandat_id(character varying) OWNER TO postgres;

