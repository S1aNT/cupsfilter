-- Table: debug_log

-- DROP TABLE debug_log;

CREATE TABLE debug_log
(
  id serial NOT NULL,
  date_time timestamp with time zone DEFAULT now(),
  info_str text,
  CONSTRAINT debug_log_pkey PRIMARY KEY (id)
)
WITH (OIDS=FALSE);
ALTER TABLE debug_log OWNER TO postgres;
