#include <QDebug>
#include <QPixmap>
#include <QtPlugin>
#include <QPainter>
#include <QFile>
#include <QDir>
#include <QTemporaryFile>
#include <QDataStream>
#include <QPrinter>
#include <QPainter>
#include <QDate>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QStandardItem>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QtAlgorithms>

#include "tmpl_plugin.h"
#include "tech_global.h"
#include "simpleitem.h"


#include <iostream>
using namespace std;

using namespace SafeVirtualPrinter;

Tmpl_plugin::Tmpl_plugin(QObject *parent)
{
    //  Регистрируем типаы
    //qRegisterMetaType<tInfo>("tInfo");
    //qRegisterMetaTypeStreamOperators<tInfo>("tInfo");
    qRegisterMetaType<Templ_info>("Templ_info");
    qRegisterMetaTypeStreamOperators<Templ_info>("Templ_info");

};


// сохранение текущей модели в xml файл
QString Tmpl_plugin::saveModel2Xml()
{
    QString e_msg;
    QString out_file;
    // Формируем имя файла
    out_file = QObject::trUtf8("%1/%2_model_data.xml").arg(spool_dir, current_sid);
    // Запись пользовательских данных в xml файл
    QFile file(out_file);
    if ( file.open(QIODevice::WriteOnly)){
        // open an xml stream writer and write simulation data
        QXmlStreamWriter stream(&file);
        stream.setAutoFormatting(true);
        stream.writeStartDocument();
        stream.writeStartElement("t_date");
        stream.writeAttribute("version", "0.1" );
        stream.writeAttribute("user", QString(getenv("USERNAME")) );
        stream.writeAttribute("when", QDateTime::currentDateTime().toString(Qt::ISODate) );
        //m_scene->writeStream( &stream );
        // Запись модели в файл
        if (doc_model){
            qDebug() << Q_FUNC_INFO << "rowCount" << doc_model->rowCount();
            for (int i=0;i<doc_model->columnCount();i++){
                QStandardItem * header_item = doc_model->horizontalHeaderItem(i);
                QString header = header_item->data(Qt::EditRole).toString().toUpper();
                for (int j=0;j<doc_model->rowCount();j++){
                    QStandardItem * cell_item = doc_model->item(j, i);
                    QString c_data = cell_item->data(Qt::EditRole).toString();
                    stream.writeEmptyElement("model_elem");
                    stream.writeAttribute("header",header);
                    stream.writeAttribute("c_data",c_data);
                }
            }
        }
        stream.writeEndDocument();
        // close the file
        file.close();
    }else{
        e_msg = tr("Ошибка создания файла %1").arg(out_file);
    }
    if (!e_msg.isEmpty()) {
        emit error(e_msg);
    }
    return out_file;
}
//Загрузка модели из xml файла
void Tmpl_plugin::loadModel4Xml(const QString &in_file)
{
    QString e_msg;
    QFile file( in_file );
    if ( file.open( QIODevice::ReadOnly ) ){
        QXmlStreamReader  stream( &file );
        doc_model->clear();
        while ( !stream.atEnd() ){
            stream.readNext();
            if ( stream.isStartElement() ) {
                if ( stream.name() == "model_elem" ){
                    // Читаем элемент


                }else{
                    stream.raiseError( QString("Unrecognised element '%1'").arg(stream.name().toString()) );
                }
            }
        }
        file.close();

        // check if error occured
        if ( stream.hasError() ){
            e_msg = QString("Ошибка загрузки '%1' (%2)").arg(in_file).arg(stream.errorString()) ;
        }
    }else{
        e_msg = tr("Ошибка открытия файла с данными %1").arg(in_file);
    }
    if (!e_msg.isEmpty()) {
        emit error(e_msg);
    }
}

void Tmpl_plugin::init(const QString &spool,const QString &sid)
{
    QString e_msg;
    QDir dir;

    if (!sid.isEmpty()) {
        current_sid = sid;// Запомним уникальный номер

        if (dir.cd(spool) && !spool.isEmpty()) {
            // Проверим факт существования временного каталога
            spool_dir = spool;
            // Формируем пути для файлов
            firstPage_tmpl_fn = QObject::trUtf8("%1/%2_first_tmpl.pdf").arg(spool, sid);
            secondPage_tmpl_fn = QObject::trUtf8("%1/%2_second_tmpl.pdf").arg(spool, sid);
            thirdPage_tmpl_fn = QObject::trUtf8("%1/%2_third_tmpl.pdf").arg(spool, sid);
            fourthPage_tmpl_fn = QObject::trUtf8("%1/%2_fourth_tmpl.pdf").arg(spool, sid);
            // создаем сцены
            firstPage_scene  = new QGraphicsScene(this);
            secondPage_scene = new QGraphicsScene(this);
            thirdPage_scene  = new QGraphicsScene(this);
            fourthPage_scene = new QGraphicsScene(this);
            // Заполним список базовых элементов шаблона
            fillElemMap();
            // Создаем списки
            elem_name_QSL = QStringList(elem_name_map.keys());

            // Заполним список размеров страниц
            fillPageMap();
            // Создаем списки
            page_name_QSL = QStringList(page_size_map.keys());

            //page_marker = "templates_page"; // маркер страницы
            // Создаем QMap размеров страниц
            // Заполним описание шаблона версией шаблона
            //templ_info = new Templ_info();
            templ_info.setT_ver(t_version);
            // Создадим модель данных для шаблона
            createModel();
        }else{
            e_msg = QObject::trUtf8("ERROR: каталог %1 не существует\n").arg(spool);
        }

    }else{
        e_msg = QObject::trUtf8("ERROR: Неверный SID для документа\n").arg(sid);
    }
    if (!e_msg.isEmpty()) {
        emit error(e_msg);
    }
}
/*
bool Tmpl_plugin::getPageOrientation()
{
    return templ_info.page_orient();
}

*/

void Tmpl_plugin::setPageOrientation(bool p_orient)
{
    templ_info.setPage_orient(p_orient);
    int p_s_id = this->getElemIdByName(templ_info.p_size());
    // Заполним размеры страницы зная только строковый индетиф. размера
    qreal w = this->findPageSize_W(p_s_id); // ширина листа
    qreal h = this->findPageSize_H(p_s_id); // высота листа
    // Теперь повернем страницу согласно ориентации
    if (!templ_info.page_orient()){
        qSwap(w,h);
    }
    templ_info.setPage_width(w);
    templ_info.setPage_height(h);
    // теперь каждой странице изменим размеры
    create_page(firstPage_scene,
                                      templ_info.page_width(),templ_info.page_height(),
                                      templ_info.m_top(),templ_info.m_bottom(),
                                      templ_info.m_right(),templ_info.m_left()
                                      );
}

