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
            firstPage_tmpl  = new QGraphicsScene(this);
            secondPage_tmpl = new QGraphicsScene(this);
            thirdPage_tmpl  = new QGraphicsScene(this);
            fourthPage_tmpl = new QGraphicsScene(this);

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

void Tmpl_plugin::setTemplates(const QString & templates_in_file,QStandardItemModel * model)
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
            currentTemplates = templates_in_file;
            if (parse_templates(currentTemplates)){
                emit allTemplatesPagesParsed(firstPage_tmpl, secondPage_tmpl, thirdPage_tmpl, fourthPage_tmpl);
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
        scene = firstPage_tmpl;
        break;
    case 2:
        scene = secondPage_tmpl;
        break;
    case 3:
        scene = thirdPage_tmpl;
        break;
    case 4:
        scene = fourthPage_tmpl;
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
    if (!currentTemplates.isEmpty() && pageNum <= 4 && pageNum >=1){
        QPrinter pdfprinter;
        if (page_orient){
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

            pdfprinter.setOutputFileName(firstPage_tmpl_fn);
            firstPage_tmpl->render(&painter);

            break;
        case 2:
            pdfprinter.setOutputFileName(secondPage_tmpl_fn);
            secondPage_tmpl->render(&painter);
            break;
        case 3:
            pdfprinter.setOutputFileName(thirdPage_tmpl_fn);
            thirdPage_tmpl->render(&painter);
            break;
        case 4:
            pdfprinter.setOutputFileName(fourthPage_tmpl_fn);
            fourthPage_tmpl->render(&painter);
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
            firstPage_tmpl->clear();
            secondPage_tmpl->clear();
            thirdPage_tmpl->clear();
            fourthPage_tmpl->clear();

            QFile file(currentTemplates);
            file.open(QIODevice::ReadOnly);
            QDataStream in(&file);
            in.setVersion(QDataStream::Qt_4_5);
            in >> ver;
            if (ver <= version ){
                // Читаем общую часть шаблона
                in >> date_time;               // дата и время создания шаблона
                in >> author;                  // автор шаблона отобразиться в tooltipe
                in >> templates_name;          // название шаблона, то что покажем в списке шаблонов
                in >> description;             // описание шаблона, может быть пустым
                in >> paper_size;              // размер бумаги
                in >> page_height;             // высота листа
                in >> page_width;              // ширина листа
                in >> page_orient;             // ориентация листа
                in >> margin_top;              // отступ сверху
                in >> margin_bottom;           // отступ снизу
                in >> margin_left;             // отступ слева
                in >> margin_right;            // отступ справа

                // создаем основное рабочее поле
                firstPage_tmpl->setSceneRect(0, 0, page_width,page_height);
                secondPage_tmpl->setSceneRect(0, 0, page_width,page_height);
                thirdPage_tmpl->setSceneRect(0, 0, page_width,page_height);
                fourthPage_tmpl->setSceneRect(0, 0, page_width,page_height);
                firstPage_tmpl->setBackgroundBrush(Qt::white);
                secondPage_tmpl->setBackgroundBrush(Qt::white);
                thirdPage_tmpl->setBackgroundBrush(Qt::white);
                fourthPage_tmpl->setBackgroundBrush(Qt::white);
                // рисуем границы (при печати надо их убирать)

                QGraphicsRectItem *paper_rect_1 = new QGraphicsRectItem (QRectF(0,0, page_width,page_height));
                paper_rect_1->setPen(QPen(Qt::black));
                paper_rect_1->setBrush(QBrush(Qt::white));
                paper_rect_1->setZValue(-1000.0);
                paper_rect_1->setData(ObjectName, "Paper_1");
                firstPage_tmpl->addItem(paper_rect_1);

                QGraphicsRectItem *border_rect_1 = new QGraphicsRectItem (
                        QRectF(margin_left, margin_top,
                               page_width-margin_left-margin_right,
                               page_height-margin_top-margin_bottom));

                border_rect_1->setPen(QPen(Qt::black,2,Qt::DotLine));
                border_rect_1->setBrush(QBrush(Qt::white));
                border_rect_1->setOpacity(1);
                border_rect_1->setZValue(-900);
                border_rect_1->setData(ObjectName, "Border_1");
                border_rect_1->setParentItem(paper_rect_1);

                QGraphicsRectItem *paper_rect_2 = new QGraphicsRectItem (QRectF(0,0, page_width,page_height));
                paper_rect_2->setPen(QPen(Qt::black));
                paper_rect_2->setBrush(QBrush(Qt::white));
                paper_rect_2->setZValue(-1000.0);
                paper_rect_2->setData(ObjectName, "Paper_2");
                secondPage_tmpl->addItem(paper_rect_2);

                QGraphicsRectItem *border_rect_2 = new QGraphicsRectItem (
                        QRectF(margin_left, margin_top,
                               page_width-margin_left-margin_right,
                               page_height-margin_top-margin_bottom));

                border_rect_2->setPen(QPen(Qt::black,2,Qt::DotLine));
                border_rect_2->setBrush(QBrush(Qt::white));
                border_rect_2->setOpacity(1);
                border_rect_2->setZValue(-900);
                border_rect_2->setData(ObjectName, "Border_2");
                border_rect_2->setParentItem(paper_rect_2);

                QGraphicsRectItem *paper_rect_3 = new QGraphicsRectItem (QRectF(0,0, page_width,page_height));
                paper_rect_3->setPen(QPen(Qt::black));
                paper_rect_3->setBrush(QBrush(Qt::white));
                paper_rect_3->setZValue(-1000.0);
                paper_rect_3->setData(ObjectName, "Paper_3");
                thirdPage_tmpl->addItem(paper_rect_3);

                QGraphicsRectItem *border_rect_3 = new QGraphicsRectItem (
                        QRectF(margin_left, margin_top,
                               page_width-margin_left-margin_right,
                               page_height-margin_top-margin_bottom));

                border_rect_3->setPen(QPen(Qt::black,2,Qt::DotLine));
                border_rect_3->setBrush(QBrush(Qt::white));
                border_rect_3->setOpacity(1);
                border_rect_3->setZValue(-900);
                border_rect_3->setData(ObjectName, "Border_3");
                border_rect_3->setParentItem(paper_rect_3);

                QGraphicsRectItem *paper_rect_4 = new QGraphicsRectItem (QRectF(0,0, page_width,page_height));
                paper_rect_4->setPen(QPen(Qt::black));
                paper_rect_4->setBrush(QBrush(Qt::white));
                paper_rect_4->setZValue(-1000.0);
                paper_rect_4->setData(ObjectName, "Paper_4");
                fourthPage_tmpl->addItem(paper_rect_4);

                QGraphicsRectItem *border_rect_4 = new QGraphicsRectItem (
                        QRectF(margin_left, margin_top,
                               page_width-margin_left-margin_right,
                               page_height-margin_top-margin_bottom));

                border_rect_4->setPen(QPen(Qt::black,2,Qt::DotLine));
                border_rect_4->setBrush(QBrush(Qt::white));
                border_rect_4->setOpacity(1);
                border_rect_4->setZValue(-900);
                border_rect_4->setData(ObjectName, "Border_4");
                border_rect_4->setParentItem(paper_rect_4);


                QString elemType;
                QPointF ps;
                QFont fnt;
                QColor col;
                QStringList pList;
                QStringList filledList;
                in >> firstPageElemCount;      // число элементов на первой странице
                for (int i=0;i<firstPageElemCount;i++){
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
                        pItem->setParentItem(paper_rect_1);
                    }
                }

                in >> secondPageElemCount;      // число элементов на второй странице
                for (int i=0;i<secondPageElemCount;i++){
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

                in >> thirdPageElemCount;      // число элементов на третьей странице
                for (int i=0;i<thirdPageElemCount;i++){
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
                        pItem->setParentItem(paper_rect_3);
                    }
                }
                in >> fourthPageElemCount;      // число элементов на четвертой странице
                for (int i=0;i<fourthPageElemCount;i++){
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
                        pItem->setParentItem(paper_rect_4);
                    }
                }
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

Q_EXPORT_PLUGIN2(Itmpl_plugin, Tmpl_plugin)
;


