--
-- PostgreSQL database dump
--

-- Started on 2009-05-29 08:08:21

SET client_encoding = 'UTF8';
SET standard_conforming_strings = off;
SET check_function_bodies = false;
SET client_min_messages = warning;
SET escape_string_warning = off;

--
-- TOC entry 338 (class 2612 OID 17011)
-- Name: plpgsql; Type: PROCEDURAL LANGUAGE; Schema: -; Owner: postgres
--

CREATE PROCEDURAL LANGUAGE plpgsql;


ALTER PROCEDURAL LANGUAGE plpgsql OWNER TO postgres;

SET search_path = public, pg_catalog;

--
-- TOC entry 29 (class 1255 OID 17258)
-- Dependencies: 338 6
-- Name: check_printer_perm(character varying, inet, character varying, character varying); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION check_printer_perm(character varying, inet, character varying, character varying) RETURNS integer
    AS $_$
    DECLARE
        printer_id INTEGER;
    BEGIN
        printer_id := (select printers.id 
			from printers 
				INNER JOIN printers_mandat ON (printers.mandat_id = printers_mandat.id) 
				INNER JOIN rel_mandat2doc_level ON ( rel_mandat2doc_level.mandat_id =printers_mandat.id) 
				INNER JOIN document_level ON (document_level.id = rel_mandat2doc_level.doclevel_id)
			where 
				printers.printer_name=$1 and 
				printers.ip_adress=$2 and 
				printers_mandat.mandat=$3 and 
				document_level.marker=$4
			); 
        IF printer_id ISNULL THEN
            RETURN 0;
        END IF;
        RETURN 1;
    END;
$_$
    LANGUAGE plpgsql;


ALTER FUNCTION public.check_printer_perm(character varying, inet, character varying, character varying) OWNER TO postgres;

--
-- TOC entry 27 (class 1255 OID 17224)
-- Dependencies: 338 6
-- Name: get_doclevel_id(character varying); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION get_doclevel_id(character varying) RETURNS integer
    AS $_$
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
$_$
    LANGUAGE plpgsql;


ALTER FUNCTION public.get_doclevel_id(character varying) OWNER TO postgres;

--
-- TOC entry 21 (class 1255 OID 17013)
-- Dependencies: 338 6
-- Name: get_firstpages_id(character varying); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION get_firstpages_id(character varying) RETURNS integer
    AS $_$
    DECLARE 
	fp_id INTEGER; -- ID записи о первой странице в Pdf
    BEGIN
	fp_id :=(select id from firstpages where fp_filename=$1); 
	IF fp_id IS NULL THEN	
		INSERT INTO firstpages (fp_filename) VALUES ($1);
		RETURN (select id from firstpages where fp_filename=$1); 
	END IF;
	RETURN fp_id;
    END;
$_$
    LANGUAGE plpgsql;


ALTER FUNCTION public.get_firstpages_id(character varying) OWNER TO postgres;

--
-- TOC entry 20 (class 1255 OID 17015)
-- Dependencies: 338 6
-- Name: get_printer_id(character varying, inet, character varying); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION get_printer_id(character varying, inet, character varying) RETURNS integer
    AS $_$
    BEGIN
	RETURN (select id from printers where printer_name=$1 and ip_adress=$2 and mandat_id=get_mandat_id($3)); 
    END;
$_$
    LANGUAGE plpgsql;


ALTER FUNCTION public.get_printer_id(character varying, inet, character varying) OWNER TO postgres;

--
-- TOC entry 22 (class 1255 OID 17016)
-- Dependencies: 6 338
-- Name: get_reports_id(inet, character varying, character varying, character varying, character varying, integer); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION get_reports_id(inet, character varying, character varying, character varying, character varying, integer) RETURNS integer
    AS $_$
    BEGIN
	RETURN (SELECT id FROM reports WHERE printer_id=get_printer_id($2,$1,$3) and doc_name=$5 and page_copy=$6 and cups_user=$4);

    END;
$_$
    LANGUAGE plpgsql;


ALTER FUNCTION public.get_reports_id(inet, character varying, character varying, character varying, character varying, integer) OWNER TO postgres;

--
-- TOC entry 26 (class 1255 OID 17237)
-- Dependencies: 338 6
-- Name: set_executor_id(character varying, character varying, character varying, character varying); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION set_executor_id(last_n character varying, firts_n character varying, middle_n character varying, tlp character varying) RETURNS integer
    AS $_$
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
    
   END;$_$
    LANGUAGE plpgsql;


ALTER FUNCTION public.set_executor_id(last_n character varying, firts_n character varying, middle_n character varying, tlp character varying) OWNER TO postgres;

--
-- TOC entry 23 (class 1255 OID 17210)
-- Dependencies: 338 6
-- Name: set_mandat_id(character varying); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION set_mandat_id(character varying) RETURNS integer
    AS $_$
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
$_$
    LANGUAGE plpgsql;


