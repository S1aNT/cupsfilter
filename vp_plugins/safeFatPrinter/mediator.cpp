#include "mediator.h"

Mediator::Mediator(QObject *parent) :
        QObject(parent)
{
    //this->spiderSoul();
    QUuid uSID=QUuid::createUuid () ;  //generate SID
    sid=uSID.toString().replace("{","").replace("}","");
    createModels();
    valid_status =  true;
}




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
        QString t_name;
        t_name="c:\\temp\\session_ticket";
        auth_plugin->init(t_name);
#elif defined(Q_OS_WIN)
        qDebug() << "start   auth_plugin->init();";
        auth_plugin->init();
#endif
        //emit StateChanged(authpluginMessageed);
    }

    if (gs_plugin) {
        // remove after debug
        QString gs_bin="C:\\Program Files\\gs\\gs8.70\\bin\\gswin32c.exe";
        QString pdftk_bin="c:\\tools\\pdftk.exe";
        QString temp_folder="c:\\spool";
        QString  rcp_file = "c:\\gs\\pdf.rcp";

        qDebug() << "start gs_init";
        if (gs_plugin->init(gs_bin, pdftk_bin,temp_folder,rcp_file,sid)){
            qDebug() << "true gs_init";
            //emit StateChanged(gspluginMessageed);
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

                //QString host = QObject::trUtf8("192.168.112.2");
                QString host = QObject::trUtf8("127.0.0.1");
                net_plugin->init(host, 4242,sid);
                QString  msg= QObject::trUtf8("Плагин: [Работы с сетью] успешно загружен.");
                emit pluginMessage(msg);
            }

            gs_plugin_Interface = qobject_cast<Igs_plugin *> (plugin);
            if (gs_plugin_Interface) {
                //connect(plugin, SIGNAL(serverResponse(QString)), this, SLOT(showInfo(QString)));
                gs_plugin=gs_plugin_Interface;
            }
            auth_plugin_Interface = qobject_cast<Auth_plugin *>(plugin);
            if (auth_plugin_Interface){
                connect(plugin,SIGNAL(get_User_name_mandat(QString &,QString &)),this,SLOT(do_User_name_mandat(QString&,QString&)));
                //connect(plugin,SIGNAL(needShowAuthWindow(QString &)),this,SLOT(getMeMandatList(QString &)));
                auth_plugin =auth_plugin_Interface;
            }
        }
    }
}


//************************************** private function ***************************************
void Mediator::readGlobal()
{

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
                emit needShowSelectWindow();

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

}

