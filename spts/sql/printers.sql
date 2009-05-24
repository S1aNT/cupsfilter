--DROP TABLE "Printers";
CREATE TABLE Printers (
	id SERIAL,
	IP varchar(10),
	Mandat varchar(25),
        printer_name varchar(255) NOT NULL,
	date_created date DEFAULT NOW() NOT NULL,
	descr text,
	PRIMARY KEY (id)
);

--DROP TABLE "Reports";
CREATE TABLE Reports (
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

--DROP TABLE "FirstPages";
CREATE TABLE FirstPages (
	id SERIAL,
	fp_filename varchar(255) NOT NULL,
	PRIMARY KEY (id)
);

-- Table: debug_log

-- DROP TABLE debug_log;

CREATE TABLE debug_log(
  	id SERIAL NOT NULL,
	date_time date DEFAULT now(),
	info_str text,
        PRIMARY KEY (id)

)
ALTER TABLE debug_log OWNER TO postgres;




select * from printers;
insert into printers (ip,mandat,printer_name)VALUES('127.0.0.1','SYSPROG','PDF');
insert into printers (ip,mandat,printer_name)VALUES('192.168.11.150','SYSPROG','PDF');
insert into printers (ip,mandat,printer_name)VALUES('192.168.11.150','SYSPROG','MyBackendPrinter');

/*
--DROP VIEW "JobsReport" ;
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