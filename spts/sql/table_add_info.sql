-- Table: add_info

-- DROP TABLE add_info;

CREATE TABLE add_info
(
  id serial NOT NULL,
  reports_id integer NOT NULL,
  tag_key character varying(255),
  tag_value character varying(255),
  CONSTRAINT add_info_pkey PRIMARY KEY (id),
  CONSTRAINT "FK_reports" FOREIGN KEY (reports_id)
      REFERENCES reports (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT "PX_key_value" UNIQUE (tag_key,tag_value)
)
WITH (OIDS=FALSE);
ALTER TABLE add_info OWNER TO postgres;

-- Index: "FKI_reports"

-- DROP INDEX "FKI_reports";

CREATE INDEX "FKI_reports"
  ON add_info
  USING btree
  (reports_id);