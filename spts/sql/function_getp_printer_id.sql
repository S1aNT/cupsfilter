
DROP  FUNCTION get_printer_id(IN printer_name character varying, IN printer_ip inet) 
CREATE FUNCTION get_printer_id(IN printer_name character varying, IN printer_ip inet) 
	RETURNS integer AS
'select id from printers where printer_name=$1 and ip_adress=$2' 

LANGUAGE 'sql' IMMUTABLE STRICT
  COST 1;
 ALTER FUNCTION get_printer_id(IN character varying, IN inet) OWNER TO postgres;


--insert into printers_mandat (mandat) values ('SYSPROG');
--insert into printers (printer_name,mandat_id,ip_adress) values('PDF',1,'192.168.11.151');
--select get_printer_id('PDF','192.168.11.151');

--select id, (select mandat from printers_mandat where printers_mandat.id = printers.mandat_id) as mandat from printers 
--where printer_name=$1 and ip_adress=$2