ALTER FUNCTION public.set_mandat_id(character varying) OWNER TO postgres;

--
-- TOC entry 24 (class 1255 OID 17215)
-- Dependencies: 6 338
-- Name: set_printer_id(character varying, inet, character varying); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION set_printer_id(character varying, inet, character varying) RETURNS integer
    AS $_$
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
$_$
    LANGUAGE plpgsql;


ALTER FUNCTION public.set_printer_id(character varying, inet, character varying) OWNER TO postgres;

--
-- TOC entry 28 (class 1255 OID 17240)
-- Dependencies: 6 338
-- Name: set_reports_info(inet, character varying, character varying, character varying, character varying, integer, character varying, character varying, character varying, integer, character varying, character varying, integer, character varying, character varying, character varying, character varying); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION set_reports_info(inet, character varying, character varying, character varying, character varying, integer, character varying, character varying, character varying, integer, character varying, character varying, integer, character varying, character varying, character varying, character varying) RETURNS integer
    AS $_$
    DECLARE
	prn_id INTEGER;
	rep_id INTEGER;
	fp_id INTEGER;
	dl_id INTEGER;
	ex_id INTEGER;
	-- Объявление псевдонимов для двух аргументов функции,
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
$_$
    LANGUAGE plpgsql;


ALTER FUNCTION public.set_reports_info(inet, character varying, character varying, character varying, character varying, integer, character varying, character varying, character varying, integer, character varying, character varying, integer, character varying, character varying, character varying, character varying) OWNER TO postgres;

--
-- TOC entry 25 (class 1255 OID 17223)
-- Dependencies: 338 6
-- Name: set_reports_short_info(inet, character varying, character varying, character varying, character varying, integer, character varying, character varying); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION set_reports_short_info(inet, character varying, character varying, character varying, character varying, integer, character varying, character varying) RETURNS integer
    AS $_$
    DECLARE
	prn_id INTEGER;
	rep_id INTEGER;
	-- Объявление псевдонимов для двух аргументов функции,
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
$_$
    LANGUAGE plpgsql;


ALTER FUNCTION public.set_reports_short_info(inet, character varying, character varying, character varying, character varying, integer, character varying, character varying) OWNER TO postgres;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- TOC entry 1514 (class 1259 OID 17039)
-- Dependencies: 6
-- Name: document_level; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE document_level (
    id integer NOT NULL,
    marker character varying(3) NOT NULL,
    ds character varying(25) NOT NULL
);


ALTER TABLE public.document_level OWNER TO postgres;

--
-- TOC entry 1891 (class 0 OID 0)
-- Dependencies: 1514
-- Name: TABLE document_level; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE document_level IS 'Таблица описывающая уровни секретности документов:
Не секретно
Секретно
Совершенно секретно
Особой важности';


--
-- TOC entry 1516 (class 1259 OID 17044)
-- Dependencies: 6
-- Name: executors; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE executors (
    id integer NOT NULL,
    last_name character varying(55),
    first_name character varying(55),
    middle_name character varying(55),
    telephone character varying(13)
);


ALTER TABLE public.executors OWNER TO postgres;

--
-- TOC entry 1521 (class 1259 OID 17064)
-- Dependencies: 1806 6
-- Name: printers; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE printers (
    id integer NOT NULL,
    mandat_id integer NOT NULL,
    printer_name character varying(255) NOT NULL,
    date_created timestamp with time zone DEFAULT now() NOT NULL,
    descr text,
    ip_adress inet
);


ALTER TABLE public.printers OWNER TO postgres;

--
-- TOC entry 1528 (class 1259 OID 17087)
-- Dependencies: 1809 6
-- Name: reports; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE reports (
    id integer NOT NULL,
    printer_id integer NOT NULL,
    firstpages_id integer,
    copy_number integer,
    doc_level_id integer,
    doc_name character varying(255) NOT NULL,
    dt timestamp with time zone DEFAULT now() NOT NULL,
    mb_number character varying(15),
    page_count integer,
    page_copy integer NOT NULL,
    info_str text,
    status character varying(15),
    cups_user character varying(55),
    executor_id integer
);


ALTER TABLE public.reports OWNER TO postgres;

--
-- TOC entry 1531 (class 1259 OID 17253)
-- Dependencies: 1607 6
-- Name: JobsReport; Type: VIEW; Schema: public; Owner: postgres
--

CREATE VIEW "JobsReport" AS
    SELECT reports.id, reports.dt AS date, reports.mb_number, reports.doc_name, reports.page_count, reports.page_copy, reports.cups_user, printers.printer_name, reports.info_str, executors.last_name, executors.first_name, executors.middle_name, document_level.marker FROM (((reports JOIN printers ON ((reports.printer_id = printers.id))) JOIN document_level ON ((reports.doc_level_id = document_level.id))) JOIN executors ON ((reports.executor_id = executors.id)));


ALTER TABLE public."JobsReport" OWNER TO postgres;