QStringList Tmpl_plugin::getPageSizeList()
{
    return page_name_QSL;
}

QStringList Tmpl_plugin::getElemNameList()
{
    return elem_name_QSL;
}

void Tmpl_plugin::loadTemplates(const QString & templates_in_file)
{
    QString e_msg;
    if (QFile::exists(templates_in_file)){
        if (parse_templates(templates_in_file)){
            emit allTemplatesPagesParsed(firstPage_scene, secondPage_scene, thirdPage_scene, fourthPage_scene);
        }else{
            e_msg = QObject::trUtf8("ERROR: Ошибка разбора шаблона [%1]\n").arg(templates_in_file);
        }
    }else {
        e_msg = QObject::trUtf8("Ошибка: Файл %1 шаблона не существует или не не верного формата!");
    }
    if (!e_msg.isEmpty()) {
        emit error(e_msg);
    }

}

void Tmpl_plugin::createEmptyTemplate(const QString & file_name)
{
    QString e_msg;

    // Проверка что функция была вызвана верно, т.е вначале были
    // заполненны данные о шаблоне
    if (templ_info.t_ver() == t_version){
        const QString startnow = QDir::currentPath();
        // Создаем пустой шаблон документа

        if (QFile::exists(file_name)){
            QFile::remove(file_name);
        }
        QFile new_tmpl_file(file_name);
        new_tmpl_file.open(QIODevice::WriteOnly);
        QDataStream out(&new_tmpl_file);
        out.setVersion(QDataStream::Qt_4_5);

        // Основные данные уже переданны в плагин вызовом setTemplInfo(tInfo)
        int p_s_id = this->getElemIdByName(templ_info.p_size());
        // Заполним размеры страницы зная только строковый индетиф. размера
        qreal w = this->findPageSize_W(p_s_id); // ширина листа
        qreal h = this->findPageSize_H(p_s_id); // высота листа
        // Теперь повернем страницу согласно ориентации
        if (!templ_info.page_orient()){
           qSwap(w,h);
        }
        templ_info.setPage_width(w);
        templ_info.setPage_height(h);

        templ_info.setFirstPageElemCount(0);  // первая страница шаблона 0 элементов
        templ_info.setSecondPageElemCount(0); // вторая страница шаблона 0 элементов
        templ_info.setThirdPageElemCount (0);  // третья страница шаблона 0 элементов
        templ_info.setFourthPageElemCount (0); // четвертая страница шаблона 0 элементов

        // Запишем общую часть шаблона
        out << templ_info;
        // Начнем сохранение страниц
        out << page_marker;
        out << templ_info.firstPageElemCount();  // первая страница шаблона 0 элементов
        out << page_marker;
        out << templ_info.secondPageElemCount(); // вторая страница шаблона 0 элементов
        out << page_marker;
        out << templ_info.thirdPageElemCount();  // третья страница шаблона 0 элементов
        out << page_marker;
        out << templ_info.fourthPageElemCount(); // четвертая страница шаблона 0 элементов

        new_tmpl_file.close();
        emit emptyTemplateCreate(file_name);

    }else{
        e_msg = QObject::trUtf8("ERROR: Ошибка создания пустого шаблона [%1]\n").arg(file_name);
    }
    if (!e_msg.isEmpty()) {
        emit error(e_msg);
    }


}

void Tmpl_plugin::createEmptyTemplate(const QString & file_name,
                                      const QString & t_author,
                                      const QString & t_name,
                                      const QString & t_desc,
                                      const QString & p_size,

                                      bool  pages_orient,
                                      const QString & c_date,
                                      qreal m_top,
                                      qreal m_bottom,
                                      qreal m_right,
                                      qreal m_left)
{
    QString e_msg;
    const QString startnow = QDir::currentPath();
    // Создаем пустой шаблон документа

    if (QFile::exists(file_name)){
        QFile::remove(file_name);
    }
    QFile new_tmpl_file(file_name);
    new_tmpl_file.open(QIODevice::WriteOnly);
    QDataStream out(&new_tmpl_file);
    out.setVersion(QDataStream::Qt_4_5);

    templates_file_name = file_name;
    int p_s_id = this->getElemIdByName(p_size);

    // Создаем общую часть шаблона
    templ_info.setT_ver(t_version);

    templ_info.setT_author(t_author);    // автор шаблона
    templ_info.setT_name(t_name);      // название шаблона, то что покажем в списке шаблонов
    templ_info.setT_desc(t_desc);      // описание шаблона, может быть пустым
    templ_info.setP_size(p_size);      // размер бумаги

    templ_info.setPage_width(this->findPageSize_W(p_s_id));     // ширина листа
    templ_info.setPage_height(this->findPageSize_H(p_s_id));    // высота листа

    templ_info.setPage_orient(pages_orient);    // ориентация листа
    templ_info.setDate_time(c_date); // дата и время создания шаблона

    templ_info.setM_left(m_left);      // отступ слева
    templ_info.setM_right(m_right);     // отступ справа
    templ_info.setM_top(m_top);       // отступ сверху
    templ_info.setM_bottom(m_bottom);    // отступ снизу

    templ_info.setFirstPageElemCount(0);  // первая страница шаблона 0 элементов
    templ_info.setSecondPageElemCount(0); // вторая страница шаблона 0 элементов
    templ_info.setThirdPageElemCount (0);  // третья страница шаблона 0 элементов
    templ_info.setFourthPageElemCount (0); // четвертая страница шаблона 0 элементов
    // Запишем общую часть шаблона
    out << templ_info;
    /*
    // Создаем общую часть шаблона
    t_info.version = t_version;
    t_info.t_author = t_author;    // автор шаблона
    t_info.t_name = t_name;      // название шаблона, то что покажем в списке шаблонов
    t_info.t_desc = t_desc;      // описание шаблона, может быть пустым
    t_info.p_size = p_size;      // размер бумаги

    t_info.page_width = this->findPageSize_W(p_s_id);     // ширина листа
    t_info.page_height = this->findPageSize_H(p_s_id);    // высота листа

    t_info.page_orient = pages_orient;    // ориентация листа
    t_info.date_time = c_date; // дата и время создания шаблона

    t_info.m_left = m_left;      // отступ слева
    t_info.m_right = m_right;     // отступ справа
    t_info.m_top = m_top;       // отступ сверху
    t_info.m_bottom = m_bottom;    // отступ снизу

    t_info.firstPageElemCount = 0;  // первая страница шаблона 0 элементов
    t_info.secondPageElemCount = 0; // вторая страница шаблона 0 элементов
    t_info.thirdPageElemCount = 0;  // третья страница шаблона 0 элементов
    t_info.fourthPageElemCount = 0; // четвертая страница шаблона 0 элементов
    // Запишем общую часть шаблона
    out << t_info;
*/


    // Начнем сохранение страниц
    out << page_marker;
    out << templ_info.firstPageElemCount();  // первая страница шаблона 0 элементов
    out << page_marker;
    out << templ_info.secondPageElemCount(); // вторая страница шаблона 0 элементов
    out << page_marker;
    out << templ_info.thirdPageElemCount();  // третья страница шаблона 0 элементов
    out << page_marker;
    out << templ_info.fourthPageElemCount(); // четвертая страница шаблона 0 элементов

    new_tmpl_file.close();
    emit emptyTemplateCreate(file_name);

    if (!e_msg.isEmpty()) {
        emit error(e_msg);
    }
}

