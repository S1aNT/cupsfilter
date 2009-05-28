--delete from printers;
--delete from printers_mandat;
--select * from printers;
--select set_printer_id ('PDF','192.168.11.151','SYSPROG');

--select * from printers;
--select get_printer_id('HPLaserJet','192.168.11.1','SYSPROG');
	
select set_reports_short_info('192.168.11.1','HPLaserJet','SYSPROG','slant','Как размножаются ежики.doc',6,'ОШИБКА','Ошибка разбора документа.Нет маркеров безопастности');