--
-- TOC entry 1510 (class 1259 OID 17022)
-- Dependencies: 6
-- Name: add_info; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE add_info (
    id integer NOT NULL,
    reports_id integer NOT NULL,
    tag_key character varying(255),
    tag_value character varying(255)
);


ALTER TABLE public.add_info OWNER TO postgres;

--
-- TOC entry 1511 (class 1259 OID 17028)
-- Dependencies: 6 1510
-- Name: add_info_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE add_info_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.add_info_id_seq OWNER TO postgres;

--
-- TOC entry 1897 (class 0 OID 0)
-- Dependencies: 1511
-- Name: add_info_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE add_info_id_seq OWNED BY add_info.id;


--
-- TOC entry 1898 (class 0 OID 0)
-- Dependencies: 1511
-- Name: add_info_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('add_info_id_seq', 1, false);


--
-- TOC entry 1512 (class 1259 OID 17030)
-- Dependencies: 1800 6
-- Name: debug_log; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE debug_log (
    id integer NOT NULL,
    date_time timestamp with time zone DEFAULT now(),
    info_str text
);


ALTER TABLE public.debug_log OWNER TO postgres;

--
-- TOC entry 1513 (class 1259 OID 17037)
-- Dependencies: 6 1512
-- Name: debug_log_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE debug_log_id_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.debug_log_id_seq OWNER TO postgres;

--
-- TOC entry 1900 (class 0 OID 0)
-- Dependencies: 1513
-- Name: debug_log_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE debug_log_id_seq OWNED BY debug_log.id;


--
-- TOC entry 1901 (class 0 OID 0)
-- Dependencies: 1513
-- Name: debug_log_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('debug_log_id_seq', 20, true);


--
-- TOC entry 1515 (class 1259 OID 17042)
-- Dependencies: 6 1514
-- Name: document_level_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE document_level_id_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.document_level_id_seq OWNER TO postgres;

--
-- TOC entry 1902 (class 0 OID 0)
-- Dependencies: 1515
-- Name: document_level_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE document_level_id_seq OWNED BY document_level.id;


--
-- TOC entry 1903 (class 0 OID 0)
-- Dependencies: 1515
-- Name: document_level_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('document_level_id_seq', 5, true);


--
-- TOC entry 1517 (class 1259 OID 17047)
-- Dependencies: 6
-- Name: firstpages; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE firstpages (
    id integer NOT NULL,
    fp_filename character varying(255) NOT NULL
);


ALTER TABLE public.firstpages OWNER TO postgres;

--
-- TOC entry 1518 (class 1259 OID 17050)
-- Dependencies: 6 1517
-- Name: firstpages_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE firstpages_id_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.firstpages_id_seq OWNER TO postgres;

--
-- TOC entry 1905 (class 0 OID 0)
-- Dependencies: 1518
-- Name: firstpages_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE firstpages_id_seq OWNED BY firstpages.id;


--
-- TOC entry 1906 (class 0 OID 0)
-- Dependencies: 1518
-- Name: firstpages_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('firstpages_id_seq', 5, true);


--
-- TOC entry 1519 (class 1259 OID 17059)
-- Dependencies: 6
-- Name: list_recivers; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE list_recivers (
    id integer NOT NULL,
    adress character varying(255)
);


ALTER TABLE public.list_recivers OWNER TO postgres;

--
-- TOC entry 1520 (class 1259 OID 17062)
-- Dependencies: 6 1519
-- Name: list_recivers_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE list_recivers_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.list_recivers_id_seq OWNER TO postgres;

--
-- TOC entry 1908 (class 0 OID 0)
-- Dependencies: 1520
-- Name: list_recivers_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE list_recivers_id_seq OWNED BY list_recivers.id;


--
-- TOC entry 1909 (class 0 OID 0)
-- Dependencies: 1520
-- Name: list_recivers_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('list_recivers_id_seq', 1, false);


--
-- TOC entry 1522 (class 1259 OID 17071)
-- Dependencies: 6 1521
-- Name: printers_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE printers_id_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.printers_id_seq OWNER TO postgres;

--
-- TOC entry 1910 (class 0 OID 0)
-- Dependencies: 1522
-- Name: printers_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE printers_id_seq OWNED BY printers.id;


--
-- TOC entry 1911 (class 0 OID 0)
-- Dependencies: 1522
-- Name: printers_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('printers_id_seq', 15, true);


--
-- TOC entry 1523 (class 1259 OID 17073)
-- Dependencies: 6
-- Name: printers_mandat; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE printers_mandat (
    id integer NOT NULL,
    mandat character varying(25)
);


ALTER TABLE public.printers_mandat OWNER TO postgres;

--
-- TOC entry 1912 (class 0 OID 0)
-- Dependencies: 1523
-- Name: TABLE printers_mandat; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE printers_mandat IS 'Таблица описывающая соотношения грифа документа и мандата принтера';


--
-- TOC entry 1524 (class 1259 OID 17076)
-- Dependencies: 6 1523
-- Name: printers_mandat_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE printers_mandat_id_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.printers_mandat_id_seq OWNER TO postgres;

