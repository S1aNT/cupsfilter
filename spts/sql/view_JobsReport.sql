-- View: "JobsReport"

-- DROP VIEW "JobsReport";

CREATE OR REPLACE VIEW "JobsReport" AS 
 SELECT reports.id, reports.date_created, reports.mb_number AS mb_numder, reports.doc_name, reports.tag, reports.page_count, reports.page_copy, reports.user_fio, printers.printer_name, reports.descr
   FROM reports
   JOIN printers ON reports.printer_id = printers.id;

ALTER TABLE "JobsReport" OWNER TO postgres;
GRANT ALL ON TABLE "JobsReport" TO postgres;
GRANT ALL ON TABLE "JobsReport" TO public;
