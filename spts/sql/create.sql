CREATE TABLE reports (
	report_id SERIAL,
        printer_id integer NOT NULL,
	fio varchar(255) NOT NULL,
	doc_name varchar(255) NOT NULL,
	date_created date DEFAULT NOW() NOT NULL,
	mb_numder varchar(15) NOT NULL,
        tag varchar(55) NOT NULL, 
        page_count integer NOT NULL,
        page_copy  integer NOT NULL,
	descr text,
	PRIMARY KEY (report_id)
);


CREATE TABLE printers (
	printers_id SERIAL,
        printer_name varchar(255) NOT NULL,
	date_created date DEFAULT NOW() NOT NULL,
	descr text,
	PRIMARY KEY (printers_id)
);

CREATE VIEW "Reports" 
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
