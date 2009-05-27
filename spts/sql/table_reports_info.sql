-- Table: reports_info

-- DROP TABLE reports_info;

CREATE TABLE reports_info
(
  id serial NOT NULL,
  printer_name character varying(255) NOT NULL,
  printer_mandat character varying(255) NOT NULL,
  firstpages_path character varying(255) NOT NULL,
  user_fio character varying(255) NOT NULL,
  doc_name character varying(255) NOT NULL,
  date_created timestamp with time zone NOT NULL DEFAULT now(),
  mb_number character varying(15) NOT NULL,
  tag character varying(55) NOT NULL,
  page_count integer NOT NULL,
  page_copy integer NOT NULL,
  descr text,
  printer_ip inet,
  CONSTRAINT reports_info_pkey PRIMARY KEY (id)
)
WITH (OIDS=FALSE);
ALTER TABLE reports_info OWNER TO postgres;
COMMENT ON TABLE reports_info IS 'Ќа вставку каждой записи срабатывает триггер который получает printer_id firstpages_id и записывает в таблицу reports все остальные данные';

-- Trigger: get_id on reports_info

-- DROP TRIGGER get_id ON reports_info;

CREATE TRIGGER get_id
  AFTER INSERT
  ON reports_info
  FOR EACH ROW
  EXECUTE PROCEDURE process_get_id();
