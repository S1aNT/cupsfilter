DROP VIEW "JobsReport";
CREATE OR REPLACE VIEW "JobsReport" AS 
SELECT
  printers.printer_name as Printer_name,
  firstpages.fp_filename as FP_path,
  copy_number as CopyNumber,
  document_level.marker as DocMarker,
  doc_name as DocName,
  dt	as DateTime,
  mb_number as MB_Number,
  page_count as PageCount,
  page_copy as PageCopy,
  cups_user as CupsUser,
  executors.fio as Executor_FIO,
  inv_number as InvNumber,
  punkt as Punkt,
  jobs_id as JobsID,
  pressman.fio as Pressman_FIO,
  list_recivers.adress,
  status as Status,
  info_str as InfoStr
  FROM reports
   INNER JOIN printers ON reports.printer_id = printers.id
   INNER JOIN document_level ON reports.doc_level_id = document_level.id
   INNER JOIN executors ON reports.executor_id = executors.id
   INNER JOIN firstpages ON reports.firstpages_id = firstpages.id
   INNER JOIN rel_list_recivers2reports ON rel_list_recivers2reports.reports_id = reports.id
   INNER JOIN list_recivers ON rel_list_recivers2reports.list_recivers_id = list_recivers.id
   INNER JOIN pressman ON reports.pressman_id = pressman.id;