-- Table: list_senders

-- DROP TABLE list_senders;

CREATE TABLE list_senders
(
  id serial NOT NULL,
  adress character varying(255),
  CONSTRAINT list_senders_pkey PRIMARY KEY (id),
  CONSTRAINT "PX_senders" UNIQUE (adress)
)
WITH (OIDS=FALSE);
ALTER TABLE list_senders OWNER TO postgres;
