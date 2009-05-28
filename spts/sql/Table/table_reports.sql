-- Table: reports

DROP TABLE reports;

CREATE TABLE reports
(
  id serial NOT NULL,
  printer_id integer NOT NULL,
  firstpages_id integer NOT NULL,
  user_id integer NOT NULL,
  list_senders_id integer NOT NULL,
  copy_number integer NOT NULL,
  document_level integer NOT NULL,
  additional_info_id integer NOT NULL,
  doc_name character varying(255) NOT NULL,
  date_created date NOT NULL DEFAULT now(),
  mb_number character varying(15) NOT NULL,
  tag character varying(55) NOT NULL,
  page_count integer NOT NULL,
  page_copy integer NOT NULL,
  descr text,
 
  CONSTRAINT reports_pkey PRIMARY KEY (id),
  CONSTRAINT "FK_firstpages" FOREIGN KEY (firstpages_id)
      REFERENCES firstpages (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT "FK_printers" FOREIGN KEY (printer_id)
      REFERENCES printers (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT "FK_users" FOREIGN KEY (user_id)
      REFERENCES users (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (OIDS=FALSE);
ALTER TABLE reports OWNER TO postgres;

-- Index: "FKI_firstpages"

-- DROP INDEX "FKI_firstpages";

CREATE INDEX "FKI_firstpages"
  ON reports
  USING btree
  (firstpages_id);

-- Index: "FKI_printers"

-- DROP INDEX "FKI_printers";

CREATE INDEX "FKI_printers"
  ON reports
  USING btree
  (printer_id);

-- Index: "FKI_users"

-- DROP INDEX "FKI_users";

CREATE INDEX "FKI_users"
  ON reports
  USING btree
  (user_id);