--
-- TOC entry 1914 (class 0 OID 0)
-- Dependencies: 1524
-- Name: printers_mandat_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE printers_mandat_id_seq OWNED BY printers_mandat.id;


--
-- TOC entry 1915 (class 0 OID 0)
-- Dependencies: 1524
-- Name: printers_mandat_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('printers_mandat_id_seq', 18, true);


--
-- TOC entry 1525 (class 1259 OID 17078)
-- Dependencies: 6
-- Name: rel_add_info2reports; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE rel_add_info2reports (
    add_info_id integer NOT NULL,
    reports_id integer NOT NULL
);


ALTER TABLE public.rel_add_info2reports OWNER TO postgres;

--
-- TOC entry 1526 (class 1259 OID 17081)
-- Dependencies: 6
-- Name: rel_list_recivers2reports; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE rel_list_recivers2reports (
    list_recivers_id integer NOT NULL,
    reports_id integer NOT NULL
);


ALTER TABLE public.rel_list_recivers2reports OWNER TO postgres;

--
-- TOC entry 1527 (class 1259 OID 17084)
-- Dependencies: 6
-- Name: rel_mandat2doc_level; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE rel_mandat2doc_level (
    mandat_id integer NOT NULL,
    doclevel_id integer NOT NULL
);


ALTER TABLE public.rel_mandat2doc_level OWNER TO postgres;

--
-- TOC entry 1918 (class 0 OID 0)
-- Dependencies: 1527
-- Name: TABLE rel_mandat2doc_level; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE rel_mandat2doc_level IS 'Связь между мандатом принтера и разрешенными к печати уровнями документов';


--
-- TOC entry 1529 (class 1259 OID 17094)
-- Dependencies: 6 1528
-- Name: reports_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE reports_id_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.reports_id_seq OWNER TO postgres;

--
-- TOC entry 1920 (class 0 OID 0)
-- Dependencies: 1529
-- Name: reports_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE reports_id_seq OWNED BY reports.id;


--
-- TOC entry 1921 (class 0 OID 0)
-- Dependencies: 1529
-- Name: reports_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('reports_id_seq', 7, true);


--
-- TOC entry 1530 (class 1259 OID 17098)
-- Dependencies: 6 1516
-- Name: users_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE users_id_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.users_id_seq OWNER TO postgres;

--
-- TOC entry 1922 (class 0 OID 0)
-- Dependencies: 1530
-- Name: users_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE users_id_seq OWNED BY executors.id;


--
-- TOC entry 1923 (class 0 OID 0)
-- Dependencies: 1530
-- Name: users_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('users_id_seq', 2, true);


--
-- TOC entry 1799 (class 2604 OID 17100)
-- Dependencies: 1511 1510
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE add_info ALTER COLUMN id SET DEFAULT nextval('add_info_id_seq'::regclass);


--
-- TOC entry 1801 (class 2604 OID 17101)
-- Dependencies: 1513 1512
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE debug_log ALTER COLUMN id SET DEFAULT nextval('debug_log_id_seq'::regclass);


--
-- TOC entry 1802 (class 2604 OID 17102)
-- Dependencies: 1515 1514
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE document_level ALTER COLUMN id SET DEFAULT nextval('document_level_id_seq'::regclass);


--
-- TOC entry 1803 (class 2604 OID 17103)
-- Dependencies: 1530 1516
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE executors ALTER COLUMN id SET DEFAULT nextval('users_id_seq'::regclass);


--
-- TOC entry 1804 (class 2604 OID 17104)
-- Dependencies: 1518 1517
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE firstpages ALTER COLUMN id SET DEFAULT nextval('firstpages_id_seq'::regclass);


--
-- TOC entry 1805 (class 2604 OID 17106)
-- Dependencies: 1520 1519
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE list_recivers ALTER COLUMN id SET DEFAULT nextval('list_recivers_id_seq'::regclass);


--
-- TOC entry 1807 (class 2604 OID 17107)
-- Dependencies: 1522 1521
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE printers ALTER COLUMN id SET DEFAULT nextval('printers_id_seq'::regclass);


--
-- TOC entry 1808 (class 2604 OID 17108)
-- Dependencies: 1524 1523
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE printers_mandat ALTER COLUMN id SET DEFAULT nextval('printers_mandat_id_seq'::regclass);


--
-- TOC entry 1810 (class 2604 OID 17109)
-- Dependencies: 1529 1528
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE reports ALTER COLUMN id SET DEFAULT nextval('reports_id_seq'::regclass);


--
-- TOC entry 1874 (class 0 OID 17022)
-- Dependencies: 1510
-- Data for Name: add_info; Type: TABLE DATA; Schema: public; Owner: postgres
--



--
-- TOC entry 1875 (class 0 OID 17030)
-- Dependencies: 1512
-- Data for Name: debug_log; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO debug_log (id, date_time, info_str) VALUES (20, '2009-05-29 07:24:39.734+04', 'Can''t determine the rights assigned to printer PDFwith 192.168.11.150 and SECRET, document level ');


