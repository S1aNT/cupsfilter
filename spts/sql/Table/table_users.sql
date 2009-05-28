-- Table:users

DROP TABLE users;


CREATE TABLE users
(
  id serial NOT NULL,
  last_name character varying(55),
  first_name character varying(55),
  middle_name character varying(55),
  telephone character varying(13),
  CONSTRAINT users_pkey PRIMARY KEY (id),
  CONSTRAINT "PX_fio" UNIQUE (last_name, first_name, middle_name)
);

ALTER TABLE users OWNER TO postgres;

