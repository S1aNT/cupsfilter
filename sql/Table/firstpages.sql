-- Table: firstpages

-- DROP TABLE firstpages;

CREATE TABLE firstpages
(
  id serial NOT NULL,
  fp_filename character varying(255) NOT NULL,
  CONSTRAINT firstpages_pkey PRIMARY KEY (id)
)
WITH (OIDS=FALSE);
ALTER TABLE firstpages OWNER TO postgres;