void Tmpl_plugin::doAddImgElementToPage(int page,QString &file_img)
{
    QString e_msg;
    QGraphicsScene *scene;
    QGraphicsItem *item;
    QGraphicsPixmapItem * imgItem;
    QPixmap pixMapItem;
    switch(page){
    case 1:
        scene = firstPage_scene;
        break;
    case 2:
        scene = secondPage_scene;
        break;
    case 3:
        scene = thirdPage_scene;
        break;
    case 4:
        scene = fourthPage_scene;
        break;
    default:
        e_msg = QObject::trUtf8("Ошибка: Такой страницы %2 в шаблоне не существует")
                .arg(page,0,10);
        emit error(e_msg);
        //emit toLog(l_msg+e_msg);
        break;
    }
    if (scene){
        item = findPaperElem(scene);
        if (QFile::exists(file_img)){

            if (pixMapItem.load(file_img)){
                imgItem = new QGraphicsPixmapItem();
                imgItem->setData(ObjectName, "tPixMap");
                imgItem->setFlag(QGraphicsItem::ItemIsMovable);
                if( pixMapItem.width() >320 ||
                    pixMapItem.height() >320){
                    imgItem->setPixmap(pixMapItem.scaled(QSize(320,320)));
                }else{
                    imgItem->setPixmap(pixMapItem);
                }
                imgItem->setParentItem(item);
                scene->update();
            }else{
                e_msg = QObject::trUtf8("Ошибка: Формат файла %1 не известен!").arg(file_img);
            }

        }else{
            e_msg = QObject::trUtf8("Ошибка: Файл %1 изображения не существует!").arg(file_img);
        }

    }
    if (!e_msg.isEmpty()) {
        emit error(e_msg);
    }

}
void Tmpl_plugin::doAddBaseElementToPage(int page,QStringList &text_list)
{
    QString e_msg;
    QString l_msg = QString(" [%1] ").arg(QString::fromAscii(Q_FUNC_INFO));
    QGraphicsScene *scene;
    QGraphicsItem *item;

    switch(page){
    case 1:
        scene = firstPage_scene;
        break;
    case 2:
        scene = secondPage_scene;
        break;
    case 3:
        scene = thirdPage_scene;
        break;
    case 4:
        scene = fourthPage_scene;
        break;
    default:
        e_msg = QObject::trUtf8("Ошибка: Такой страницы %2 в шаблоне не существует")
                .arg(page,0,10);
        emit error(e_msg);
        emit toLog(l_msg+e_msg);
        break;
    }
    if (scene){
        item = findPaperElem(scene);

        SimpleItem * pItem = new SimpleItem();
        pItem->setZValue(100);
        pItem->setPos(100.0,100.0);
        //pItem->setText(QStringList()<<QObject::trUtf8("Элемент"));
        pItem->setText(text_list);
        pItem->setFlag(QGraphicsItem::ItemIsMovable);
        pItem->setData(ObjectName, "tElem");
        pItem->setParentItem(item);
        scene->update();
    }
}
void Tmpl_plugin::setTemplates(const QString & templates_in_file)
{
    QString e_msg;
    if (QFile::exists(templates_in_file)) {
        if (doc_model){
            if (QFile::exists(firstPage_tmpl_fn)){
                QFile::remove(firstPage_tmpl_fn);
            }
            if (QFile::exists(secondPage_tmpl_fn)){
                QFile::remove(secondPage_tmpl_fn);
            }
            if (QFile::exists(thirdPage_tmpl_fn)){
                QFile::remove(thirdPage_tmpl_fn);
            }
            if (QFile::exists(fourthPage_tmpl_fn )){
                QFile::remove(fourthPage_tmpl_fn );
            }

            if (parse_templates(templates_in_file)){
                emit allTemplatesPagesParsed(firstPage_scene, secondPage_scene, thirdPage_scene, fourthPage_scene);
            }else{
                e_msg = QObject::trUtf8("ERROR: Ошибка разбора шаблона [%1]\n").arg(templates_in_file);
            }
        }else{
            e_msg = QObject::trUtf8("ERROR: Модель [карточки документа] не существует\n");
        }
    }else{
        e_msg = QObject::trUtf8("Файл [%1] шаблона не найден.").arg(templates_in_file);
    }
    if (!e_msg.isEmpty()) {
        emit error(e_msg);
    }
}

