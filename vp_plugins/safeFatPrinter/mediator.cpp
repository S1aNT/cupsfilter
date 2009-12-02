#include "mediator.h"




Mediator::Mediator(QObject *parent) :
        QObject(parent)
{
    //this->spiderSoul();
    QUuid uSID=QUuid::createUuid () ;  //generate SID
    sid=uSID.toString().replace("{","").replace("}","");
    createModels();
}

//************************** public function *****************************************
//                                Геттеры

QString Mediator::getElemTagById(int elem_id)
{
    QString result;
    if (elem_id <= elemTag.size() && elem_id >0){
        QMapIterator<QString, int> i (elemTag);
         while (i.hasNext()) {
             i.next();
             if (i.value() == elem_id){
                 result = i.key();
             }
         }
    }
    return result;
}

int Mediator::getElemIdByName(const QString elem_name)
{
    int result=0; // Если в списке запрошенного элемента нет то возвращаем 0
    if (!elem_name.isEmpty() && elemTag.contains(elem_name)){
        result= elemTag.value(elem_name);
    }
    return result;
}

//************************************************************************************

void Mediator::do_User_name_mandat(QString &userName,QString &userMandat)
{
    QString msg= QObject::trUtf8("Плагин: [Auth_User] успешно загружен.");
    emit pluginMessage(msg);

    qDebug() << Q_FUNC_INFO << userName <<userMandat;
    if (!userName.isEmpty()){
        user_name=userName;
        if (!userMandat.isEmpty()){
            user_mandat=userMandat;
            this->getSecretLevelName();

        }else{
            this->getMeMandatList(userName);
        }
    }
}

void Mediator::setUserMandat(QString mnd)
{
    user_mandat=mnd;
    // Получим название уровней секретности
    this->getSecretLevelName();
}

void Mediator::plugin_init()
{
    qDebug() << Q_FUNC_INFO << auth_plugin << gs_plugin;
    if (auth_plugin){
#if defined(Q_OS_UNIX)
        auth_plugin->init(ticket_name);
#elif defined(Q_OS_WIN)
        auth_plugin->init();
#endif
        emit StateChanged (authPluginInit);
    }

    if (gs_plugin) {
        if (gs_plugin->init(gsBin, pdftkBin,spoolDIR,rcp_file,sid)){
            emit StateChanged (gsPluginInit);
            QString msg =QObject::trUtf8("Плагин: [Обработки ps и pdf документов] успешно загружен.");
            emit pluginMessage(msg);
        }
    }
}

void Mediator::loadPlugin(const QString &app_dir)
{

    QDir pluginsDir(app_dir);
    Inet_plugin *net_plugin_Interface;
    Igs_plugin *gs_plugin_Interface;
    Auth_plugin *auth_plugin_Interface;

#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#endif
    this->readGlobal(app_dir); // Читаем глобальные настройки

    pluginsDir.cd("plugins");
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)){
        QPluginLoader pluginMessageer(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginMessageer.instance();
        if (plugin) {
            connect(plugin, SIGNAL(error(QString )), this, SIGNAL (error(QString )));
            net_plugin_Interface = qobject_cast<Inet_plugin *> (plugin);
            if (net_plugin_Interface) {

                connect(plugin, SIGNAL(serverResponse(QString &)), this, SLOT(parseServerResponse(QString &)));
                // Сохраним указатель на плагин как данные класса
                net_plugin=net_plugin_Interface;
                net_plugin->init(serverHostName, serverPort,sid);
                QString  msg= QObject::trUtf8("Плагин: [Работы с сетью] успешно загружен.");
                emit pluginMessage(msg);
            }

            gs_plugin_Interface = qobject_cast<Igs_plugin *> (plugin);
            if (gs_plugin_Interface) {
                connect (plugin,SIGNAL(taskStateChanged(TaskState)),this,SLOT(parserGSMessage(TaskState)));
                connect (plugin,SIGNAL(pagesInDoc(int)),this,SLOT(setPageCountInDoc(int)));
                gs_plugin=gs_plugin_Interface;
            }
            auth_plugin_Interface = qobject_cast<Auth_plugin *>(plugin);
            if (auth_plugin_Interface){
                connect(plugin,SIGNAL(get_User_name_mandat(QString &,QString &)),this,SLOT(do_User_name_mandat(QString&,QString&)));
                auth_plugin =auth_plugin_Interface;
            }
        }
    }
}

