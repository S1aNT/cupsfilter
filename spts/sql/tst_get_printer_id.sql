--insert into printers (printer_name,ip_adress,mandat_id) values ('PDF','192.168.11.151',get_mandat_id('SYSPROG'));
select get_printer_id ('PDF','192.168.11.151','SYSPROG');