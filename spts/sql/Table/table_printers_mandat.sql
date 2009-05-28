-- Table: printers_mandat

-- DROP TABLE printers_mandat;

CREATE TABLE printers_mandat
(
  id integer NOT NULL DEFAULT nextval('print_permission_id_seq'::regclass),
  mandat character varying(25),
  CONSTRAINT print_permission_pkey PRIMARY KEY (id)
)
WITH (OIDS=FALSE);
ALTER TABLE printers_mandat OWNER TO postgres;
COMMENT ON TABLE printers_mandat IS '“аблица описывающа€ соотношени€ грифа документа и мандата принтера';