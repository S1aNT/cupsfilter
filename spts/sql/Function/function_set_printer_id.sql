--DROP FUNCTION set_printer_id(character varying,inet,character varying);

CREATE OR REPLACE FUNCTION set_printer_id(
				character varying,-- Имя принтера 
				inet,		-- Ip адрес
				character varying -- Мандат 
				)
  RETURNS integer AS
$BODY$
    /*
	Получить ID мандата принтера (функция всегда возвращет значение, если мандат не существует то вернется ID мандата UNDEF_MANDAT)

	Получить id принтера (с параметрами IP,имя принтера,МАНДАТ)если такого не существует,
		то добавить запись в таблицу принтеров вернуть ID записи, записать сообщение в таблицу лога 

	Проверить как часто эти функции будут вызываться по отдельности, в плане объединить их в одну
    */
    DECLARE 
	m_id INTEGER; -- Мандат ID
	p_id INTEGER; -- Принтер ID
	-- Объявление псевдонимов для двух аргументов функции,
	p_name ALIAS for $1;
	ip ALIAS for $2;
	mandat ALIAS for $3;
    BEGIN
	m_id := set_mandat_id(mandat);
	p_id :=(select id from printers where printer_name=p_name and ip_adress=ip and mandat_id=m_id); 
	IF p_id IS NULL THEN	
		INSERT INTO debug_log (info_str) VALUES ('Принтер '||p_name||' c параметрами IP '|| host(ip) ||' не определенный мандат, добавлен');
		INSERT INTO printers (printer_name,ip_adress,mandat_id) VALUES (p_name,ip,m_id);
		RETURN (SELECT id FROM printers WHERE printer_name=p_name and ip_adress=ip and mandat_id=m_id);
	END IF;
	RETURN p_id;
    END;
$BODY$
  LANGUAGE 'plpgsql' VOLATILE
  COST 100;
ALTER FUNCTION set_printer_id(character varying,inet,character varying) OWNER TO postgres;

