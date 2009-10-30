#include "dController.h"
#include "SimpleItem.h"


dController::dController(QObject *parent)
{
    QUuid uSID=QUuid::createUuid () ;  //generate SID
    SID=uSID.toString().replace("{","").replace("}","");
    read_settings();
    conn_state=false;

    connect2Server(serverHostName,serverPort);
    createDocModel();
}
QStandardItemModel *dController::document_model () const
{
    return doc_model;
}
void dController::createDocModel()
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

void dController::setPrinter(QString selected_printer)
 {
    qDebug() << Q_FUNC_INFO<<selected_printer;
    curPrinter=selected_printer;
 }
void dController::savebase(QString wide_msg)
{
     currentDoc = wide_msg.split("~");
}

void dController::checkMB(QString mb)
{
    //TODO ask server mb
    // Сервер проверят существует ли в базе документ с таким МБ за год
    // если есть то он возвращает строку содержащую все его данные
    QDate dt_end;
    dt_end=QDate::currentDate (); // Текущая дата
    QDate dt_begin (dt_end.year (), 1,1); // Первое января
    QString m=QString(QObject::trUtf8("%1~%2~%3")).arg(mb).arg(dt_begin.toString(),dt_end.toString());
    emit sendServerCmd(IS_MB_EXIST,m);
}
void dController::connect2Server(QString &host,int port)
{
    netClient  = new netAgent();
    netClient->on_login(host,port,this->SID);
    connect (netClient,SIGNAL(error_signal(int,QString)),this,SIGNAL(error(int,QString)));
    connect (netClient,SIGNAL(takeServerResponce(QString &)),this,SLOT(parseNetwork(QString &)));
    connect (this,SIGNAL(sendServerCmd(commands_t ,QString &)), netClient,SLOT(send2Server(commands_t ,QString &)) );
}

void dController::parseNetwork(QString &message )
{
    qDebug() << Q_FUNC_INFO << message;
    // формат ответа /answer:тело ответа
    QRegExp rx("^/answer:(.+):(.+)$");
    if(rx.indexIn(message) != -1){
	QString ans =rx.cap(1);
	QString body =rx.cap(2);
	qDebug() << ans << body <<"\n";
	switch (ans.toInt()) {

	    case MB_LIST_ANS:
		  // добавим в модель
		  this->insertDocToModel(body);
		break;
	    case IS_MB_NOT_EXIST_ANS:
		  // Такого документа нет
		 emit mbNumberChecked(body);
		break;
	    case IS_MB_EXIST_AND_BRAK_ANS:
		  // Документ есть и он бракованный можно допечатывать
		//doc_model->clear();// Очистим модель ???
		this->insertDocToModel(body);
		 emit mbNumberExist(doc_model->rowCount());
	       break;
	       case IS_MB_EXIST_AND_NOT_BRAK_ANS:
		  // Документ есть и он не бракованный  нельзя допечатывать
		  emit  mbNumberExist(-1);
		   break;
	    case REGISTER_ANS:
		    emit connect2Demon();
		break;
	}
    }
}
void dController::parseCnvThread(int Code,QString output )
{
    QString msg;
	if (Code != QProcess::NormalExit) {
	    msg =QObject::trUtf8("Ошибка предпечатной подготовки.\n%1:\nКод %2").arg(output).arg(Code,0,10);
	    emit error(Code,msg);
	}else{
	    emit  inputFileConverted();
	    qDebug() << QObject::tr("Успешно завершена конвертация исходного файла в pdf.");
	}
}
void dController::parseMergeThread(int Code,QString output )
{
    QString msg;
	if (Code != QProcess::NormalExit) {
	    msg =QObject::trUtf8("Ошибка предпечатной подготовки.\n%1:\nКод %2").arg(output).arg(Code,0,10);
	    emit error(Code,msg);
	}else{
	    emit  inputFileMerged();
	    qDebug() << QObject::tr("Успешно завершено добавление шаблона.");
	}
}
void dController::read_settings()
{
  // TODO надо добавить возможность выбора диапазона дат за которую запрашивает через ini файл
  qDebug() << Q_FUNC_INFO;
	QSettings settings(QSettings::IniFormat, QSettings::UserScope,"Техносервъ","Защищенный принтер");
	settings.setIniCodec("UTF-8");
	settings.beginGroup("SERVICE");
	serverHostName = settings.value("server","127.0.0.1").toString();
	serverPort = settings.value("port",4242).toInt();
	timeout_connect =settings.value("timeout_connect",5000).toInt();
	timeout_read=settings.value("timeout_read",15000).toInt();
	timeout_write=settings.value("timeout_write",15000).toInt();
	settings.endGroup();
#if defined(Q_OS_UNIX)
	settings.beginGroup("POSTSCRIPT");
	gsBin = settings.value("gs_bin","/usr/local/bin/gs").toString();

	spoolDIR = settings.value("spool_dir","//var/log/spool/cups/tmp//").toString();
	settings.endGroup();
	settings.beginGroup("PDF");
	pdfTk = settings.value("pdfTK","/usr/local/bin/pdftk").toString();
	settings.endGroup();
#elif defined(Q_OS_WIN)
	settings.beginGroup("POSTSCRIPT");
	gsBin = settings.value("gs_bin","C:\\Program Files\\gs\\gs8.70\\bin\\gswin32c.exe").toString();
	spoolDIR = settings.value("spool_dir","c:\\spool\\").toString();
	settings.endGroup();
	settings.beginGroup("PDF");
	pdftkBin = settings.value("pdfTK","c:\\Tools\\pdftk.exe").toString();
	settings.endGroup();
#endif
	mainPDF.append(spoolDIR).append("%1.pdf");
	outPDF.append(spoolDIR). append("%1_out.pdf");
}






