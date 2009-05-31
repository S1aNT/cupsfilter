DROP VIEW "JobsReport";
CREATE OR REPLACE VIEW "JobsReport" AS 
 SELECT reports.id, reports.dt as Date, reports.mb_number, reports.doc_name, reports.page_count, reports.page_copy, 
		reports.cups_user, printers.printer_name, reports.info_str,executors.last_name,executors.first_name,executors.middle_name,document_level.marker
   FROM reports
   INNER JOIN printers ON reports.printer_id = printers.id
   INNER JOIN document_level ON reports.doc_level_id = document_level.id
   INNER JOIN executors ON reports.executor_id = executors.id;
