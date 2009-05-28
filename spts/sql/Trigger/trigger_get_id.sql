DROP TRIGGER get_id ON reports_info;

CREATE TRIGGER get_id
AFTER INSERT ON reports_info
    FOR EACH ROW EXECUTE PROCEDURE process_get_id();