void dController::write_settings()
{
    
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,"Техносервъ","Защищенный принтер");
    settings.setIniCodec("UTF-8");

    settings.beginGroup("SERVICE" );
    settings.setValue("server",serverHostName);
    settings.setValue("port",serverPort);
    settings.setValue("timeout_connect",timeout_connect);
    settings.setValue("timeout_read",timeout_read);
    settings.setValue("timeout_write",timeout_write);
    settings.endGroup();

    settings.beginGroup("POSTSCRIPT" );
    settings.setValue("gs_bin",gsBin);
    //settings.setValue("output_fg_PDF",mainPDF);
    //settings.value("output_PDF",outPDF);
    settings.setValue("spool_dir",spoolDIR);
    settings.endGroup();

    settings.beginGroup("PDF");
    settings.setValue("pdfTK",pdftkBin);
    settings.endGroup();

}
void dController::convert2PDF(QString &in_file)
{
	ProcessT *proc = new ProcessT();
	// Обработчик сообщений файлового потока
	connect(proc,SIGNAL(commandOutput(int,QString)),this,SLOT(parseCnvThread(int,QString)));

    	args.clear();
	args.append( "-q" );
	args.append( "-dQUIET" );
	args.append( "-dNOPAUSE" );
	args.append( "-dPARANOIDSAFER" );
	args.append( "-dBATCH" );
	args.append("-r300");
	args.append( "-sDEVICE=pdfwrite" );
	mainPDF=mainPDF.arg(SID);
	outPDF=outPDF.arg(SID);
	qDebug() << mainPDF;
	args.append(QString("-sOutputFile=%1").arg(mainPDF));
	args.append("-c");
	args.append(".setpdfwrite");
	args.append("-f");
	args.append(in_file);
	proc->execute(gsBin, args,QProcess::MergedChannels);
	qDebug() <<Q_FUNC_INFO << "convert input file to pdf";
}
void dController::mergeTwoPDF(QString &fg_file,QString &bg_file,QString &out_file)
{
	ProcessT *proc = new ProcessT();
	// Обработчик сообщений файлового потока
	connect(proc,
			SIGNAL(commandOutput(int,QString)),
		    this,
			SLOT(parseMergeThread(int,QString))
			);
	args.clear();
	//pdftk in.pdf background back.pdf output out.pdf
	//pdftk in.pdf stamp back.pdf output out.pdf
	args.append(fg_file);
	args.append("stamp");
	args.append(bg_file);
	args.append("output");
	args.append(out_file);
	proc->execute(pdftkBin, args,QProcess::MergedChannels);
	qDebug() << Q_FUNC_INFO <<"pdftk " << fg_file <<" stamp " <<bg_file <<" output " <<out_file;
}
dController::~dController()
{
    qDebug() << Q_FUNC_INFO;
    write_settings();
}
void dController::printWithTemplate (int RowId)
{
    /*
    qDebug()<<Q_FUNC_INFO;
    //TODO это должно быть в отдельном классе !!!
    scene = new QGraphicsScene();

    page_orient= false;
    margin_top = MM_TO_POINT(15);
    margin_bottom=MM_TO_POINT(15);
    margin_left=MM_TO_POINT(35);
    margin_right=MM_TO_POINT(15);
    page_width=210;
    page_height=297;
    scene->clear();
    scene->setSceneRect(0, 0, MM_TO_POINT(page_width),MM_TO_POINT(page_height));
    QGraphicsRectItem *border_rect = new QGraphicsRectItem (QRectF(margin_left, margin_top, MM_TO_POINT(page_width)-margin_left-margin_right,MM_TO_POINT(page_height)-margin_top-margin_bottom));
    qDebug() <<"margin_rect"<< margin_left << margin_top << margin_left<<margin_right;
    border_rect->setPen(QPen(Qt::black,2,Qt::DotLine));
    border_rect->setBrush(QBrush(Qt::white));
    border_rect->setOpacity(0.5);
    border_rect->setZValue(0);
    border_rect->setData(ObjectName, "Border");
    scene->addItem(border_rect);


	    SimpleItem* Gritem = new SimpleItem;
	    Gritem->setPos(MM_TO_POINT(180), MM_TO_POINT(0));
	    Gritem->setPrintFrame(false);
	    Gritem->setText(QStringList()<<QString("Секретно")<<QString("Пункт 12"));

	    Gritem->setFlag(QGraphicsItem::ItemIsMovable);

	    Gritem->setParentItem(border_rect);

	    SimpleItem* MBitem = new SimpleItem;
	    MBitem->setPos(MM_TO_POINT(5), MM_TO_POINT(280));
	    MBitem->setPrintFrame(false);
	    MBitem->setText(QStringList()<<QString("МБ №-%1").arg(templ));
	    MBitem->setFlag(QGraphicsItem::ItemIsMovable);

	    MBitem->setParentItem(border_rect);

	scene->update();

    QPrinter printer;
    QString with_t;
    with_t.append(spoolDIR).append("compl_teml2.pdf");

     printer.setOutputFormat(QPrinter::PdfFormat);
     printer.setOutputFileName(with_t);
     printer.setPageSize(QPrinter::A4);
     QPainter painter(&printer);
     scene->render(&painter);

     //QString in_file =mainPDF;
     //in_file.arg(QString(SID));
     qDebug() <<Q_FUNC_INFO<< mainPDF << outPDF;
     this->mergeTwoPDF(mainPDF,with_t,outPDF);
     // Рисуем последнюю страничку
     scene->clear();
    border_rect = new QGraphicsRectItem (QRectF(margin_left, margin_top, MM_TO_POINT(page_width)-margin_left-margin_right,MM_TO_POINT(page_height)-margin_top-margin_bottom));
    qDebug() <<"margin_rect"<< margin_left << margin_top << margin_left<<margin_right;
    border_rect->setPen(QPen(Qt::black,2,Qt::DotLine));
    border_rect->setBrush(QBrush(Qt::white));
    border_rect->setOpacity(0.5);
    border_rect->setZValue(0);
    border_rect->setData(ObjectName, "Border");
    scene->addItem(border_rect);


	    SimpleItem* Lastitem = new SimpleItem;
	    Lastitem->setPos(MM_TO_POINT(40), MM_TO_POINT(200));
	    Lastitem->setPrintFrame(true);
	    Lastitem->setText(QStringList()<<QString("Секретно")
							 <<QString("Пункт 12")
							 <<QString("Исполнитель:ФИО исполнителя")
							 <<QString("Отпечатал:ФИО кто печатал")
							 <<QString("Телефон:2-63-15")
							 <<QString("Инв.№ 12/13")
							 <<QString("Дата: 09.09.09'")
							 <<QString("МБ №-%1").arg(templ)
							);

	    Lastitem->setFlag(QGraphicsItem::ItemIsMovable);

	    Lastitem->setParentItem(border_rect);
     QPrinter printer2;

     printer2.setOutputFormat(QPrinter::PdfFormat);
     QString last_p;
     last_p.append(spoolDIR).append("last_page.pdf");
     printer2.setOutputFileName(last_p);
     printer2.setPageSize(QPrinter::A4);
     QPainter painter2(&printer2);
     scene->render(&painter2);
     */
     emit sayMeGood();
}
void dController::printOverSide(int RowId)
{
    /*
    //TODO это должно быть в отдельном классе !!!
    scene = new QGraphicsScene();

    page_orient= false;
    margin_top = MM_TO_POINT(15);
    margin_bottom=MM_TO_POINT(15);
    margin_left=MM_TO_POINT(35);
    margin_right=MM_TO_POINT(15);
    page_width=210;
    page_height=297;
    scene->clear();
    scene->setSceneRect(0, 0, MM_TO_POINT(page_width),MM_TO_POINT(page_height));

    QGraphicsRectItem *border_rect = new QGraphicsRectItem (QRectF(margin_left, margin_top, MM_TO_POINT(page_width)-margin_left-margin_right,MM_TO_POINT(page_height)-margin_top-margin_bottom));
    qDebug() <<"margin_rect"<< margin_left << margin_top << margin_left<<margin_right;
    border_rect->setPen(QPen(Qt::black,2,Qt::DotLine));
    border_rect->setBrush(QBrush(Qt::white));
    border_rect->setOpacity(0.5);
    border_rect->setZValue(0);
    border_rect->setData(ObjectName, "Border");
    scene->addItem(border_rect);

	    SimpleItem* MBitem = new SimpleItem;
	    MBitem->setPos(MM_TO_POINT(5), MM_TO_POINT(280));
	    MBitem->setPrintFrame(false);
	    MBitem->setText(QStringList()<<QString("МБ №-%1").arg(mb));
	    MBitem->setFlag(QGraphicsItem::ItemIsMovable);

	    MBitem->setParentItem(border_rect);
	    //scene->addItem(MBtem);
	scene->update();

    QPrinter printer;
    printer.setPrinterName(curPrinter); //Печать на реальный принтер
     //printer.setOutputFormat(QPrinter::PdfFormat);
     //printer.setOutputFileName("overside.pdf");
     //printer.setPageSize(QPrinter::A4);
     QPainter painter(&printer);
     scene->render(&painter);
     */
     emit sayMeGood();
}
