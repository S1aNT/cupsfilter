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

#include "tmpl_plugin.h"
#include "tech_global.h"
#include "simpleitem.h"

using namespace SafeVirtualPrinter;


void Tmpl_plugin::init(const QString &spool,const QString &sid)
{
    QString error_msg;
    QDir dir;

    if (!sid.isEmpty()) {
        current_sid = sid;// Запомним уникальный номер

        if (dir.cd(spool) && !spool.isEmpty()) {
            // Проверим факт существования временного каталога
            spool_dir = spool;
            // Формируем пути для файлов
            firstPage_tmpl_fn = QObject::trUtf8("%1/%2_first.pdf").arg(spool, sid);
            secondPage_tmpl_fn = QObject::trUtf8("%1/%2_second.pdf").arg(spool, sid);
            thirdPage_tmpl_fn = QObject::trUtf8("%1/%2_third.pdf").arg(spool, sid);
            fourthPage_tmpl_fn = QObject::trUtf8("%1/%2_fourth.pdf").arg(spool, sid);
            // создаем сцены
            firstPage_scene  = new QGraphicsScene(this);
            secondPage_scene = new QGraphicsScene(this);
            thirdPage_scene  = new QGraphicsScene(this);
            fourthPage_scene = new QGraphicsScene(this);

            page_size.insert(QString("A4 (210 x 297 мм)"), QPrinter::A4);
            page_size.insert(QString("A3 (297 x 420 мм)"), QPrinter::A3);
            page_size.insert(QString("A0 (841 x 1189 мм)"), QPrinter::A0);
            page_size.insert(QString("A1 (594 x 841 мм)"), QPrinter::A1);
            page_size.insert(QString("A2 (420 x 594 мм)"), QPrinter::A2);
            page_size.insert(QString("A5 (148 x 210 мм)"), QPrinter::A5);
            page_size.insert(QString("A6 (105 x 148 мм)"), QPrinter::A6);
            page_size.insert(QString("A7 (74 x 105 мм)"), QPrinter::A7);
            page_size.insert(QString("A8 (52 x 74 мм)"), QPrinter::A8);
            page_size.insert(QString("A9 (37 x 52 мм)"), QPrinter::A9);
            page_size.insert(QString("B0 (1000 x 1414 мм)"), QPrinter::B0);
            page_size.insert(QString("B1 (707 x 1000 мм)"), QPrinter::B1);
            page_size.insert(QString("B2 (500 x 707 мм)"), QPrinter::B2);
            page_size.insert(QString("B3 (353 x 500 мм)"), QPrinter::B3);
            page_size.insert(QString("B4 (250 x 353 мм)"), QPrinter::B4);
            page_size.insert(QString("B5 (176 x 250 мм)"), QPrinter::B5);
            page_size.insert(QString("B6 (125 x 176 мм)"), QPrinter::B6);
            page_size.insert(QString("B7 (88 x 125 мм)"), QPrinter::B7);
            page_size.insert(QString("B8 (62 x 88 мм)"), QPrinter::B8);
            page_size.insert(QString("B9 (44 x 62 мм)"), QPrinter::B9);
            page_size.insert(QString("B10 (31 x 44 мм)"), QPrinter::B10);
            page_size.insert(QString("C5E (163 x 229 мм)"), QPrinter::C5E);
            page_size.insert(QString("DLE (110 x 220 мм)"), QPrinter::DLE);
            page_size.insert(QString("Executive (191 x 254 мм)"), QPrinter::Executive);
            page_size.insert(QString("Folio (210 x 330 мм)"), QPrinter::Folio);
            page_size.insert(QString("Ledger (432 x 279 мм)"), QPrinter::Ledger);
            page_size.insert(QString("Legal (216 x 356 мм)"), QPrinter::Legal);
            page_size.insert(QString("Letter (216 x 279 мм)"), QPrinter::Letter);
            page_size.insert(QString("Tabloid (279 x 432 мм)"), QPrinter::Tabloid);


        }else{
            error_msg = QObject::trUtf8("ERROR: каталог %1 не существует\n").arg(spool);
        }

    }else{
        error_msg = QObject::trUtf8("ERROR: Неверный SID для документа\n").arg(sid);
    }
    if (!error_msg.isEmpty()) {
        emit error(error_msg);
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
    QString error_msg;
    const QString startnow = QDir::currentPath();
    // Создаем пустой шаблон документа

    if (QFile::exists(file_name)){
        QFile::remove(file_name);
    }
    QFile new_tmpl_file(file_name);
    new_tmpl_file.open(QIODevice::WriteOnly);
    QDataStream out(&new_tmpl_file);
    out.setVersion(QDataStream::Qt_4_5);
    // Запишем версию шаблона
    out << t_info.version;
    // Создаем общую часть шаблона
    out << c_date;      // дата и время создания шаблона
    out << t_author;    // автор шаблона
    out << t_name;      // название шаблона, то что покажем в списке шаблонов
    out << t_desc;      // описание шаблона, может быть пустым
    out << p_size;      // размер бумаги
    int p_s_id = this->getElemIdByName(p_size);

    out << this->findPageSize_H(p_s_id);    // высота листа
    out << this->findPageSize_V(p_s_id);     // ширина листа
    out << pages_orient;    // ориентация листа
    out << m_top;       // отступ сверху
    out << m_bottom;    // отступ снизу
    out << m_left;      // отступ слева
    out << m_right;     // отступ справа
    int elem_count =0;
    out << elem_count; // первая страница шаблона 0 элементов
    out << elem_count; // вторая страница шаблона 0 элементов
    out << elem_count; // третья страница шаблона 0 элементов
    out << elem_count; // четвертая страница шаблона 0 элементов
    new_tmpl_file.close();
    emit emptyTemplateCreate(file_name);


    if (!error_msg.isEmpty()) {
        emit error(error_msg);
    }
}

void Tmpl_plugin::doAddBaseElementToPage(int page)
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
        pItem->setText(QStringList()<<QObject::trUtf8("Элемент"));
        pItem->setFlag(QGraphicsItem::ItemIsMovable);
        pItem->setData(ObjectName, "tElem");
        pItem->setParentItem(item);
        scene->update();
    }
}
void Tmpl_plugin::setTemplates(const QString & templates_in_file,
                               QStandardItemModel * model)
{
    QString error_msg;
    if (QFile::exists(templates_in_file)) {
        if (model){
            work_model = model;
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
                error_msg = QObject::trUtf8("ERROR: Ошибка разбора шаблона [%1]\n").arg(templates_in_file);
            }
        }else{
            error_msg = QObject::trUtf8("ERROR: Модель [карточки документа] не существует\n");
        }
    }else{
        error_msg = QObject::trUtf8("Файл [%1] шаблона не найден.").arg(templates_in_file);
    }
    if (!error_msg.isEmpty()) {
        emit error(error_msg);
    }
}

