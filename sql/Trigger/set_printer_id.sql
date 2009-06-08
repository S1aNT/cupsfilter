--CREATE PROCEDURAL LANGUAGE plpgsql;

CREATE FUNCTION set_printer_id() RETURNS "trigger"
    AS $$
DECLARE
    pr_id INTEGER;    
begin
	pr_id :=(select printers.id from printers where printers.printer_name=NEW.cups_printer_name);
	if  (pr_id IS NULL)THEN
		-- в моей базе нет такого принтера, но так как на уровень выше была проверка, то запишем в таблицу принтеров и вернем ID
		INSERT INTO printers (printer_name,descr) values (NEW.cups_printer_name,'Принтер был добавлен не через интерфейс CUPS');
		pr_id :=(select printers.id from printers where printers.printer_name=NEW.cups_printer_name);
	END IF;
	    
    UPDATE reports SET printer_id=pr_id where id=NEW.id;
    return new;
end;

$$ LANGUAGE plpgsql;

CREATE TRIGGER tr_set_printer_id AFTER INSERT
    ON reports FOR EACH ROW EXECUTE PROCEDURE set_printer_id();


/*    

CREATE FUNCTION del_product() RETURNS "trigger"
    AS $$
begin
    update vendor set products=products-1 where id=old.vendor_id;
    return old;
end;
$$ LANGUAGE plpgsql;
CREATE TRIGGER tr_del_product AFTER DELETE
    ON product FOR EACH ROW EXECUTE PROCEDURE del_product();

CREATE FUNCTION upd_product() RETURNS "trigger"
    AS $$
begin
    if old.vendor_id<>new.vendor_id then
        update vendor set products=products+1 where id=new.vendor_id;
        update vendor set products=products-1 where id=old.vendor_id;
    end if;
    return new
end;
$$ LANGUAGE plpgsql;
CREATE TRIGGER tr_upd_product AFTER UPDATE 
    ON product FOR EACH ROW EXECUTE PROCEDURE upd_product();
  */  