void Tmpl_plugin::saveTemplatesAs(const QString & save_file)
{
    QString e_msg;

    if (QFile::exists(save_file)){
        QFile::rename(save_file,save_file+".bak");
    }
    //Сцены уже показы пользователю их и сохраняем
    if (firstPage_scene && secondPage_scene &&
        thirdPage_scene && fourthPage_scene){
        // TODO Обновим время модификации шаблона
        // Обновим данные о количестве страниц
        templ_info.setFirstPageElemCount(getElemCount(firstPage_scene));
        templ_info.setSecondPageElemCount(getElemCount(secondPage_scene));
        templ_info.setThirdPageElemCount (getElemCount(thirdPage_scene));
        templ_info.setFourthPageElemCount (getElemCount(fourthPage_scene));
        // запись основных данных шаблона
        QFile new_tmpl_file(save_file);
        new_tmpl_file.open(QIODevice::WriteOnly);

        QDataStream out(&new_tmpl_file);
        out.setVersion(QDataStream::Qt_4_5);

        // запись основных данных шаблона
        out << templ_info;
        //Запись маркера начало страницы
        out << page_marker;
        out << templ_info.firstPageElemCount();
        // первая страница шаблона элементов
        out << firstPage_scene;

        out << page_marker;
        out << templ_info.secondPageElemCount();
        // вторая страница шаблона элементов
        out << secondPage_scene;

        out << page_marker;
        out << templ_info.thirdPageElemCount();
        // третья страница шаблона элементов
        out << thirdPage_scene;

        out << page_marker;
        out << templ_info.fourthPageElemCount();
        // четвертая страница шаблона элементов
        out << fourthPage_scene;
        new_tmpl_file.close();
    }

    if (!e_msg.isEmpty()){
        emit error(e_msg);
    }
}
//----------------------- public slots -----------------------------------------
void Tmpl_plugin::doSaveTemplates()
{
    // Сохраняется текущий рабочий шаблон,
    // т.е тот который был выбран и показан пользователю
    QString e_msg;
    QGraphicsItem *item;
    SimpleItem* tElem;
    QString elem_type;

    if (QFile::exists(templates_file_name)){
        QFile::rename(templates_file_name,templates_file_name+".bak");
        // запись основных данных шаблона
        QFile new_tmpl_file(templates_file_name);
        new_tmpl_file.open(QIODevice::WriteOnly);

        QDataStream out(&new_tmpl_file);
        out.setVersion(QDataStream::Qt_4_5);

        // Обновим время модификации шаблона
        templ_info.setDate_time(QDateTime::currentDateTime()
                                .toString("dd.MM.yyyy hh:mm:ss"));
        // Обновим данные о количестве страниц
        templ_info.setFirstPageElemCount(getElemCount(firstPage_scene));
        templ_info.setSecondPageElemCount(getElemCount(secondPage_scene));
        templ_info.setThirdPageElemCount (getElemCount(thirdPage_scene));
        templ_info.setFourthPageElemCount (getElemCount(fourthPage_scene));
        // запись основных данных шаблона

        out << templ_info;

        //Запись маркера начало страницы
        out << page_marker;
        out << templ_info.firstPageElemCount(); // первая страница шаблона элементов

        for (int i = 0; i < firstPage_scene->items().size(); ++i){
            item = firstPage_scene->items().at(i);
            elem_type=item->data(ObjectName).toString();
            out << elem_type; // Сохраним тип элемента
            if (elem_type == "tElem"){
                tElem =(SimpleItem* )firstPage_scene->items().at(i);
                out << tElem->pos() << tElem->getFont() << tElem->getColor()
                        << tElem->getText();
            }
        }

        out << page_marker; // вторая страница шаблона элементов
        out << templ_info.secondPageElemCount();
        for (int i = 0; i < secondPage_scene->items().size(); ++i){
            item = secondPage_scene->items().at(i);
            elem_type=item->data(ObjectName).toString();
            out << elem_type; // Сохраним тип элемента
            if (elem_type == "tElem"){
                tElem =(SimpleItem* )secondPage_scene->items().at(i);
                out << tElem->pos() << tElem->getFont() << tElem->getColor()
                        << tElem->getText();
            }
        }

        out << page_marker; // третья страница шаблона элементов
        out << templ_info.thirdPageElemCount();
        for (int i = 0; i < thirdPage_scene->items().size(); ++i){
            item = thirdPage_scene->items().at(i);
            elem_type=item->data(ObjectName).toString();
            out << elem_type; // Сохраним тип элемента
            if (elem_type == "tElem"){
                tElem =(SimpleItem* )thirdPage_scene->items().at(i);
                out << tElem->pos() << tElem->getFont() << tElem->getColor()
                        << tElem->getText();
            }
        }

        out << page_marker; // четвертая страница шаблона элементов
        out << templ_info.fourthPageElemCount();
        for (int i = 0; i < fourthPage_scene->items().size(); ++i){
            item = fourthPage_scene->items().at(i);
            elem_type=item->data(ObjectName).toString();
            out << elem_type; // Сохраним тип элемента
            if (elem_type == "tElem"){
                tElem =(SimpleItem* )fourthPage_scene->items().at(i);
                out << tElem->pos() << tElem->getFont() << tElem->getColor()
                        << tElem->getText();
            }
        }

        new_tmpl_file.close();
    }
    if (!e_msg.isEmpty()){
        emit error(e_msg);
    }
}

//******************************************************************************
void Tmpl_plugin::convertTemplatesToPdf(const QString & templates_in_file)
{
    QString e_msg;
    if (QFile::exists(templates_in_file)) {
            if (parse_templates(templates_in_file)){
                // создаем pdf файлы
                // FIXME hardcore!!!!
                printFormatingPageToFile(1);
                printFormatingPageToFile(2);
                printFormatingPageToFile(3);
                printFormatingPageToFile(4);

                emit allPagesConverted(firstPage_tmpl_fn,secondPage_tmpl_fn,
                                       thirdPage_tmpl_fn,fourthPage_tmpl_fn);
            }else{
                e_msg = QObject::trUtf8("ERROR: Ошибка разбора шаблона [%1]\n").arg(templates_in_file);
            }
    }else{
        e_msg = QObject::trUtf8("Файл [%1] шаблона не найден.").arg(templates_in_file);
    }
    if (!e_msg.isEmpty()) {
        emit error(e_msg);
    }
}