//----------------------- public slots -----------------------------------------
void Tmpl_plugin::doSaveTemplates()
{
    // Сохраняется текущий рабочий шаблон,
    // т.е тот который был выбран и показан пользователю
    QString e_msg;
    QGraphicsItem *paper_item;
    QString elem_type;
    int count;
    if (QFile::exists(t_info.file_name)){
        QFile::rename(t_info.file_name,t_info.file_name+".bak");
        // запись основных данных шаблона
        QFile new_tmpl_file(t_info.file_name);
        new_tmpl_file.open(QIODevice::WriteOnly);
        QDataStream out(&new_tmpl_file);
        out.setVersion(QDataStream::Qt_4_5);
        // Запишем версию шаблона
        out << t_info.version;
        // Создаем общую часть шаблона
        t_info.date_time = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss");
        out <<  t_info.date_time;      // дата и время создания шаблона / модификации
        out << t_info.t_author;    // автор шаблона
        out << t_info.t_name;      // название шаблона, то что покажем в списке шаблонов
        out << t_info.t_desc;      // описание шаблона, может быть пустым
        out << t_info.p_size;      // размер бумаги
        int p_s_id = this->getElemIdByName(t_info.p_size);

        out << this->findPageSize_H(p_s_id);    // высота листа
        out << this->findPageSize_V(p_s_id);     // ширина листа
        out << t_info.page_orient;    // ориентация листа
        out << t_info.m_top;       // отступ сверху
        out << t_info.m_bottom;    // отступ снизу
        out << t_info.m_left;      // отступ слева
        out << t_info.m_right;     // отступ справа

        count = getElemCount(firstPage_scene);
        paper_item = findPaperElem(firstPage_scene);
        out << count; // первая страница шаблона элементов
        for (int i=0; i < count; ++i){
            elem_type = paper_item->childItems().at(i)->data(ObjectName).toString();
            out << elem_type; // Сохраним тип элемента
            if (elem_type == "tElem"){
                SimpleItem* pItem = (SimpleItem* )paper_item->childItems().at(i);

                out << pItem->pos()
                    << pItem->getFont()
                    << pItem->getColor()
                    << pItem->getText();
            }
        }

        count = getElemCount(secondPage_scene);
        paper_item = findPaperElem(secondPage_scene);
        out << count; // вторая страница шаблона элементов
        for (int i=0; i < count; ++i){
            elem_type = paper_item->childItems().at(i)->data(ObjectName).toString();
            if (elem_type == "tElem"){

                SimpleItem* pItem = (SimpleItem* )paper_item->childItems().at(i);
                out << pItem->pos()
                        << pItem->getFont()
                        << pItem->getColor()
                        << pItem->getText();
            }
        }

        count = getElemCount(thirdPage_scene);
        paper_item = findPaperElem(thirdPage_scene);
        out << count; // третья страница шаблона элементов
        for (int i=0; i < count; ++i){
            elem_type = paper_item->childItems().at(i)->data(ObjectName).toString();
            if (elem_type == "tElem"){

                SimpleItem* pItem = (SimpleItem* )paper_item->childItems().at(i);
                out << pItem->pos()
                        << pItem->getFont()
                        << pItem->getColor()
                        << pItem->getText();
            }
        }

        count = getElemCount(fourthPage_scene);
        paper_item = findPaperElem(fourthPage_scene);
        out << count; // четвертая страница шаблона элементов
        for (int i=0; i < count; ++i){
            elem_type = paper_item->childItems().at(i)->data(ObjectName).toString();
            if (elem_type == "tElem"){

                SimpleItem* pItem = (SimpleItem* )paper_item->childItems().at(i);
                out << pItem->pos()
                        << pItem->getFont()
                        << pItem->getColor()
                        << pItem->getText();
            }
        }

        new_tmpl_file.close();
    }
    if (!e_msg.isEmpty()){
        emit error(e_msg);
    }
}

