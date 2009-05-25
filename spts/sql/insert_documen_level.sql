/*
DROP TABLE document_level;

CREATE TABLE document_level
(
  id serial NOT NULL,
  marker varchar(3) NOT NULL,
  sec_level integer NOT NULL,
  ds character varying(25) NOT NULL,
  PRIMARY KEY (id)
);

ALTER TABLE document_level OWNER TO postgres;
COMMENT ON TABLE document_level IS 'Таблица описывающая уровни секретности документов:
Не секретно
Секретно
Совершенно секретно
Особой важности';


insert into document_level (marker,sec_level,ds) values ('НС','0','Не секретно');
insert into document_level (marker,sec_level,ds) values ('С','1','Секретно');
insert into document_level (marker,sec_level,ds) values ('СС','2','Совершенно секретно');
insert into document_level (marker,sec_level,ds) values ('ОВ','3','Особой важности');
*/

select * from document_level