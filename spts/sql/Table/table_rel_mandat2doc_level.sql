-- Table: rel_mandat2doc_level

-- DROP TABLE rel_mandat2doc_level;

CREATE TABLE rel_mandat2doc_level
(
  mandat_id integer NOT NULL,
  doclevel_id integer NOT NULL,
  CONSTRAINT "PK" PRIMARY KEY (mandat_id, doclevel_id),
  CONSTRAINT "FK_doclevel" FOREIGN KEY (doclevel_id)
      REFERENCES document_level (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT "FK_mandat" FOREIGN KEY (mandat_id)
      REFERENCES printers_mandat (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (OIDS=FALSE);
ALTER TABLE rel_mandat2doc_level OWNER TO postgres;
COMMENT ON TABLE rel_mandat2doc_level IS 'Связь между мандатом принтера и разрешенными к печати уровнями документов';

-- Index: "FKI_doclevel"

-- DROP INDEX "FKI_doclevel";

CREATE INDEX "FKI_doclevel"
  ON rel_mandat2doc_level
  USING btree
  (doclevel_id);
