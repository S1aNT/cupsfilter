-- Table: document_level

-- DROP TABLE document_level;

CREATE TABLE document_level
(
  id serial NOT NULL,
  marker character varying(3) NOT NULL,
  ds character varying(25) NOT NULL,
  CONSTRAINT document_level_pkey PRIMARY KEY (id)
)
WITH (OIDS=FALSE);
ALTER TABLE document_level OWNER TO postgres;
COMMENT ON TABLE document_level IS 'Таблица описывающая уровни секретности документов:
Не секретно
Секретно
Совершенно секретно
Особой важности';
