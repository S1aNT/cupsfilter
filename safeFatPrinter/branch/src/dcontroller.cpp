#include "dcontroller.h"

dController::dController(QObject * parent)
{
    // формируем SID
    QUuid uSID=QUuid::createUuid () ;  //generate SID
    SID=uSID.toString().replace("{","").replace("}","");

    this->read_settings();
    this->createModel();
    this->getUserNameMandat();
    this->createPrinterList();
    netAgent = new net_agent(this);
    connect (netAgent,SIGNAL(serverSay(QString&)),this,SLOT(readServerResponce(QString&)));
    connect(this,SIGNAL(sendServerCmd(commands_t,QString&)),netAgent,SLOT(send2Server(commands_t,QString&)));
}

//----------------------------------------  Public   functions --------------------------------------------------------------------
bool   dController::isUser_Mandat() const
{
    if (!Mandat.isEmpty() && !UserName.isEmpty()){
	return true;
    }else {
	return false;
    }
}

// ---------------------------------------      Public SLOTS     -------------------------------------------------------------------
void dController::connect2Demon()
{
   netAgent->do_connect(serverHostName,serverPort,SID);
}
 void dController::convert2PDF(QString &in_file)
{

    QStringList args;
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

    QStringList args;
    // Файл объединен
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


//----------------------------------------	  Private SLOTS   --------------------------------------------------------------------
void dController::done_ftp_transfer()
{
     QString msg= QObject::trUtf8("Успешно полученны глобальные шаблоны.");
     init_done(GET_FROM_FTP_STEP,msg);
}

void dController::parseCnvThread(int Code,QString output )
{
    QString msg;
	if (Code != QProcess::NormalExit) {
	    msg =QObject::trUtf8("Ошибка предпечатной подготовки.\n%1:\nКод %2").arg(output).arg(Code,0,10);
	    emit error(Code,msg);
	}else{
	// Файл преобразован в pdf
	    QString msg= QObject::trUtf8("Исходный файл успешно конвертирован в pdf");
	    emit init_done(CONVERT_STEP,msg);
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
	emit inputFileMerged();
	    qDebug() << QObject::tr("Успешно завершено добавление шаблона.");
	}
}

void dController::readServerResponce(QString &line)
{
    QRegExp rx("^/(\\d+);:;(.+)$");
    QString msg;
    QString cmd;
    QString body;
    if(rx.indexIn(line) != -1)
    {
	cmd =rx.cap(1);
	body = rx.cap(2);
	qDebug() <<Q_FUNC_INFO<< cmd<<body;
	switch (cmd.toInt()){
	    case STAMP_LIST_ANS:
		this->stampModel->setStringList(QStringList() << body.split(";:;"));
		break;
	    case REGISTER_ANS: // Соединились с сервером безопастности
		 msg= QObject::trUtf8("Успешно соединились с сервером безопастности");
		emit init_done(CONN_STEP,msg);
		this->getStampName(); // Получим с сервера название уровней безопастности
		this->getPrinterCount();
		break;
	    case MB_EXIST_AND_BRAK_ANS:
		this->insertDocToModel(body);
		emit mbNumberExist(doc_model->rowCount());
		break;
	    case MB_EXIST_AND_NOT_BRAK_ANS:
		emit mbNumberExist(DOC_PRINTED);
		break;
	    case MB_NOT_EXIST_ANS:
		emit mbNumberNotExist();
		break;
	    case PRINTER_LIST_EMPTY:
		emit printersNotFound();
		break;
	}
    }else{
	// emit error
    }
}

//----------------------------------------  Private functions --------------------------------------------------------------------
void dController::read_settings()
{ // Читаем файл настроек
    QString ini_path =QString(QObject::trUtf8("%1\\Technoserv\\safe_printer.ini")).arg(QCoreApplication::applicationDirPath());
    //ini_path.arg(QCoreApplication::applicationDirPath());
    QSettings settings (ini_path,QSettings::IniFormat);
 //   QSettings settings(QSettings::IniFormat, QSettings::UserScope,"Technoserv","safe_printer");

    settings.setIniCodec("UTF-8");
    settings.beginGroup("SERVICE");
    serverHostName = settings.value("server","127.0.0.1").toString();
    serverPort = settings.value("port",4242).toInt();
    timeout_connect =settings.value("timeout_connect",5000).toInt();
    timeout_read=settings.value("timeout_read",15000).toInt();
    timeout_write=settings.value("timeout_write",15000).toInt();

    settings.endGroup();
    settings.beginGroup("PERIOD");
    end_date =settings.value("end_date",QDate::currentDate ()).toDate();
    QDate begin(end_date.year(), 1,1);
    begin_date =settings.value("begin_date",begin).toDate();
    settings.endGroup();
#if defined(Q_OS_UNIX)
    settings.beginGroup("POSTSCRIPT");
    gsBin = settings.value("gs_bin","/usr/local/bin/gs").toString();
    settings.endGroup();
    settings.beginGroup("PDF");
    pdftkBin = settings.value("pdfTK","/usr/local/bin/pdftk.py").toString();
    settings.endGroup();
    settings.beginGroup("USED_DIR_FILE");
    spoolDIR = settings.value("spool_dir","/var/log/spool/cups/tmp/").toString();
    printer_list_pid=settings.value("printers_pid","/var/log/pid/get_printer.pid").toString();
    settings.endGroup();
    settings.beginGroup("TEMPLATES");
    localTemplates=settings.value("local_templates","$HOME/vprinter/local_templates/").toString();
    globalTemplates=settings.value("global_templates","$HOME/vprinter/global_templates/").toString();
    ftpTemplatesDir=settings.value("ftp_templates_dir","ftp://127.0.0.1/pub/templates/").toString();
    settings.endGroup();
#elif defined(Q_OS_WIN)
    settings.beginGroup("POSTSCRIPT");
    gsBin = settings.value("gs_bin","C:\\Program Files\\gs\\gs8.70\\bin\\gswin32c.exe").toString();
    settings.endGroup();
    settings.beginGroup("PDF");
    pdftkBin = settings.value("pdfTK","c:\\Tools\\pdftk.exe").toString();
    settings.endGroup();
    settings.beginGroup("USED_DIR_FILE");
    spoolDIR = settings.value("spool_dir","c:\\spool\\").toString();
    printer_list_pid=settings.value("printers_pid","c:\\spool\\get_printer.pid").toString();
    settings.endGroup();
    settings.beginGroup("TEMPLATES");
    localTemplates=settings.value("local_templates","%APPDATA%/vprinter/local_templates/").toString();
    globalTemplates=settings.value("global_templates","%APPDATA%/vprinter/global_templates/").toString();
    ftpTemplatesDir=settings.value("ftp_templates_dir","ftp://127.0.0.1/pub/templates/").toString();
    settings.endGroup();
#endif
    mainPDF.append(spoolDIR).append("%1.pdf");
    outPDF.append(spoolDIR). append("%1_out.pdf");

    qDebug() << Q_FUNC_INFO << settings.fileName() ;

    qDebug() << "\nSERVICE"
	    << "\nserverHostName=" <<serverHostName
	    << "\nserverPort="<< serverPort
	    << "\ntimeout_connect=" << timeout_connect
	    << "\ntimeout_read=" <<timeout_read
	    << "\ntimeout_write" <<timeout_write;
    qDebug() << "\nPERIOD"
	    <<"\nbegin_date=" << begin_date.toString("dd.MM.yyyy")
	    <<"\nend_date" <<end_date.toString("dd.MM.yyyy");
    qDebug()   <<"\nPOSTSCRIPT"
	    "\ngsBin="<< gsBin;
    qDebug()   <<"\nPDF"<< "pdftkBin="<< pdftkBin;
    qDebug()<<"\nUSED_DIR_FILE"
    << "\nspoolDIR="    << spoolDIR
    << "\nprinter_list_pid=" << printer_list_pid;
    qDebug() << "\nTEMPLATES"
    << "\nlocalTemplates="<< localTemplates
    << "\nglobalTemplates="<< globalTemplates
    << "\nftpTemplatesDir="<< ftpTemplatesDir;

}
void dController::createModel()
{
    stampModel = new  QStringListModel(this);
    doc_model = new QStandardItemModel(this);

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
	    << QObject::trUtf8("Штамп последней стр.")
	    << QObject::trUtf8("Список рассылки")
	    << QObject::trUtf8("Статус документа")
	    << QObject::trUtf8("Брак страниц")
	    << QObject::trUtf8("Брак документа");
    doc_model->setHorizontalHeaderLabels(header);
   // this->insertDocToModel();
}
void dController::insertDocToModel()
{ // add empty row to model
    doc_model->insertRow(doc_model->rowCount());
}
void dController::insertDocToModel(QString &item)
{
    qDebug() << Q_FUNC_INFO << item;
    
    if (!item.isEmpty() && item.contains(";:;", Qt::CaseInsensitive)){
        QStringList itemList= item.split(";:;");
        if (itemList.size()>0){
            QList<QStandardItem *> cells;
            int Pos;

            for (int i = 0; i <itemList.size() ; ++i) {
                QStandardItem * cell_item= new QStandardItem();
                cells.append(cell_item);
            }

            for (int i = 0; i <itemList.size() ; ++i) {
                QString tmp_str=itemList.at(i);
                qDebug() << tmp_str;
                if (!tmp_str.isEmpty() && tmp_str.contains("=")){
            	    QStringList list_2 = tmp_str.split("=");
            	        qDebug() << list_2.size();
            	    	Pos =header.indexOf(list_2.at(0)); // Ищем индекс
            		QStandardItem *cell_item =cells.at(Pos);
            		cell_item->setData(QVariant(list_2.at(1)),Qt::EditRole);
            		qDebug() << "Key" << list_2.at(0) << " Value " << list_2.at(1);
            	    
                }
            }
            doc_model->appendRow (cells);
        }

    }
}

void dController::checkMB(QString  mb)
{
    // Сервер проверят существует ли в базе документ с таким МБ за год
    // если есть то он возвращает строку содержащую все его данные
    QDate dt_end;
    dt_end=QDate::currentDate (); // Текущая дата
    QDate dt_begin (dt_end.year (), 1,1); // Первое января

    QString cmd_body=QString(QObject::trUtf8("%1;:;%2;:;%3")).arg(mb).arg(dt_begin.toString("dd.MM.yyyy"),dt_end.toString("dd.MM.yyyy"));
    qDebug() <<Q_FUNC_INFO<<cmd_body;
    emit sendServerCmd(IS_MB_EXIST_CMD,cmd_body);
}

void dController::getStampName()
{// Получим название уровней секретности
    QString cmd_body;
    cmd_body=QObject::trUtf8("%1;:;%2").arg(UserName,Mandat);
    emit sendServerCmd(GET_SEC_LEVEL_CMD,cmd_body);
}
void dController::getPrinterCount()
{
    QString cmd_body;
    cmd_body=QObject::trUtf8("%1;:;%2").arg(UserName,Mandat);
    emit sendServerCmd(GET_PRINTER_LIST_CMD,cmd_body);
}

void dController::getUserNameMandat()
{
    // Выполним функции из ldap_auth.dll
    UserName=QObject::trUtf8("sva");
    Mandat=QObject::trUtf8("SYSPROG");
}
void dController::createPrinterList()
{
    QList<QPrinterInfo> plist;
    plist = QPrinterInfo::availablePrinters () ;
    for (int i = 0; i < plist.size(); ++i) {
	if (plist.at(i).printerName()!="Защищенный принтер"){
	   this->printerList.append(plist.at(i).printerName());
	}
    }
}


void dController::getTemplatesFromServer(QString &saveTo)
{
    QDir dir;
    if (dir.mkpath(saveTo))
    {
	QFileInfoList list = dir.entryInfoList();
	//Очистил каталог назначения
	for (int i = 0; i < list.size(); ++i) {
	    QFileInfo fileInfo = list.at(i);
	    dir.remove(fileInfo.fileName());
	}
	Spider spider;
	spider.getDirectory(QUrl(this->ftpTemplatesDir),this->globalTemplates);
	QObject::connect(&spider, SIGNAL(done()), this, SLOT(done_ftp_transfer()));

    }

}
