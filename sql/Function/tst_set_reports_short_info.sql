
--delete from printers;
--delete from printers_mandat;
--select * from printers;
--select set_printer_id ('PDF','192.168.11.151','SYSPROG');

--select * from printers;
--select get_printer_id('HPLaserJet','192.168.11.1','SYSPROG');

--select set_reports_short_info('192.168.11.99','HPLaserJet5','SY','guest','Как размножаются ежики.doc',6,'ОШИБКА','Ошибка разбора документа.Нет маркеров безопастности');
--select * from reports


/*
select set_reports_info('192.168.11.99','HPLaserJet5','SYSPROG','guest','Как размножаются ежики.doc',6,
			      'ОШИБКА','Ошибка разбора документа,нет маркеров безопастности',
			      'd/:/test.pdf',9,'СС','мб14/14',99,'Мосенцев','Павел','Владимирович','+7964xxxxxxx');

*/
--select get_doclevel_id('СС');
--select * from document_level;

--select * from debug_log;

--insert into debug_log (info_str) values(E'SQLShell.pm:: When execute query SELECT check_printer_perm(\'PDF\',\'192.168.11.150\',\'SYSPROG\',\'N\'):');
select check_printer_perm('PDF','192.168.11.200','SYSPROG','СС')