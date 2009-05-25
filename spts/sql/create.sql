--DROP TABLE "printers";
CREATE TABLE printers (
	id SERIAL,
	IP varchar(10),
	mandat_id  integer NOT NULL,
        printer_name varchar(255) NOT NULL,
	date_created date DEFAULT NOW() NOT NULL,
	descr text,
	PRIMARY KEY (id)
);

--DROP TABLE "reports";
CREATE TABLE reports (
	id SERIAL,
        printer_id integer NOT NULL,
        firstpages_id integer NOT NULL,
	user_fio varchar(255) NOT NULL,
	doc_name varchar(255) NOT NULL,
	date_created date DEFAULT NOW() NOT NULL,
	mb_numder varchar(15) NOT NULL,
        tag varchar(55) NOT NULL, 
        page_count integer NOT NULL,
        page_copy  integer NOT NULL,
	descr text,
	PRIMARY KEY (id)
);

--DROP TABLE "firstpages";
CREATE TABLE FirstPages (
	id SERIAL,
	fp_filename varchar(255) NOT NULL,
	PRIMARY KEY (id)
);

-- Table: debug_log

--DROP TABLE debug_log;

CREATE TABLE debug_log(
  	id SERIAL NOT NULL,
	date_time date DEFAULT now(),
	info_str text,
        PRIMARY KEY (id)

);

ALTER TABLE debug_log OWNER TO postgres;


--DROP TABLE print_permission;

CREATE TABLE print_permission
(
  	id SERIAL,
	mandat varchar(25),
	PRIMARY KEY (id)
)
WITH (OIDS=FALSE);
ALTER TABLE print_permission OWNER TO postgres;
COMMENT ON TABLE print_permission IS '“аблица описывающа€ соотношени€ грифа документа и мандата принтера';

/*
DROP VIEW "JobsReport" ;
CREATE VIEW "JobsReport" 
	AS SELECT 
        "public"."reports"."date_created",
        "public"."reports"."mb_numder",
        "public"."reports"."doc_name", 
        "public"."reports"."tag", 
	"public"."reports"."page_count",
        "public"."reports"."page_copy", 
	"public"."reports"."fio",
        "public"."printers"."printer_name", 
	"public"."reports"."descr"
		FROM "public"."reports" 
		INNER JOIN "public"."printers" ON ("public"."reports"."printer_id" = "public"."printers"."printers_id")
*/