//******************************************************************************
void Tmpl_plugin::convertTemplatesToPdf(const QString & templates_in_file,QStandardItemModel * model)
{
    QString error_msg;
    if (QFile::exists(templates_in_file)) {
        if (model){
            work_model = model;
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
                error_msg = QObject::trUtf8("ERROR: Ошибка разбора шаблона [%1]\n").arg(templates_in_file);
            }
        }else{
            error_msg = QObject::trUtf8("ERROR: Модель [карточки документа] не существует\n");
        }
    }else{
        error_msg = QObject::trUtf8("Файл [%1] шаблона не найден.").arg(templates_in_file);
    }
    if (!error_msg.isEmpty()) {
        emit error(error_msg);
    }
}

void Tmpl_plugin::update_scene(int pageNum)
{
    // пользователь удалил или добавил элемент на сцену, требуется заново
    // пройти по сцене и обновить содержимое элемента имеющего в качестве
    // текста [some_text] проверка есть ли такой в модели и запись значения
    QGraphicsScene *scene;
    QString t_str;
    QStringList old_list;
    QStringList new_list;

    switch(pageNum){
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
    }

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

void Tmpl_plugin::printFormatingPageToFile(int pageNum)
{
    QString error_msg;
    // Печатает выбранную страницу текущего шаблона в pdf файл
    // страница формируется исходя из данных модели
    if (!t_info.file_name.isEmpty() && pageNum <= 4 && pageNum >=1){
        QPrinter pdfprinter;
        if (t_info.page_orient){
            pdfprinter.setOrientation(QPrinter::Portrait);
        }else{
            pdfprinter.setOrientation(QPrinter::Landscape);
        }
        pdfprinter.setOutputFormat(QPrinter::PdfFormat);
        //pdfprinter.setPageSize(paper_size);
        QPainter painter(&pdfprinter);

        switch (pageNum){
        case 1:
            //FIXME:
            // пройтись по всем элементам сцены и удалить границу
            // сцена к этому моменту уже создана и заполнена элементами
            if (QFile::exists(firstPage_tmpl_fn)){
                QFile::remove(firstPage_tmpl_fn);
            }
            pdfprinter.setOutputFileName(firstPage_tmpl_fn);
            firstPage_scene->render(&painter);

            break;
        case 2:
            if (QFile::exists(secondPage_tmpl_fn)){
                QFile::remove(secondPage_tmpl_fn);
            }

            pdfprinter.setOutputFileName(secondPage_tmpl_fn);
            secondPage_scene->render(&painter);
            break;
        case 3:
            if (QFile::exists(thirdPage_tmpl_fn)){
                QFile::remove(thirdPage_tmpl_fn);
            }

            pdfprinter.setOutputFileName(thirdPage_tmpl_fn);
            thirdPage_scene->render(&painter);
            break;
        case 4:
            if (QFile::exists(fourthPage_tmpl_fn)){
                QFile::remove(fourthPage_tmpl_fn);
            }

            pdfprinter.setOutputFileName(fourthPage_tmpl_fn);
            fourthPage_scene->render(&painter);
            break;
        }
    }
}

bool Tmpl_plugin::parse_templates(const QString & in_file)
{
    bool flag;
    int ver;

    if (!in_file.isEmpty()){
        if (QFile::exists(in_file)){
            firstPage_scene->clear();
            secondPage_scene->clear();
            thirdPage_scene->clear();
            fourthPage_scene->clear();

            // сохраним имя текущего файла шаблона
            t_info.file_name = in_file;
            QFile file(in_file);
            file.open(QIODevice::ReadOnly);
            QDataStream in(&file);
            in.setVersion(QDataStream::Qt_4_5);
            // Получим версию шаблона
            in >> ver;
            if (ver == t_info.version ){
                // Читаем общую часть шаблона
                in >> t_info.date_time;               // дата и время создания шаблона
                in >> t_info.t_author;                  // автор шаблона отобразиться в tooltipe
                in >> t_info.t_name;          // название шаблона, то что покажем в списке шаблонов
                in >> t_info.t_desc;             // описание шаблона, может быть пустым
                in >> t_info.p_size;              // размер бумаги
                in >> t_info.page_height;             // высота листа
                in >> t_info.page_width;              // ширина листа
                in >> t_info.page_orient;             // ориентация листа
                in >> t_info.m_top;              // отступ сверху
                in >> t_info.m_bottom;           // отступ снизу
                in >> t_info.m_left;             // отступ слева
                in >> t_info.m_right;            // отступ справа

                // создаем основное рабочее поле
                firstPage_scene->setSceneRect(0, 0, t_info.page_width,t_info.page_height);
                secondPage_scene->setSceneRect(0, 0, t_info.page_width,t_info.page_height);
                thirdPage_scene->setSceneRect(0, 0, t_info.page_width,t_info.page_height);
                fourthPage_scene->setSceneRect(0, 0, t_info.page_width,t_info.page_height);
                firstPage_scene->setBackgroundBrush(Qt::white);
                secondPage_scene->setBackgroundBrush(Qt::white);
                thirdPage_scene->setBackgroundBrush(Qt::white);
                fourthPage_scene->setBackgroundBrush(Qt::white);
                // рисуем границы (при печати надо их убирать)

                QGraphicsRectItem *paper_rect_1 =
                        new QGraphicsRectItem (QRectF(0,0, t_info.page_width,t_info.page_height));
                paper_rect_1->setPen(QPen(Qt::black));
                paper_rect_1->setBrush(QBrush(Qt::white));
                paper_rect_1->setZValue(-1000.0);
                paper_rect_1->setData(ObjectName, "Paper");
                firstPage_scene->addItem(paper_rect_1);

                QGraphicsRectItem *border_rect_1 = new QGraphicsRectItem (
                        QRectF(t_info.m_left, t_info.m_top,
                               t_info.page_width-t_info.m_left-t_info.m_right,
                               t_info.page_height-t_info.m_top-t_info.m_bottom));

                border_rect_1->setPen(QPen(Qt::black,2,Qt::DotLine));
                border_rect_1->setBrush(QBrush(Qt::white));
                border_rect_1->setOpacity(1);
                border_rect_1->setZValue(-900);
                border_rect_1->setData(ObjectName, "Border");
                border_rect_1->setParentItem(paper_rect_1);
                //firstPage_scene->addItem(border_rect_1);

                QGraphicsRectItem *paper_rect_2 =
                        new QGraphicsRectItem (QRectF(0,0, t_info.page_width,t_info.page_height));
                paper_rect_2->setPen(QPen(Qt::black));
                paper_rect_2->setBrush(QBrush(Qt::white));
                paper_rect_2->setZValue(-1000.0);
                paper_rect_2->setData(ObjectName, "Paper");
                secondPage_scene->addItem(paper_rect_2);

                QGraphicsRectItem *border_rect_2 = new QGraphicsRectItem (
                        QRectF(t_info.m_left, t_info.m_top,
                               t_info.page_width-t_info.m_left-t_info.m_right,
                               t_info.page_height-t_info.m_top-t_info.m_bottom));

                border_rect_2->setPen(QPen(Qt::black,2,Qt::DotLine));
                border_rect_2->setBrush(QBrush(Qt::white));
                border_rect_2->setOpacity(1);
                border_rect_2->setZValue(-900);
                border_rect_2->setData(ObjectName, "Border");
                border_rect_2->setParentItem(paper_rect_2);
                //secondPage_scene->addItem(border_rect_2);

                QGraphicsRectItem *paper_rect_3 =
                        new QGraphicsRectItem (QRectF(0,0, t_info.page_width,t_info.page_height));
                paper_rect_3->setPen(QPen(Qt::black));
                paper_rect_3->setBrush(QBrush(Qt::white));
                paper_rect_3->setZValue(-1000.0);
                paper_rect_3->setData(ObjectName, "Paper");
                thirdPage_scene->addItem(paper_rect_3);

                QGraphicsRectItem *border_rect_3 = new QGraphicsRectItem (
                        QRectF(t_info.m_left, t_info.m_top,
                               t_info.page_width-t_info.m_left-t_info.m_right,
                               t_info.page_height-t_info.m_top-t_info.m_bottom));

                border_rect_3->setPen(QPen(Qt::black,2,Qt::DotLine));
                border_rect_3->setBrush(QBrush(Qt::white));
                border_rect_3->setOpacity(1);
                border_rect_3->setZValue(-900);
                border_rect_3->setData(ObjectName, "Border");
                border_rect_3->setParentItem(paper_rect_3);
                //thirdPage_scene->addItem(border_rect_3);

                QGraphicsRectItem *paper_rect_4 =
                        new QGraphicsRectItem (QRectF(0,0, t_info.page_width,t_info.page_height));
                paper_rect_4->setPen(QPen(Qt::black));
                paper_rect_4->setBrush(QBrush(Qt::white));
                paper_rect_4->setZValue(-1000.0);
                paper_rect_4->setData(ObjectName, "Paper");
                fourthPage_scene->addItem(paper_rect_4);

                QGraphicsRectItem *border_rect_4 = new QGraphicsRectItem (
                        QRectF(t_info.m_left, t_info.m_top,
                               t_info.page_width-t_info.m_left-t_info.m_right,
                               t_info.page_height-t_info.m_top-t_info.m_bottom));

                border_rect_4->setPen(QPen(Qt::black,2,Qt::DotLine));
                border_rect_4->setBrush(QBrush(Qt::white));
                border_rect_4->setOpacity(1);
                border_rect_4->setZValue(-900);
                border_rect_4->setData(ObjectName, "Border");
                border_rect_4->setParentItem(paper_rect_4);
                //fourthPage_scene->addItem(border_rect_4);

                QString elemType;
                QPointF ps;
                QFont fnt;
                QColor col;
                QStringList pList;
                QStringList filledList;
                in >> t_info.firstPageElemCount;      // число элементов на первой странице
                for (int i=0;i<t_info.firstPageElemCount;i++){
                    // перебор всех элементов страницы
                    in >> elemType;
                    if (elemType=="tElem"){
                        pList.clear();
                        filledList.clear();
                        SimpleItem * pItem = new SimpleItem;
                        in >>ps >>fnt >> col >>pList;
                        //Анализ pList на предмет наличия [тег]
                        for (int j = 0; j <pList.size();j++){
                            filledList.append(findFromModel(pList.at(j)));
                        }
                        pItem->setPos(ps);
                        pItem->setText(filledList);
                        pItem->setZValue(i);
                        pItem->setFlag(QGraphicsItem::ItemIsMovable);
                        pItem->setData(ObjectName, "tElem");
                        pItem->setParentItem(paper_rect_1);
                    }
                }

                in >> t_info.secondPageElemCount;      // число элементов на второй странице
                for (int i=0;i<t_info.secondPageElemCount;i++){
                    // перебор всех элементов страницы
                    in >> elemType;
                    if (elemType=="tElem"){
                        pList.clear();
                        filledList.clear();
                        SimpleItem * pItem = new SimpleItem;
                        in >>ps >>fnt >> col >>pList;
                        //Анализ pList на предмет наличия [тег]
                        for (int j = 0; j <pList.size();j++){
                            filledList.append(findFromModel(pList.at(j)));
                        }
                        pItem->setPos(ps);
                        pItem->setText(pList);
                        pItem->setZValue(i);
                        pItem->setFlag(QGraphicsItem::ItemIsMovable);
                        pItem->setData(ObjectName, "tElem");
                        pItem->setParentItem(paper_rect_2);
                    }
                }

                in >> t_info.thirdPageElemCount;      // число элементов на третьей странице
                for (int i=0;i<t_info.thirdPageElemCount;i++){
                    // перебор всех элементов страницы
                    in >> elemType;
                    if (elemType=="tElem"){
                        pList.clear();
                        filledList.clear();
                        SimpleItem * pItem = new SimpleItem;
                        in >>ps >>fnt >> col >>pList;
                        //Анализ pList на предмет наличия [тег]
                        for (int j = 0; j <pList.size();j++){
                            filledList.append(findFromModel(pList.at(j)));
                        }
                        pItem->setPos(ps);
                        pItem->setText(filledList);
                        pItem->setZValue(i);
                        pItem->setFlag(QGraphicsItem::ItemIsMovable);
                        pItem->setData(ObjectName, "tElem");
                        pItem->setParentItem(paper_rect_3);
                    }
                }
                in >> t_info.fourthPageElemCount;      // число элементов на четвертой странице
                for (int i=0;i<t_info.fourthPageElemCount;i++){
                    // перебор всех элементов страницы
                    in >> elemType;
                    if (elemType=="tElem"){
                        pList.clear();
                        filledList.clear();
                        SimpleItem * pItem = new SimpleItem;
                        in >>ps >>fnt >> col >>pList;
                        //Анализ pList на предмет наличия [тег]
                        for (int j = 0; j <pList.size();j++){
                            filledList.append(findFromModel(pList.at(j)));
                        }
                        pItem->setPos(ps);
                        pItem->setText(filledList);
                        pItem->setZValue(i);
                        pItem->setFlag(QGraphicsItem::ItemIsMovable);
                        pItem->setData(ObjectName, "tElem");
                        pItem->setParentItem(paper_rect_4);
                    }
                }
                firstPage_scene->update();
                secondPage_scene->update();
                thirdPage_scene->update();
                fourthPage_scene->update();

                flag = true;
            }else {
                flag =  false; // Мой плагин работает с шаблоном только версии <= version
            }
            file.close();
        }
    }
    return flag;
}

QString Tmpl_plugin::findFromModel(const QString &find_line)
{
    QString local_find;
    local_find = find_line;
    if (find_line.contains("[") && find_line.contains("]")){
        local_find.replace("[","").replace("]","");
        if (work_model){
            for (int i=0;i<work_model->columnCount();i++){
                QStandardItem * header_item = work_model->horizontalHeaderItem(i);
                QString header = header_item->data(Qt::EditRole).toString();
                if (header.compare(find_line) == 0){
                    // В модели всегда две строчки заголовок и данные,работаю со второй строчкой
                    QStandardItem * cell_item = work_model->item(work_model->rowCount(), i);
                    local_find = cell_item->data(Qt::EditRole).toString();
                    break;
                }
            }
        }
    }
    return local_find;
}

QStringList Tmpl_plugin::getPageSizeList()
{
    QStringList result;

    QMapIterator<QString, int> i (page_size);
    while (i.hasNext()) {
        i.next();
        result.append(i.key());
    }

    return result;
}

int Tmpl_plugin::getElemIdByName(const QString &elem_name)
{
    int result=0; // Если в списке запрошенного элемента нет то возвращаем 0
    if (!elem_name.isEmpty() && page_size.contains(elem_name)){
        result= page_size.value(elem_name);
    }
    return result;
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
        //FixMe
    }
    return res;
}

qreal Tmpl_plugin::findPageSize_V(int page_size_id)
{
    qreal res;
    switch (page_size_id){
    case QPrinter::A4:
        res=MM_TO_POINT(210);

        break;
    case QPrinter::A3:
        res=MM_TO_POINT(297);

        break;
        //FixMe
    }
    return res;
}

// ------------------------- protected function -------------------------------
int Tmpl_plugin::getElemCount(QGraphicsScene *scene)
{
    QGraphicsItem *item;
    item = findPaperElem(scene);
    return item->childItems().size();
}
QGraphicsItem *Tmpl_plugin::findPaperElem(QGraphicsScene *scene)
{
    QGraphicsItem *item;

    // Поиск  указателя на бумагу
    for (int i = 0; i < scene->items().size(); ++i){
        item = scene->items().at(i);
        if ( item->data(ObjectName).toString()=="Paper"){
            //labelGroup->setParentItem(item);
            break;
        }
    }
    return item;
}

//*****************************************************************************

Q_EXPORT_PLUGIN2(Itmpl_plugin, Tmpl_plugin)
;