--
-- TOC entry 1876 (class 0 OID 17039)
-- Dependencies: 1514
-- Data for Name: document_level; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO document_level (id, marker, ds) VALUES (1, 'НУ', 'Не установленно');
INSERT INTO document_level (id, marker, ds) VALUES (2, 'НС', 'Не секретно');
INSERT INTO document_level (id, marker, ds) VALUES (3, 'С', 'Секретно');
INSERT INTO document_level (id, marker, ds) VALUES (4, 'СС', 'Совершенно секретно');
INSERT INTO document_level (id, marker, ds) VALUES (5, 'ОВ', 'Особой важности');


--
-- TOC entry 1877 (class 0 OID 17044)
-- Dependencies: 1516
-- Data for Name: executors; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO executors (id, last_name, first_name, middle_name, telephone) VALUES (1, 'Мосенцев961', 'Павел', 'Владимирович', '+7964xxxxxxx');
INSERT INTO executors (id, last_name, first_name, middle_name, telephone) VALUES (2, 'Мосенцев', 'Павел', 'Владимирович', '+7964xxxxxxx');


--
-- TOC entry 1878 (class 0 OID 17047)
-- Dependencies: 1517
-- Data for Name: firstpages; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO firstpages (id, fp_filename) VALUES (5, 'd/:/test.pdf');


--
-- TOC entry 1879 (class 0 OID 17059)
-- Dependencies: 1519
-- Data for Name: list_recivers; Type: TABLE DATA; Schema: public; Owner: postgres
--



--
-- TOC entry 1880 (class 0 OID 17064)
-- Dependencies: 1521
-- Data for Name: printers; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO printers (id, mandat_id, printer_name, date_created, descr, ip_adress) VALUES (7, 18, 'PDF', '2009-05-29 02:20:58.781+04', NULL, '192.168.11.151');
INSERT INTO printers (id, mandat_id, printer_name, date_created, descr, ip_adress) VALUES (14, 18, 'HPLaserJet', '2009-05-29 02:45:50.843+04', NULL, '192.168.11.1');
INSERT INTO printers (id, mandat_id, printer_name, date_created, descr, ip_adress) VALUES (15, 18, 'HPLaserJet5', '2009-05-29 02:50:20.968+04', NULL, '192.168.11.99');


--
-- TOC entry 1881 (class 0 OID 17073)
-- Dependencies: 1523
-- Data for Name: printers_mandat; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO printers_mandat (id, mandat) VALUES (18, 'UNDEF_MANDAT');


--
-- TOC entry 1882 (class 0 OID 17078)
-- Dependencies: 1525
-- Data for Name: rel_add_info2reports; Type: TABLE DATA; Schema: public; Owner: postgres
--



--
-- TOC entry 1883 (class 0 OID 17081)
-- Dependencies: 1526
-- Data for Name: rel_list_recivers2reports; Type: TABLE DATA; Schema: public; Owner: postgres
--



--
-- TOC entry 1884 (class 0 OID 17084)
-- Dependencies: 1527
-- Data for Name: rel_mandat2doc_level; Type: TABLE DATA; Schema: public; Owner: postgres
--



--
-- TOC entry 1885 (class 0 OID 17087)
-- Dependencies: 1528
-- Data for Name: reports; Type: TABLE DATA; Schema: public; Owner: postgres
--

INSERT INTO reports (id, printer_id, firstpages_id, copy_number, doc_level_id, doc_name, dt, mb_number, page_count, page_copy, info_str, status, cups_user, executor_id) VALUES (1, 14, NULL, NULL, NULL, 'Как размножаются ежики.doc', '2009-05-29 02:48:54.015+04', NULL, NULL, 6, 'Ошибка разбора документа.Нет маркеров безопастности', 'ОШИБКА', 'slant', 1);
INSERT INTO reports (id, printer_id, firstpages_id, copy_number, doc_level_id, doc_name, dt, mb_number, page_count, page_copy, info_str, status, cups_user, executor_id) VALUES (2, 15, NULL, NULL, NULL, 'Как размножаются ежики.doc', '2009-05-29 02:50:20.968+04', NULL, NULL, 6, 'Ошибка разбора документа.Нет маркеров безопастности', 'ОШИБКА', 'guest', 2);
INSERT INTO reports (id, printer_id, firstpages_id, copy_number, doc_level_id, doc_name, dt, mb_number, page_count, page_copy, info_str, status, cups_user, executor_id) VALUES (7, 15, NULL, NULL, NULL, 'Как размножаются ежики.doc', '2009-05-29 04:13:13.031+04', NULL, NULL, 6, 'Ошибка разбора документа,нет маркеров безопастности', 'ОШИБКА', 'guest', 2);


