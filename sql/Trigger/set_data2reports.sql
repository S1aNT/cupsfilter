DROP TRIGGER set_data2reports ON cups_data_log;

CREATE TRIGGER set_data2reports
AFTER INSERT ON cups_data_log
    FOR EACH ROW EXECUTE PROCEDURE process_set_data();
