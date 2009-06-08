-- Table: pressman

-- DROP TABLE pressman;

CREATE TABLE pressman
(
  id SERIAL NOT NULL,
  fio character varying,
  CONSTRAINT PK_pressman PRIMARY KEY (id)
)
WITH (OIDS=FALSE);
ALTER TABLE pressman OWNER TO postgres;
GRANT ALL ON TABLE pressman TO postgres;