--
-- TOC entry 1850 (class 2606 OID 17111)
-- Dependencies: 1527 1527 1527
-- Name: PK; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY rel_mandat2doc_level
    ADD CONSTRAINT "PK" PRIMARY KEY (mandat_id, doclevel_id);


--
-- TOC entry 1842 (class 2606 OID 17113)
-- Dependencies: 1525 1525 1525
-- Name: PK_add_info2reports; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY rel_add_info2reports
    ADD CONSTRAINT "PK_add_info2reports" PRIMARY KEY (add_info_id, reports_id);


--
-- TOC entry 1846 (class 2606 OID 17115)
-- Dependencies: 1526 1526 1526
-- Name: PK_list_recivers2reports; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY rel_list_recivers2reports
    ADD CONSTRAINT "PK_list_recivers2reports" PRIMARY KEY (list_recivers_id, reports_id);


--
-- TOC entry 1829 (class 2606 OID 17117)
-- Dependencies: 1519 1519
-- Name: PX_adress; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY list_recivers
    ADD CONSTRAINT "PX_adress" UNIQUE (adress);


--
-- TOC entry 1825 (class 2606 OID 17119)
-- Dependencies: 1517 1517
-- Name: PX_filename; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY firstpages
    ADD CONSTRAINT "PX_filename" UNIQUE (fp_filename);


--
-- TOC entry 1821 (class 2606 OID 17121)
-- Dependencies: 1516 1516 1516 1516
-- Name: PX_fio; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY executors
    ADD CONSTRAINT "PX_fio" UNIQUE (last_name, first_name, middle_name);


--
-- TOC entry 1834 (class 2606 OID 17123)
-- Dependencies: 1521 1521 1521 1521
-- Name: PX_ip_name_mandat; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY printers
    ADD CONSTRAINT "PX_ip_name_mandat" UNIQUE (ip_adress, printer_name, mandat_id);


--
-- TOC entry 1813 (class 2606 OID 17125)
-- Dependencies: 1510 1510 1510
-- Name: PX_key_value; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY add_info
    ADD CONSTRAINT "PX_key_value" UNIQUE (tag_key, tag_value);


--
-- TOC entry 1858 (class 2606 OID 17226)
-- Dependencies: 1528 1528 1528 1528 1528 1528 1528 1528 1528 1528
-- Name: PX_main_data; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY reports
    ADD CONSTRAINT "PX_main_data" UNIQUE (printer_id, doc_name, doc_level_id, page_count, page_copy, mb_number, copy_number, firstpages_id, status);


--
-- TOC entry 1838 (class 2606 OID 17127)
-- Dependencies: 1523 1523
-- Name: PX_mandat; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY printers_mandat
    ADD CONSTRAINT "PX_mandat" UNIQUE (mandat);


--
-- TOC entry 1815 (class 2606 OID 17129)
-- Dependencies: 1510 1510
-- Name: add_info_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY add_info
    ADD CONSTRAINT add_info_pkey PRIMARY KEY (id);


--
-- TOC entry 1817 (class 2606 OID 17131)
-- Dependencies: 1512 1512
-- Name: debug_log_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY debug_log
    ADD CONSTRAINT debug_log_pkey PRIMARY KEY (id);


--
-- TOC entry 1819 (class 2606 OID 17133)
-- Dependencies: 1514 1514
-- Name: document_level_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY document_level
    ADD CONSTRAINT document_level_pkey PRIMARY KEY (id);


--
-- TOC entry 1827 (class 2606 OID 17135)
-- Dependencies: 1517 1517
-- Name: firstpages_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY firstpages
    ADD CONSTRAINT firstpages_pkey PRIMARY KEY (id);


--
-- TOC entry 1831 (class 2606 OID 17137)
-- Dependencies: 1519 1519
-- Name: list_recivers_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY list_recivers
    ADD CONSTRAINT list_recivers_pkey PRIMARY KEY (id);


--
-- TOC entry 1840 (class 2606 OID 17139)
-- Dependencies: 1523 1523
-- Name: printers_mandat_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY printers_mandat
    ADD CONSTRAINT printers_mandat_pkey PRIMARY KEY (id);


--
-- TOC entry 1836 (class 2606 OID 17141)
-- Dependencies: 1521 1521
-- Name: printers_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY printers
    ADD CONSTRAINT printers_pkey PRIMARY KEY (id);


--
-- TOC entry 1862 (class 2606 OID 17145)
-- Dependencies: 1528 1528
-- Name: reports_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY reports
    ADD CONSTRAINT reports_pkey PRIMARY KEY (id);


--
-- TOC entry 1823 (class 2606 OID 17147)
-- Dependencies: 1516 1516
-- Name: users_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY executors
    ADD CONSTRAINT users_pkey PRIMARY KEY (id);


--
-- TOC entry 1853 (class 1259 OID 17148)
-- Dependencies: 1528
-- Name: FKI_firstpages; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX "FKI_firstpages" ON reports USING btree (firstpages_id);


