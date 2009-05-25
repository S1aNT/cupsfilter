--DROP FUNCTION check_printer_perm(IN character varying, IN inet,IN character varying,IN character varying);
/*
CREATE FUNCTION check_printer_perm(IN printer_name character varying, 
				   IN printer_ip inet,
				   IN printer_mandat character varying, 
				   IN doc_level character varying) 
RETURNS integer AS
 'select printers.id 
  from printers INNER JOIN 
	printers_mandat ON (printers.mandat_id = printers_mandat.id) INNER JOIN
	rel_mandat2doc_level ON ( rel_mandat2doc_level.mandat_id =printers_mandat.id) INNER JOIN
	document_level ON (document_level.id = rel_mandat2doc_level.doclevel_id)
  where printers.printer_name=$1 and printers.ip_adress=$2 and printers_mandat.mandat=$3 and document_level.marker=$4'

LANGUAGE 'sql' IMMUTABLE STRICT
COST 1;
ALTER FUNCTION check_printer_perm(IN character varying, IN inet,IN character varying,IN character varying) OWNER TO postgres;

*/
select check_printer_perm('PDF','192.168.11.200','SYSPROG','ÑÑ');