void Tmpl_plugin::printFormatingPageToFile(int pageNum)
{
    QString e_msg;
    QGraphicsScene *scene;

    // Печатает выбранную страницу текущего шаблона в pdf файл
    // страница формируется исходя из данных модели
    if (!templates_file_name.isEmpty() && pageNum <= 4 && pageNum >=1){
        QPrinter pdfprinter;
        if (templ_info.page_orient()){
            pdfprinter.setOrientation(QPrinter::Portrait);
        }else{
            pdfprinter.setOrientation(QPrinter::Landscape);
        }
        pdfprinter.setOutputFormat(QPrinter::PdfFormat);
        //FIXME:
        // пройтись по всем элементам сцены и удалить границу
        // сцена к этому моменту уже создана и заполнена элементами
        switch(pageNum){
        case 1:
            scene = firstPage_scene;
            update_scene(scene);
            if (QFile::exists(firstPage_tmpl_fn)){
                QFile::remove(firstPage_tmpl_fn);
            }

            pdfprinter.setOutputFileName(firstPage_tmpl_fn);
            break;
        case 2:
            scene = secondPage_scene;
            update_scene(scene);
            if (QFile::exists(secondPage_tmpl_fn)){
                QFile::remove(secondPage_tmpl_fn);
            }

            pdfprinter.setOutputFileName(secondPage_tmpl_fn);

            break;
        case 3:
            scene = thirdPage_scene;
            update_scene(scene);
            if (QFile::exists(thirdPage_tmpl_fn)){
                QFile::remove(thirdPage_tmpl_fn);
            }

            pdfprinter.setOutputFileName(thirdPage_tmpl_fn);
            break;
        case 4:
            scene = fourthPage_scene;
            update_scene(scene);
            if (QFile::exists(fourthPage_tmpl_fn)){
                QFile::remove(fourthPage_tmpl_fn);
            }

            pdfprinter.setOutputFileName(fourthPage_tmpl_fn);
            break;
        }
        QPainter painter(&pdfprinter);
        scene->render(&painter);
    }
}

void Tmpl_plugin::create_page(QGraphicsScene * scene,
                              qreal width,qreal height,
                              qreal m_top,qreal m_bottom,
                              qreal m_right,qreal m_left)
{
    if (scene){
        scene->setSceneRect(0, 0, width,height);
        scene->setBackgroundBrush(Qt::white);
        // рисуем границы (при печати надо их убирать)

        QGraphicsRectItem *paper_rect =
                new QGraphicsRectItem (QRectF(0,0, width,height));
        paper_rect->setPen(QPen(Qt::black));
        paper_rect->setBrush(QBrush(Qt::white));
        paper_rect->setZValue(-1000.0);
        paper_rect->setData(ObjectName, "Paper");
        scene->addItem(paper_rect);

        QGraphicsRectItem *border_rect =
                new QGraphicsRectItem (
                        QRectF(m_left, m_top,width-m_left-m_right,height-m_top-m_bottom)
                        );

        border_rect->setPen(QPen(Qt::black,2,Qt::DotLine));
        border_rect->setBrush(QBrush(Qt::white));
        border_rect->setOpacity(1);
        border_rect->setZValue(-900);
        border_rect->setData(ObjectName, "Border");
        border_rect->setParentItem(paper_rect);
    }
}

void Tmpl_plugin::createModel()
{
   doc_model     = new QStandardItemModel(this);
   doc_model->setHorizontalHeaderLabels(elem_name_QSL);
   doc_model->insertRow(doc_model->rowCount());
   qDebug() << Q_FUNC_INFO
            << "doc_model->rowCount" << doc_model->rowCount()
            << "doc_model->colCount" << doc_model->columnCount();
}