--
-- TOC entry 1832 (class 1259 OID 17149)
-- Dependencies: 1521
-- Name: FKI_mandat; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX "FKI_mandat" ON printers USING btree (mandat_id);


--
-- TOC entry 1854 (class 1259 OID 17150)
-- Dependencies: 1528
-- Name: FKI_printers; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX "FKI_printers" ON reports USING btree (printer_id);


--
-- TOC entry 1811 (class 1259 OID 17151)
-- Dependencies: 1510
-- Name: FKI_reports; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX "FKI_reports" ON add_info USING btree (reports_id);


--
-- TOC entry 1855 (class 1259 OID 17235)
-- Dependencies: 1528
-- Name: PX_cupsuser; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX "PX_cupsuser" ON reports USING btree (cups_user);


--
-- TOC entry 1856 (class 1259 OID 17233)
-- Dependencies: 1528
-- Name: PX_doc_name; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX "PX_doc_name" ON reports USING btree (doc_name);


--
-- TOC entry 1859 (class 1259 OID 17234)
-- Dependencies: 1528
-- Name: PX_status; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX "PX_status" ON reports USING btree (status);


--
-- TOC entry 1847 (class 1259 OID 17152)
-- Dependencies: 1526
-- Name: fki2list_recivers; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX fki2list_recivers ON rel_list_recivers2reports USING btree (list_recivers_id);


--
-- TOC entry 1848 (class 1259 OID 17153)
-- Dependencies: 1526
-- Name: fki2reports; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX fki2reports ON rel_list_recivers2reports USING btree (reports_id);


--
-- TOC entry 1860 (class 1259 OID 17232)
-- Dependencies: 1528
-- Name: fki_Fk_executor; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX "fki_Fk_executor" ON reports USING btree (executor_id);


--
-- TOC entry 1843 (class 1259 OID 17154)
-- Dependencies: 1525
-- Name: fki_KF_add_info; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX "fki_KF_add_info" ON rel_add_info2reports USING btree (add_info_id);


--
-- TOC entry 1851 (class 1259 OID 17155)
-- Dependencies: 1527
-- Name: fki_KF_doc_level; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX "fki_KF_doc_level" ON rel_mandat2doc_level USING btree (doclevel_id);


--
-- TOC entry 1852 (class 1259 OID 17156)
-- Dependencies: 1527
-- Name: fki_KF_mandat; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX "fki_KF_mandat" ON rel_mandat2doc_level USING btree (mandat_id);


--
-- TOC entry 1844 (class 1259 OID 17157)
-- Dependencies: 1525
-- Name: fki_KF_reports; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX "fki_KF_reports" ON rel_add_info2reports USING btree (reports_id);


--
-- TOC entry 1865 (class 2606 OID 17159)
-- Dependencies: 1510 1525 1814
-- Name: FK_add_info; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY rel_add_info2reports
    ADD CONSTRAINT "FK_add_info" FOREIGN KEY (add_info_id) REFERENCES add_info(id);


--
-- TOC entry 1869 (class 2606 OID 17164)
-- Dependencies: 1527 1818 1514
-- Name: FK_doclevel; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY rel_mandat2doc_level
    ADD CONSTRAINT "FK_doclevel" FOREIGN KEY (doclevel_id) REFERENCES document_level(id);


--
-- TOC entry 1871 (class 2606 OID 17169)
-- Dependencies: 1517 1528 1826
-- Name: FK_firstpages; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY reports
    ADD CONSTRAINT "FK_firstpages" FOREIGN KEY (firstpages_id) REFERENCES firstpages(id);


--
-- TOC entry 1867 (class 2606 OID 17174)
-- Dependencies: 1519 1830 1526
-- Name: FK_list_recivers; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY rel_list_recivers2reports
    ADD CONSTRAINT "FK_list_recivers" FOREIGN KEY (list_recivers_id) REFERENCES list_recivers(id);


--
-- TOC entry 1864 (class 2606 OID 17179)
-- Dependencies: 1523 1839 1521
-- Name: FK_mandat; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY printers
    ADD CONSTRAINT "FK_mandat" FOREIGN KEY (mandat_id) REFERENCES printers_mandat(id);


--
-- TOC entry 1870 (class 2606 OID 17184)
-- Dependencies: 1523 1839 1527
-- Name: FK_mandat; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY rel_mandat2doc_level
    ADD CONSTRAINT "FK_mandat" FOREIGN KEY (mandat_id) REFERENCES printers_mandat(id);


--
-- TOC entry 1872 (class 2606 OID 17189)
-- Dependencies: 1528 1835 1521
-- Name: FK_printers; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY reports
    ADD CONSTRAINT "FK_printers" FOREIGN KEY (printer_id) REFERENCES printers(id);


--
-- TOC entry 1863 (class 2606 OID 17194)
-- Dependencies: 1528 1861 1510
-- Name: FK_reports; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY add_info
    ADD CONSTRAINT "FK_reports" FOREIGN KEY (reports_id) REFERENCES reports(id);


