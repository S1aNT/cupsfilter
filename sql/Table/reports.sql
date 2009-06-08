--insert into reports (cups_printer_name,doc_name,page_copy) values ('test','doc name',1);
--DROP TABLE reports;

CREATE TABLE reports
(
  id serial NOT NULL,
  printer_id integer,
  firstpages_id integer,
  copy_number integer,
  doc_level_id integer,
  doc_name character varying(255) NOT NULL,
  dt timestamp with time zone NOT NULL DEFAULT now(),
  mb_number character varying(15),
  page_count integer,
  page_copy integer NOT NULL,
  info_str text,
  status character varying(15),
  cups_user character varying(55),
  executor_id integer,
  inv_number integer,
  punkt character varying,
  cups_data_log_id integer,
  jobs_id integer,
  pressman_id integer,
  cups_printer_name character varying,
  CONSTRAINT reports_pkey PRIMARY KEY (id),
  CONSTRAINT "FK_firstpages" FOREIGN KEY (firstpages_id)
      REFERENCES firstpages (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT "FK_pressman" FOREIGN KEY (pressman_id)
      REFERENCES pressman (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT "FK_printers" FOREIGN KEY (printer_id)
      REFERENCES printers (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT "FK_executor" FOREIGN KEY (executor_id)
      REFERENCES executors (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT "PX_main_data" UNIQUE (printer_id, doc_name, doc_level_id, page_count, page_copy, mb_number, copy_number, firstpages_id, status)
)
WITH (OIDS=FALSE);
ALTER TABLE reports OWNER TO postgres;
GRANT ALL ON TABLE reports TO postgres;

-- Index: "FKI_firstpages"

-- DROP INDEX "FKI_firstpages";

CREATE INDEX "FKI_firstpages"
  ON reports
  USING btree
  (firstpages_id);

-- Index: "FKI_pressman"

-- DROP INDEX "FKI_pressman";

CREATE INDEX "FKI_pressman"
  ON reports
  USING btree
  (pressman_id);

-- Index: "FKI_executor"

-- DROP INDEX "FKI_executor";

CREATE INDEX "FKI_executor"
  ON reports
  USING btree
  (executor_id);


-- Index: "FKI_printers"

-- DROP INDEX "FKI_printers";

CREATE INDEX "FKI_printers"
  ON reports
  USING btree
  (printer_id);

-- Index: "PX_cupsuser"

-- DROP INDEX "PX_cupsuser";

CREATE INDEX "PX_cupsuser"
  ON reports
  USING btree
  (cups_user);

-- Index: "PX_doc_name"

-- DROP INDEX "PX_doc_name";

CREATE INDEX "PX_doc_name"
  ON reports
  USING btree
  (doc_name);

-- Index: "PX_status"

-- DROP INDEX "PX_status";

CREATE INDEX "PX_status"
  ON reports
  USING btree
  (status);


-- Trigger: tr_set_printer_id on reports

-- DROP TRIGGER tr_set_printer_id ON reports;

CREATE TRIGGER tr_set_printer_id
  AFTER INSERT
  ON reports
  FOR EACH ROW
  EXECUTE PROCEDURE set_printer_id();

