--DROP VIEW "JobsReport";
CREATE OR REPLACE VIEW "JobsReport" AS 
 SELECT 
  printer_id,
  printers.printer_name,
  firstpages_id,
  copy_number,
  doc_level_id,
  document_level.marker,
  doc_name,
  dt,
  mb_number,
  page_count,
  page_copy,
  status,
  cups_user,
  executor_id,
  executors.fio as Executor_FIO,
  inv_number,
  punkt,
  jobs_id,
  pressman_id,
  --executors.fio as Pressman_FIO,
  info_str
  FROM reports
   INNER JOIN printers ON reports.printer_id = printers.id
   INNER JOIN document_level ON reports.doc_level_id = document_level.id
   INNER JOIN executors ON reports.executor_id = executors.id;
   --INNER JOIN executors ON reports.pressman_id = executors.id;
