#include "dcontroller.h"

dController::dController()
{
    // формируем SID
    QUuid uSID=QUuid::createUuid () ;  //generate SID
    SID=uSID.toString().replace("{","").replace("}","");

    this->read_settings();
     createModel();
}

void dController::createModel()
{
    doc_model = new QStandardItemModel();

     header << QObject::trUtf8("Номер док-та")
	    << QObject::trUtf8("Название док-та")
	    << QObject::trUtf8("Гриф")
	    << QObject::trUtf8("Пункт перечня")
	    << QObject::trUtf8("Номер копии")
	    << QObject::trUtf8("Кол-во листов")
	    << QObject::trUtf8("Исполнитель")
	    << QObject::trUtf8("Отпечатал")
	    << QObject::trUtf8("Телефон")
	    << QObject::trUtf8("Инв. №")
	    << QObject::trUtf8("Дата распечатки")
	    << QObject::trUtf8("Получатель №1")
	    << QObject::trUtf8("Получатель №2")
	    << QObject::trUtf8("Получатель №3")
	    << QObject::trUtf8("Получатель №4")
	    << QObject::trUtf8("Получатель №5")
	    << QObject::trUtf8("Статус документа")
	    << QObject::trUtf8("Штамп последней стр.")
	    << QObject::trUtf8("Список рассылки");
    doc_model->setHorizontalHeaderLabels(header);
    this->insertDocToModel();

}

void dController::insertDocToModel()
{
    // add empty row to model
    doc_model->insertRow(doc_model->rowCount());
}


void dController::insertDocToModel(QString &item)
{
     QStringList itemList= item.split("~");
     QList<QStandardItem *> cells;
     int Pos;

     for (int i = 0; i <itemList.size() ; ++i) {
	 QStandardItem * cell_item= new QStandardItem();
	 cells.append(cell_item);
     }

     for (int i = 0; i <itemList.size() ; ++i) {
	 QStringList list_2 = itemList.at(i).split("=", QString::SkipEmptyParts);
	 Pos =header.indexOf(list_2.at(0)); // Ищем индекс
	 QStandardItem *cell_item =cells.at(Pos);
	 cell_item->setData(QVariant(list_2.at(1)),Qt::EditRole);
     }
     doc_model->appendRow (cells);
}

QStandardItemModel *dController::document_model () const
{
    return doc_model;
}

void dController::connect2Demon()
{
    qDebug() <<"Connect";
    // Соединились с сервером безопастности
   QString msg= QObject::trUtf8("Успешно соединились с сервером безопастности");
   emit init_done(CONN_STEP,msg);
}

void dController::convert2PDF(QString &in_file)
{
    qDebug() <<"Convert";
     // Файл преобразован в pdf
    QString msg= QObject::trUtf8("Исходный файл успешно конвертирован в pdf");
    emit init_done(CONVERT_STEP,msg);

}
void dController::mergeTwoPDF(QString &fg_file,QString &bg_file,QString &out_file)
{
    emit inputFileMerged();
    // Файл объединен
}

void dController::read_settings()
{
    // Читаем файл настроек
}
