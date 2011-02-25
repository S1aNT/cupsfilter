CREATE TABLE add_info (
    id INTEGER primary key autoincrement,
    info_body TEXT
);



CREATE TABLE documents (
    id INTEGER primary key autoincrement,
    stamp TEXT,
    mb TEXT,
    punkt TEXT,
    inv_number TEXT,
    number_ex INTEGER,
    doc_des TEXT,
    print_last_page_stamp boolean,
    print_overside_stamp boolean,
    executor_id INTEGER,
    pressman_id INTEGER
);


CREATE TABLE printers (
    id INTEGER primary key autoincrement,
    printer_name TEXT,
    dpi INTEGER,
    descr TEXT
);


CREATE TABLE rel_addinfo_document (
    add_info_id INTEGER NOT NULL,
    document_id INTEGER NOT NULL
);



CREATE TABLE rel_rec_document (
    reciver_id INTEGER NOT NULL,
    document_id INTEGER NOT NULL
);



CREATE TABLE reports (
    id INTEGER primary key autoincrement,
    document_id INTEGER,
    cups_user TEXT,
    job_id INTEGER,
    doc_name TEXT,
    page_copy INTEGER,
    printer_id INTEGER,
    file_name TEXT,
    dt INTEGER,
    status TEXT,
    print_status,
    cups_error TEXT,
    cups_printer_name TEXT
);



CREATE TABLE resivers_list (
    id INTEGER primary key autoincrement,
    rec_info TEXT(125)
);


