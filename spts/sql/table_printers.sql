-- Table: printers

-- DROP TABLE printers;

CREATE TABLE printers
(
  id serial NOT NULL,
  mandat_id integer NOT NULL,
  printer_name character varying(255) NOT NULL,
  date_created timestamp with time zone NOT NULL DEFAULT now(),
  descr text,
  ip_adress inet,
  CONSTRAINT printers_pkey PRIMARY KEY (id),
  CONSTRAINT "FK_mandat" FOREIGN KEY (mandat_id)
      REFERENCES printers_mandat (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (OIDS=FALSE);
ALTER TABLE printers OWNER TO postgres;

-- Index: "FKI_mandat"

-- DROP INDEX "FKI_mandat";

CREATE INDEX "FKI_mandat"
  ON printers
  USING btree
  (mandat_id);