//************************************** private function ***************************************

void Mediator::readGlobal(const QString &app_dir)
{
    // Читаем файл настроек

    QString ini_path =QString(QObject::trUtf8("%1\\Technoserv\\safe_printer.ini")).arg(app_dir);
    qDebug() << ini_path << endl;
    QSettings settings (ini_path,QSettings::IniFormat);

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
    rcp_file = settings.value("rcp_file","/opt/vprinter/pdf.rcp").toString();
    ticket_fname=settings.value("session_ticket","/tmp/session_ticket").toString();
    settings.endGroup();

    settings.beginGroup("TEMPLATES");
    localTemplates=settings.value("local_templates","/opt/vprinter/local_templates/").toString();
    globalTemplates=settings.value("global_templates","/opt/vprinter/global_templates/").toString();
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
    rcp_file = settings.value("rcp_file","c:\\gs\\pdf.rcp").toString();
    settings.endGroup();

    settings.beginGroup("TEMPLATES");
    localTemplates=settings.value("local_templates","local_templates/").toString();
    globalTemplates=settings.value("global_templates","global_templates/").toString();
    ftpTemplatesDir=settings.value("ftp_templates_dir","ftp://127.0.0.1/pub/templates/").toString();
    settings.endGroup();
#endif

    //mainPDF.append(spoolDIR).append("%1.pdf");
    //outPDF.append(spoolDIR).append("%1_out.pdf");

}

void Mediator::getSecretLevelName()
{
    qDebug() << Q_FUNC_INFO;
    if (this->connect_state){
        QString message =QString("/cmd;:;%1;:;%2;:;%3").arg(this->sid).arg(GET_SEC_LEVEL_CMD,0,10).arg(this->user_mandat);
        qDebug() << Q_FUNC_INFO << message;
        net_plugin->sendData(message);
    }
}

void Mediator::getEnablePrinter()
{
    if (this->connect_state){

        QString msg =QString("/cmd;:;%1;:;%2;:;%3;:;%4").arg(sid).arg(GET_PRINTER_LIST_CMD,0,10).arg(this->user_name).arg(this->user_mandat);
        qDebug() << Q_FUNC_INFO << msg;
        net_plugin->sendData(msg);
    }
}

//*************************************** private slots *****************************************

void Mediator::setPageCountInDoc(int p_count)
{
    this->pagesInDocCount=p_count;
}

void Mediator::parserGSMessage(TaskState state)
{// Обработчик сообщений для плагина работы с ghostscript
    switch(state){
    case converted:
        emit StateChanged(psToPdfConverted);
        break;
    }
}

void Mediator::getMeMandatList(QString &userName)
{
    qDebug() << Q_FUNC_INFO << userName << this->connect_state;
    if (this->connect_state){
        QString message =QString("/cmd;:;%1;:;%2;:;%3").arg(sid).arg(GET_MANDAT_LIST_CMD,0,10).arg(userName);
        qDebug() << Q_FUNC_INFO << message;
        net_plugin->sendData(message);
    }
}