bool Tmpl_plugin::parse_templates(const QString & in_file)
{
    bool flag;

    QString e_msg;

    QString elemType;
    QPointF ps;
    QFont fnt;
    QColor col;
    QStringList pList;
    QStringList filledList;
    int page_count_elem;
    QString marker;
    QGraphicsItem  * parent;

    //TODO Можно объединить в одну проверку QFile::exists
    if (!in_file.isEmpty()){
        if (QFile::exists(in_file)){

            firstPage_scene->clear();
            secondPage_scene->clear();
            thirdPage_scene->clear();
            fourthPage_scene->clear();
            //t_info.
            // Сохраним текущее состояние модели в файл xml

            // сохраним имя текущего файла шаблона
            templates_file_name = in_file;
            QFile file(in_file);
            file.open(QIODevice::ReadOnly);

            QDataStream in(&file);
            in.setVersion(QDataStream::Qt_4_5);

            // Читаем общую часть шаблона
            in >> templ_info;

            if (templ_info.t_ver() == t_version){// Сравним версию шаблона
                // создаем основное рабочее поле
                this->create_page(firstPage_scene,
                                  templ_info.page_width(),templ_info.page_height(),
                                  templ_info.m_top(),templ_info.m_bottom(),
                                  templ_info.m_right(),templ_info.m_left()
                                  );
                this->create_page(secondPage_scene,
                                  templ_info.page_width(),templ_info.page_height(),
                                  templ_info.m_top(),templ_info.m_bottom(),
                                  templ_info.m_right(),templ_info.m_left()
                                  );
                this->create_page(thirdPage_scene,
                                  templ_info.page_width(),templ_info.page_height(),
                                  templ_info.m_top(),templ_info.m_bottom(),
                                  templ_info.m_right(),templ_info.m_left()
                                  );
                this->create_page(fourthPage_scene,
                                  templ_info.page_width(),templ_info.page_height(),
                                  templ_info.m_top(),templ_info.m_bottom(),
                                  templ_info.m_right(),templ_info.m_left()
                                  );


                in >> marker;
                if (marker == page_marker){
                    // Не сбились с позиционированием
                    in >> page_count_elem;
                    if (page_count_elem == templ_info.firstPageElemCount()){
                        // Поиск предка
                        parent = this->findPaperElem(firstPage_scene);

                        // число элементов на первой странице совпадает с заголовком
                        for (int i=0;i<templ_info.firstPageElemCount();i++){
                            // перебор всех элементов страницы
                            in >> elemType;
                            if (elemType=="tElem"){
                                in >>ps >>fnt >> col >>pList;
                                this->create_SimpleItem(parent,ps,fnt,col,pList);
                            }
                        }
                    }else{
                        e_msg = QObject::trUtf8("Ошибка:Кол-во элементов [%1] в заголовке первой страницы не совпадает с записанным в блоке %2")
                                .arg(templ_info.firstPageElemCount(),0,10)
                                .arg(page_count_elem);
                        emit error(e_msg);
                        return false;
                    }
                }else{
                    e_msg = QObject::trUtf8("Ошибка:Не найден маркер начала страницы");
                    emit error(e_msg);
                    return false;
                }

                in >> marker;
                if (marker == page_marker){
                    // Не сбились с позиционированием
                    in >> page_count_elem;
                    if (page_count_elem == templ_info.secondPageElemCount()){
                        // Поиск предка
                        parent = this->findPaperElem(secondPage_scene);

                        // число элементов на 2 странице совпадает с заголовком
                        for (int i=0;i<templ_info.secondPageElemCount();i++){
                            // перебор всех элементов страницы
                            in >> elemType;
                            if (elemType=="tElem"){
                                in >>ps >>fnt >> col >>pList;
                                this->create_SimpleItem(parent,ps,fnt,col,pList);
                            }
                        }

                    }else{
                        e_msg = QObject::trUtf8("Ошибка:Кол-во элементов [%1] в заголовке второй страницы не совпадает с записанным в блоке %2")
                                .arg(templ_info.secondPageElemCount(),0,10)
                                .arg(page_count_elem);
                        emit error(e_msg);
                        return false;
                    }
                }else{
                    e_msg = QObject::trUtf8("Ошибка:Не найден маркер начала страницы");
                    emit error(e_msg);
                    return false;
                }

                in >> marker;
                if (marker == page_marker){
                    // Не сбились с позиционированием
                    in >> page_count_elem;
                    if (page_count_elem == templ_info.thirdPageElemCount()){
                        // Поиск предка
                        parent = this->findPaperElem(thirdPage_scene);

                        // число элементов на 3 странице совпадает с заголовком
                        for (int i=0;i<templ_info.thirdPageElemCount();i++){
                            // перебор всех элементов страницы
                            in >> elemType;
                            if (elemType=="tElem"){
                                in >>ps >>fnt >> col >>pList;
                                this->create_SimpleItem(parent,ps,fnt,col,pList);
                            }
                        }

                    }else{
                        e_msg = QObject::trUtf8("Ошибка:Кол-во элементов [%1] в заголовке третьей страницы не совпадает с записанным в блоке %2")
                                .arg(templ_info.thirdPageElemCount(),0,10)
                                .arg(page_count_elem);
                        emit error(e_msg);
                        return false;
                    }
                }else{
                    e_msg = QObject::trUtf8("Ошибка:Не найден маркер начала страницы");
                    emit error(e_msg);
                    return false;
                }
                in >> marker;
                if (marker == page_marker){
                    // Не сбились с позиционированием
                    in >> page_count_elem;
                    if (page_count_elem == templ_info.fourthPageElemCount()){
                        // Поиск предка
                        parent = this->findPaperElem(fourthPage_scene);

                        // число элементов на 4 странице совпадает с заголовком
                        for (int i=0;i<templ_info.fourthPageElemCount();i++){
                            // перебор всех элементов страницы
                            in >> elemType;
                            if (elemType=="tElem"){
                                in >>ps >>fnt >> col >>pList;
                                this->create_SimpleItem(parent,ps,fnt,col,pList);
                            }
                        }

                    }else{
                        e_msg = QObject::trUtf8("Ошибка:Кол-во элементов [%1] в заголовке четвертой страницы не совпадает с записанным в блоке %2")
                                .arg(templ_info.fourthPageElemCount(),0,10)
                                .arg(page_count_elem);
                        emit error(e_msg);
                        return false;
                    }
                }else{
                    e_msg = QObject::trUtf8("Ошибка:Не найден маркер начала страницы");
                    emit error(e_msg);
                    return false;
                }
                file.close();

                firstPage_scene->update();
                secondPage_scene->update();
                thirdPage_scene->update();
                fourthPage_scene->update();

                flag = true;
            }else {
                flag =  false; // Мой плагин работает с шаблоном только версии <= version
            }

        }
    }
    return flag;
}

QString Tmpl_plugin::findFromModel(const QString &find_line)
{
    QString local_find;
    QString find_rep;
    local_find = find_line.toUpper();
    find_rep = find_line;

    if (find_line.contains("[") && find_line.contains("]")){

        local_find.replace("[","").replace("]","");

        qDebug() << Q_FUNC_INFO << "local_find" << local_find;
        if (doc_model){
            qDebug() << Q_FUNC_INFO << "rowCount" << doc_model->rowCount();
            for (int i=0;i<doc_model->columnCount();i++){
                QStandardItem * header_item = doc_model->horizontalHeaderItem(i);
                QString header = header_item->data(Qt::EditRole).toString().toUpper();
                if (header.compare(local_find) == 0){
                    // В модели всегда две строчки заголовок и данные,работаю со второй строчкой
                    //QStandardItem * cell_item = doc_model->item(doc_model->rowCount(), i);
                    QStandardItem * cell_item = doc_model->item(0, i);
                    if (cell_item){
                        qDebug() << "cell " << cell_item->data(Qt::EditRole).toString()
                                   << "find_rep "<< find_rep;
                        find_rep.replace(QRegExp("\\[(.+)\\]"),cell_item->data(Qt::EditRole).toString() );
                        qDebug() << Q_FUNC_INFO << find_rep;
                    }

                    break;
                }
            }
        }
        // Теперь надо заменить [**] на  найденное значение


    }
    return find_rep;
}


QSize Tmpl_plugin::getPageSizeFromString(QString & page_str)
{
    QSize p_size;
    switch (getElemIdByName(page_str)){
    case QPrinter::A4:
        p_size = QSize(210,297);
        break;
    case QPrinter::A3:
        p_size = QSize(420,297);
        break;
        //FixMe

    }

    return p_size;
}

int Tmpl_plugin::getElemIdByName(const QString &elem_name)
{
    int result=0; // Если в списке запрошенного элемента нет то возвращаем 0
    if (!elem_name.isEmpty() && page_size_map.contains(elem_name)){
        result= page_size_map.value(elem_name);
    }
    return result;
}

// Всегда возвращает для портетной ориентации