--
-- TOC entry 1873 (class 2606 OID 17227)
-- Dependencies: 1822 1528 1516
-- Name: Fk_executor; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY reports
    ADD CONSTRAINT "Fk_executor" FOREIGN KEY (executor_id) REFERENCES executors(id);


--
-- TOC entry 1866 (class 2606 OID 17199)
-- Dependencies: 1861 1528 1525
-- Name: KF_reports; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY rel_add_info2reports
    ADD CONSTRAINT "KF_reports" FOREIGN KEY (reports_id) REFERENCES reports(id);


--
-- TOC entry 1868 (class 2606 OID 17204)
-- Dependencies: 1528 1526 1861
-- Name: KF_reports; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY rel_list_recivers2reports
    ADD CONSTRAINT "KF_reports" FOREIGN KEY (reports_id) REFERENCES reports(id);


--
-- TOC entry 1890 (class 0 OID 0)
-- Dependencies: 6
-- Name: public; Type: ACL; Schema: -; Owner: postgres
--

REVOKE ALL ON SCHEMA public FROM PUBLIC;
REVOKE ALL ON SCHEMA public FROM postgres;
GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON SCHEMA public TO PUBLIC;


--
-- TOC entry 1892 (class 0 OID 0)
-- Dependencies: 1514
-- Name: document_level; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE document_level FROM PUBLIC;
REVOKE ALL ON TABLE document_level FROM postgres;
GRANT ALL ON TABLE document_level TO postgres;


--
-- TOC entry 1893 (class 0 OID 0)
-- Dependencies: 1516
-- Name: executors; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE executors FROM PUBLIC;
REVOKE ALL ON TABLE executors FROM postgres;
GRANT ALL ON TABLE executors TO postgres;


--
-- TOC entry 1894 (class 0 OID 0)
-- Dependencies: 1521
-- Name: printers; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE printers FROM PUBLIC;
REVOKE ALL ON TABLE printers FROM postgres;
GRANT ALL ON TABLE printers TO postgres;


--
-- TOC entry 1895 (class 0 OID 0)
-- Dependencies: 1528
-- Name: reports; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE reports FROM PUBLIC;
REVOKE ALL ON TABLE reports FROM postgres;
GRANT ALL ON TABLE reports TO postgres;


--
-- TOC entry 1896 (class 0 OID 0)
-- Dependencies: 1510
-- Name: add_info; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE add_info FROM PUBLIC;
REVOKE ALL ON TABLE add_info FROM postgres;
GRANT ALL ON TABLE add_info TO postgres;


--
-- TOC entry 1899 (class 0 OID 0)
-- Dependencies: 1512
-- Name: debug_log; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE debug_log FROM PUBLIC;
REVOKE ALL ON TABLE debug_log FROM postgres;
GRANT ALL ON TABLE debug_log TO postgres;


--
-- TOC entry 1904 (class 0 OID 0)
-- Dependencies: 1517
-- Name: firstpages; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE firstpages FROM PUBLIC;
REVOKE ALL ON TABLE firstpages FROM postgres;
GRANT ALL ON TABLE firstpages TO postgres;


--
-- TOC entry 1907 (class 0 OID 0)
-- Dependencies: 1519
-- Name: list_recivers; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE list_recivers FROM PUBLIC;
REVOKE ALL ON TABLE list_recivers FROM postgres;
GRANT ALL ON TABLE list_recivers TO postgres;


--
-- TOC entry 1913 (class 0 OID 0)
-- Dependencies: 1523
-- Name: printers_mandat; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE printers_mandat FROM PUBLIC;
REVOKE ALL ON TABLE printers_mandat FROM postgres;
GRANT ALL ON TABLE printers_mandat TO postgres;


--
-- TOC entry 1916 (class 0 OID 0)
-- Dependencies: 1525
-- Name: rel_add_info2reports; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE rel_add_info2reports FROM PUBLIC;
REVOKE ALL ON TABLE rel_add_info2reports FROM postgres;
GRANT ALL ON TABLE rel_add_info2reports TO postgres;


--
-- TOC entry 1917 (class 0 OID 0)
-- Dependencies: 1526
-- Name: rel_list_recivers2reports; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE rel_list_recivers2reports FROM PUBLIC;
REVOKE ALL ON TABLE rel_list_recivers2reports FROM postgres;
GRANT ALL ON TABLE rel_list_recivers2reports TO postgres;


--
-- TOC entry 1919 (class 0 OID 0)
-- Dependencies: 1527
-- Name: rel_mandat2doc_level; Type: ACL; Schema: public; Owner: postgres
--

REVOKE ALL ON TABLE rel_mandat2doc_level FROM PUBLIC;
REVOKE ALL ON TABLE rel_mandat2doc_level FROM postgres;
GRANT ALL ON TABLE rel_mandat2doc_level TO postgres;


-- Completed on 2009-05-29 08:08:21

--
-- PostgreSQL database dump complete
--