void  Mediator::parseServerResponse(QString &responce_msg)
{
    qDebug() << Q_FUNC_INFO << responce_msg;

    QRegExp rx("^/(\\d+);:;(.*)$");
    QString msg;
    QString cmd;
    QString body;
    QList<QPrinterInfo> plist;
    QStringList tmp_list;

    if(rx.indexIn(responce_msg) != -1)
    {
        cmd =rx.cap(1);
        body = rx.cap(2);
        qDebug() <<Q_FUNC_INFO<< cmd<<body;
        switch (cmd.toInt()){
            case STAMP_LIST_ANS:
                 this->stampModel->setStringList(QStringList() << body.split(";:;"));
                 this->getEnablePrinter();
                break;
            case REGISTER_ANS: // Соединились с сервером безопастности
                 this->connect_state=true;
                 msg =QObject::trUtf8("Успешно соединились с сервером безопасности");
                 emit pluginMessage(msg);
                 emit StateChanged(netPluginInit);
                 this->plugin_init();
                 break;
            case MB_EXIST_AND_BRAK_ANS:
                //this->insertDocToModel(body);
                //emit mbNumberExist(doc_model->rowCount());
                break;
            case MB_EXIST_AND_NOT_BRAK_ANS:
                //emit mbNumberExist(DOC_PRINTED);
                break;
            case MB_NOT_EXIST_ANS:
                //emit mbNumberNotExist();
                break;
            case PRINTER_LIST_ANS:
//TODO написать парсер разбирающий список принтеров отданый сервером
                plist = QPrinterInfo::availablePrinters();

                for (int i = 0; i < plist.size(); ++i) {
                    if (plist.at(i).printerName()!="Защищенный принтер"){
                       tmp_list.append(plist.at(i).printerName());
                    }
                }
                this->printersModel->setStringList(tmp_list);
                emit StateChanged(filledPrinterList);

                break;
        case PRINTER_LIST_EMPTY:
                msg =QObject::trUtf8("У данного пользователя нет ни одного разрешенного принтера");
                emit error(msg);
                break;
            case MANDAT_LIST_ANS:// Список мандатов к которым допущен пользоватль
                qDebug() <<body.split(";:;");
                this->mandatModel->setStringList(QStringList() << body.split(";:;"));
                emit needShowAuthWindow(this->user_name);
                break;
            case MANDAT_LIST_EMPTY_ANS:// У данного пользователя нет ни одного мадата
                msg =QObject::trUtf8("У данного пользователя нет ни одного мандата");
                emit error(msg);
                break;
        }
    }else{
        // emit error
    }

}

//*************************************** public slots*******************************************

void Mediator::convert2pdf(const QString &input_fn)
{
    gs_plugin->convertPs2Pdf(input_fn);
}

void Mediator::authToPrinter(const QString & printer)
{
    //this->user_name="usr1";
    //this->user_mandat="CC";

    QString msg =QString("/cmd;:;%1;:;%2;:;%3;:;%4;:;%5").arg(sid).arg(AUTHOR_USER,0,10).arg(printer).arg(this->user_mandat).arg(this->user_name);
    qDebug() << Q_FUNC_INFO << msg;
    net_plugin->sendData(msg);


}

//**************************************** protected ******************************************
void Mediator::createModels()
{
    stampModel = new  QStringListModel(this);
    doc_model = new QStandardItemModel(this);
    mandatModel = new  QStringListModel(this);
    printersModel = new  QStringListModel(this);
}

void Mediator::fillMap()
{
    elemTag.insert(QObject::trUtf8("N док-та"),1 );
    elemTag.insert(QObject::trUtf8("Название док-та"),2 );
    elemTag.insert(QObject::trUtf8("Гриф"), 3);
    elemTag.insert(QObject::trUtf8("Пункт перечня"),4 );
    elemTag.insert(QObject::trUtf8("Номер копии"), 5);
    elemTag.insert(QObject::trUtf8("Кол-во листов"),6 );
    elemTag.insert(QObject::trUtf8("Исполнитель"), 7);
    elemTag.insert(QObject::trUtf8("Отпечатал"), 8);
    elemTag.insert(QObject::trUtf8("Телефон"), 9);
    elemTag.insert(QObject::trUtf8("Инв. N"), 10);
    elemTag.insert(QObject::trUtf8("Дата распечатки"), 11);
    elemTag.insert(QObject::trUtf8("Получатель N1"), 12);
    elemTag.insert(QObject::trUtf8("Получатель N2"), 13);
    elemTag.insert(QObject::trUtf8("Получатель N3"), 14);
    elemTag.insert(QObject::trUtf8("Получатель N4"), 15);
    elemTag.insert(QObject::trUtf8("Получатель N5"), 16);
    elemTag.insert(QObject::trUtf8("last_page_stamp"), 17);
    elemTag.insert(QObject::trUtf8("recivers_list"), 18);
    elemTag.insert(QObject::trUtf8("doc_status"), 19);
    elemTag.insert(QObject::trUtf8("brak_pages"), 20);
    elemTag.insert(QObject::trUtf8("brak_doc"), 21);
}