qreal Tmpl_plugin::findPageSize_W(int page_size_id)
{
    qreal res;
    switch (page_size_id){
    case QPrinter::A4:
        res=MM_TO_POINT(210);
        break;
    case QPrinter::A3:
        res=MM_TO_POINT(297);
        break;
    case QPrinter::A0:
        res = MM_TO_POINT(841);
        break;
    case QPrinter::A1:
        res = MM_TO_POINT(594);
        break;
    case QPrinter::A2:
        res = MM_TO_POINT(420);
        break;
    case QPrinter::A5:
        res = MM_TO_POINT(148);
        break;
    case QPrinter::A6:
        res = MM_TO_POINT(105);
        break;
    case QPrinter::A7:
        res = MM_TO_POINT(74);
        break;
    case QPrinter::A8:
        res = MM_TO_POINT(52);
        break;
    case QPrinter::A9:
        res = MM_TO_POINT(37);
        break;
    case QPrinter::B0:
        res = MM_TO_POINT(1000);
        break;
    case QPrinter::B1:
        res = MM_TO_POINT(707);
        break;
    case QPrinter::B2:
        res = MM_TO_POINT(500);
        break;
    case QPrinter::B3:
        res = MM_TO_POINT(353);
        break;
    case QPrinter::B4:
        res = MM_TO_POINT(250);
        break;
    case QPrinter::B5:
        res = MM_TO_POINT(176);
        break;
    case QPrinter::B6:
        res = MM_TO_POINT(125);
        break;
    case QPrinter::B7:
        res = MM_TO_POINT(88);
        break;
    case QPrinter::B8:
        res = MM_TO_POINT(62);
        break;
    case QPrinter::B9:
        res = MM_TO_POINT(44);
        break;
    case QPrinter::B10:
        res = MM_TO_POINT(31);
        break;
    case QPrinter::C5E:
        res = MM_TO_POINT(163);
        break;
    case QPrinter::DLE:
        res = MM_TO_POINT(110);
        break;
    case QPrinter::Executive:
        res = MM_TO_POINT(191);
        break;
    case QPrinter::Folio:
        res = MM_TO_POINT(210);
        break;
    case QPrinter::Ledger:
        res = MM_TO_POINT(432);
        break;
    case QPrinter::Legal:
        res = MM_TO_POINT(216);
        break;
    case QPrinter::Letter:
        res = MM_TO_POINT(216);
        break;
    case QPrinter::Tabloid:
        res = MM_TO_POINT(279);
        break;

    }
    return res;
}

qreal Tmpl_plugin::findPageSize_H(int page_size_id)
{
    qreal res;
    switch (page_size_id){
    case QPrinter::A4:
        res=MM_TO_POINT(297);
        break;
    case QPrinter::A3:
        res=MM_TO_POINT(420);
        break;
    case QPrinter::A0:
        res = MM_TO_POINT(1189);
        break;
    case QPrinter::A1:
        res = MM_TO_POINT(841);
        break;
    case QPrinter::A2:
        res = MM_TO_POINT(594);
        break;
    case QPrinter::A5:
        res = MM_TO_POINT(210);
        break;
    case QPrinter::A6:
        res = MM_TO_POINT(148);
        break;
    case QPrinter::A7:
        res = MM_TO_POINT(105);
        break;
    case QPrinter::A8:
        res = MM_TO_POINT(74);
        break;
    case QPrinter::A9:
        res = MM_TO_POINT(52);
        break;
    case QPrinter::B0:
        res = MM_TO_POINT(1414);
        break;
    case QPrinter::B1:
        res = MM_TO_POINT(1000);
        break;
    case QPrinter::B2:
        res = MM_TO_POINT(707);
        break;
    case QPrinter::B3:
        res = MM_TO_POINT(500);
        break;
    case QPrinter::B4:
        res = MM_TO_POINT(353);
        break;
    case QPrinter::B5:
        res = MM_TO_POINT(250);
        break;
    case QPrinter::B6:
        res = MM_TO_POINT(176);
        break;
    case QPrinter::B7:
        res = MM_TO_POINT(125);
        break;
    case QPrinter::B8:
        res = MM_TO_POINT(88);
        break;
    case QPrinter::B9:
        res = MM_TO_POINT(62);
        break;
    case QPrinter::B10:
        res = MM_TO_POINT(44);
        break;
    case QPrinter::C5E:
        res = MM_TO_POINT(229);
        break;
    case QPrinter::DLE:
        res = MM_TO_POINT(220);
        break;
    case QPrinter::Executive:
        res = MM_TO_POINT(254);
        break;
    case QPrinter::Folio:
        res = MM_TO_POINT(330);
        break;
    case QPrinter::Ledger:
        res = MM_TO_POINT(279);
        break;
    case QPrinter::Legal:
        res = MM_TO_POINT(356);
        break;
    case QPrinter::Letter:
        res = MM_TO_POINT(279);
        break;
    case QPrinter::Tabloid:
        res = MM_TO_POINT(432);
        break;

    }
    return res;
}

// ------------------------- protected function -------------------------------


int Tmpl_plugin::getElemCount(QGraphicsScene *scene)
{
    qDebug() <<Q_FUNC_INFO<< "scene count = " << scene->items().size();
    return scene->items().size();
    /*
    QGraphicsItem *item;
    int count = 0;

    item = findPaperElem(scene);
    if (item){
        count = item->childItems().size();
        count--; // Так элементы считаются с 0 то уменьшим на один
    }

    return count;
    */
}
QGraphicsItem *Tmpl_plugin::findPaperElem(QGraphicsScene *scene)
{
    QGraphicsItem *item;

    // Поиск  указателя на бумагу
    for (int i = 0; i < scene->items().size(); ++i){
        item = scene->items().at(i);
        if ( item->data(ObjectName).toString()=="Paper"){
            break;
        }
    }
    return item;
}

void Tmpl_plugin::fillElemMap()
{
    elem_name_map.insert(QObject::trUtf8("МБ"),1 );
    elem_name_map.insert(QObject::trUtf8("Название док-та"),2 );
    elem_name_map.insert(QObject::trUtf8("Гриф"), 3);
    elem_name_map.insert(QObject::trUtf8("Пункт перечня"),4 );
    elem_name_map.insert(QObject::trUtf8("Номер копии"), 5);
    elem_name_map.insert(QObject::trUtf8("Кол-во листов"),6 );
    elem_name_map.insert(QObject::trUtf8("Исполнитель"), 7);
    elem_name_map.insert(QObject::trUtf8("Отпечатал"), 8);
    elem_name_map.insert(QObject::trUtf8("Телефон"), 9);
    elem_name_map.insert(QObject::trUtf8("Инв. N"), 10);
    elem_name_map.insert(QObject::trUtf8("Дата распечатки"), 11);
    elem_name_map.insert(QObject::trUtf8("Получатель N1"), 12);
    elem_name_map.insert(QObject::trUtf8("Получатель N2"), 13);
    elem_name_map.insert(QObject::trUtf8("Получатель N3"), 14);
    elem_name_map.insert(QObject::trUtf8("Получатель N4"), 15);
    elem_name_map.insert(QObject::trUtf8("Получатель N5"), 16);
    /*
    elem_name_map.insert(QObject::trUtf8("last_page_stamp"), 17);
    elem_name_map.insert(QObject::trUtf8("recivers_list"), 18);
    elem_name_map.insert(QObject::trUtf8("doc_status"), 19);
    elem_name_map.insert(QObject::trUtf8("brak_pages"), 20);
    elem_name_map.insert(QObject::trUtf8("brak_doc"), 21);
    elem_name_map.insert(QObject::trUtf8("stamp_index"), 22);
*/
}

