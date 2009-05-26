insert into document_level (marker,sec_level,ds) values ('НС','0','Не секретно');
insert into document_level (marker,sec_level,ds) values ('С','1','Секретно');
insert into document_level (marker,sec_level,ds) values ('СС','2','Совершенно секретно');
insert into document_level (marker,sec_level,ds) values ('ОВ','3','Особой важности');

insert into rel_mandat2doc_level(mandat_id,doclevel_id) values(1,1);
insert into rel_mandat2doc_level(mandat_id,doclevel_id) values(1,2);
insert into rel_mandat2doc_level(mandat_id,doclevel_id) values(1,3);
insert into rel_mandat2doc_level(mandat_id,doclevel_id) values(1,4);
insert into rel_mandat2doc_level(mandat_id,doclevel_id) values(2,1);

select * from document_level