void Tmpl_plugin::fillPageMap()
{

    page_size_map.insert(QString("A4 (210 x 297 мм)"), QPrinter::A4);
    page_size_map.insert(QString("A3 (297 x 420 мм)"), QPrinter::A3);
    page_size_map.insert(QString("A0 (841 x 1189 мм)"), QPrinter::A0);
    page_size_map.insert(QString("A1 (594 x 841 мм)"), QPrinter::A1);
    page_size_map.insert(QString("A2 (420 x 594 мм)"), QPrinter::A2);
    page_size_map.insert(QString("A5 (148 x 210 мм)"), QPrinter::A5);
    page_size_map.insert(QString("A6 (105 x 148 мм)"), QPrinter::A6);
    page_size_map.insert(QString("A7 (74 x 105 мм)"), QPrinter::A7);
    page_size_map.insert(QString("A8 (52 x 74 мм)"), QPrinter::A8);
    page_size_map.insert(QString("A9 (37 x 52 мм)"), QPrinter::A9);
    page_size_map.insert(QString("B0 (1000 x 1414 мм)"), QPrinter::B0);
    page_size_map.insert(QString("B1 (707 x 1000 мм)"), QPrinter::B1);
    page_size_map.insert(QString("B2 (500 x 707 мм)"), QPrinter::B2);
    page_size_map.insert(QString("B3 (353 x 500 мм)"), QPrinter::B3);
    page_size_map.insert(QString("B4 (250 x 353 мм)"), QPrinter::B4);
    page_size_map.insert(QString("B5 (176 x 250 мм)"), QPrinter::B5);
    page_size_map.insert(QString("B6 (125 x 176 мм)"), QPrinter::B6);
    page_size_map.insert(QString("B7 (88 x 125 мм)"), QPrinter::B7);
    page_size_map.insert(QString("B8 (62 x 88 мм)"), QPrinter::B8);
    page_size_map.insert(QString("B9 (44 x 62 мм)"), QPrinter::B9);
    page_size_map.insert(QString("B10 (31 x 44 мм)"), QPrinter::B10);
    page_size_map.insert(QString("C5E (163 x 229 мм)"), QPrinter::C5E);
    page_size_map.insert(QString("DLE (110 x 220 мм)"), QPrinter::DLE);
    page_size_map.insert(QString("Executive (191 x 254 мм)"), QPrinter::Executive);
    page_size_map.insert(QString("Folio (210 x 330 мм)"), QPrinter::Folio);
    page_size_map.insert(QString("Ledger (432 x 279 мм)"), QPrinter::Ledger);
    page_size_map.insert(QString("Legal (216 x 356 мм)"), QPrinter::Legal);
    page_size_map.insert(QString("Letter (216 x 279 мм)"), QPrinter::Letter);
    page_size_map.insert(QString("Tabloid (279 x 432 мм)"), QPrinter::Tabloid);
}
void Tmpl_plugin::create_SimpleItem(QGraphicsItem *parent,
                                    QPointF &ps, QFont &fnt,
                                    QColor &col,QStringList &pList)
{
    QStringList filledList;

    SimpleItem * pItem = new SimpleItem;

    //Анализ pList на предмет наличия [тег]
    /*
    for (int j = 0; j <pList.size();j++){
        filledList.append(findFromModel(pList.at(j)));
    }
    */
    pItem->setPos(ps);
    pItem->setFont(fnt);
    pItem->setColor(col);
    pItem->setText(pList);
    pItem->setZValue(100);
    pItem->setFlag(QGraphicsItem::ItemIsMovable);
    pItem->setData(ObjectName, "tElem");
    pItem->setParentItem(parent);
}

//*****************************************************************************
void Tmpl_plugin::update_scene(QGraphicsScene *scene)
{
    // пользователь удалил или добавил элемент на сцену, требуется заново
    // пройти по сцене и обновить содержимое элемента имеющего в качестве
    // текста [some_text] проверка есть ли такой в модели и запись значения

    QString t_str;
    QStringList old_list;
    QStringList new_list;

    for (int i = 0; i < scene->items().size(); i++){
        QGraphicsItem *item = scene->items().at(i);
        t_str=item->data(ObjectName).toString();
        if (t_str==QString("tElem")){
            old_list.clear();
            new_list.clear();

            SimpleItem* item =(SimpleItem* )scene->items().at(i);
            old_list = item->getText();
            //Анализ old_list на предмет наличия [тег]
            for (int j = 0; j <old_list.size();j++){
                new_list.append(findFromModel(old_list.at(j)));
            }
            item->setText(new_list);
        }
    }

}



QDataStream &operator << (QDataStream &out,const QGraphicsScene * scene)
{
    QGraphicsItem * item;
    QString elem_type;

    for (int i = 0; i < scene->items().size(); ++i){
        item = scene->items().at(i);
        elem_type=item->data(ObjectName).toString();
        out << elem_type; // Сохраним тип элемента
        if (elem_type == "tElem"){
            SimpleItem* tElem =(SimpleItem* )scene->items().at(i);
            out << tElem->pos();
            out << tElem->getFont();
            out << tElem->getColor();
            out << tElem->getText();
        }
    }
    return out;
}
/*
QDebug       operator << (QDebug &out, const QGraphicsScene * scene)
{

}
QDataStream &operator >> (QDataStream &in, QGraphicsScene *scene)
{

}
*/

Q_EXPORT_PLUGIN2(Itmpl_plugin, Tmpl_plugin)
;


