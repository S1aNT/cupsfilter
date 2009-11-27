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

bool Mediator::isAuth()
{
    if (!user_name.isEmpty() && !user_mandat.isEmpty()){
        return true;
    }else {
        return false;
    }
}


bool Mediator::isHaveMandatList()
{
    if (mandatModel->rowCount()>0){
        return true;
    }else{
        return false;
    }
}

void Mediator::do_User_name_mandat(QString &userName,QString &userMandat)
{
    if (!userName.isEmpty()){
        user_name=userName;
        if (!userMandat.isEmpty()){
            user_mandat=userMandat;
            emit StateChanged(authPluginLoaded);
        }
    }
}

void Mediator::plugin_init()
{
    if (auth_plugin){
#if defined(Q_OS_UNIX)
        QString t_name;
        t_name="c:\\temp\\session_ticket";
        auth_plugin->init(t_name);
#elif defined(Q_OS_WIN)
        qDebug() << "start   auth_plugin->init();";
        auth_plugin->init();

#endif
        emit StateChanged(authPluginLoaded);
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
            emit StateChanged(gsPluginLoaded);
        }
    }
}




void Mediator::loadPlugin(const QString &app_dir)
{
    //QString msg=QObject::trUtf8("Пример ошибки");
    //emit error(msg);

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
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        if (plugin) {
            connect(plugin, SIGNAL(error(QString )), this, SLOT (parseError(QString )));
            net_plugin_Interface = qobject_cast<Inet_plugin *> (plugin);
            if (net_plugin_Interface) {

                connect(plugin, SIGNAL(serverResponse(QString &)), this, SLOT(parseServerResponse(QString &)));
                // Сохраним указатель на плагин как данные класса
                net_plugin=net_plugin_Interface;

                QString host = QObject::trUtf8("192.168.112.2");
                //QString host = QObject::trUtf8("127.0.0.1");
                net_plugin->init(host, 4242,sid);
                emit StateChanged(netPluginLoaded);
            }

            gs_plugin_Interface = qobject_cast<Igs_plugin *> (plugin);
            if (gs_plugin_Interface) {
                //connect(plugin, SIGNAL(serverResponse(QString)), this, SLOT(showInfo(QString)));
                gs_plugin=gs_plugin_Interface;
            }
            auth_plugin_Interface = qobject_cast<Auth_plugin *>(plugin);
            if (auth_plugin_Interface){
                connect(plugin,SIGNAL(get_User_name_mandat(QString &,QString &)),this,SLOT(do_User_name_mandat(QString&,QString&)));
                connect(plugin,SIGNAL(needShowAuthWindow(QString &)),this,SLOT(getMeMandatList(QString &)));
                auth_plugin =auth_plugin_Interface;
            }
        }
    }
}


//************************************** private function ***************************************
void Mediator::readGlobal()
{

}

//*************************************** private slots *****************************************
void Mediator::getMeMandatList(QString &userName)
{
    if (this->connect_state){
        QString message =QString("/cmd;:;%1;:;%2;:;%3").arg(sid).arg(GET_MANDAT_LIST_CMD,0,10).arg(userName);
        qDebug() << Q_FUNC_INFO << message;
        net_plugin->sendData(message);
        this->user_name=userName;
    }
}
void Mediator::parseError(QString msg)
{
    this->valid_status = false;
    emit error(msg);
}

void  Mediator::parseServerResponse(QString &responce_msg)
{
    QRegExp rx("^/(\\d+);:;(.*)$");
    QString msg;
    QString cmd;
    QString body;
    if(rx.indexIn(responce_msg) != -1)
    {
        cmd =rx.cap(1);
        body = rx.cap(2);
        qDebug() <<Q_FUNC_INFO<< cmd<<body;
        switch (cmd.toInt()){
            case STAMP_LIST_ANS:
                //this->stampModel->setStringList(QStringList() << body.split(";:;"));

            msg =QString("/cmd;:;%1;:;%2;:;%3;:;CC").arg(sid).arg(GET_PRINTER_LIST_CMD,0,10).arg(this->user_name).arg(this->user_mandat);
            qDebug() << Q_FUNC_INFO << msg;
            net_plugin->sendData(msg);

                break;
            case REGISTER_ANS: // Соединились с сервером безопастности
                 emit StateChanged(connectedToDemon);
                 qDebug() <<Q_FUNC_INFO<< "StateChanged(connectedToDemon);";
                 this->plugin_init();

                 this->connect_state=true;
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
                //emit printersNotFound();
                this->user_name="usr1";
                this->user_mandat="CC";
                msg =QString("/cmd;:;%1;:;%2;:;SL9PRT.NEW;:;%3;:;%4").arg(sid).arg(AUTHOR_USER,0,10).arg(this->user_mandat).arg(this->user_name).arg("usr1");
                qDebug() << Q_FUNC_INFO << msg;
                net_plugin->sendData(msg);

                break;
            case MANDAT_LIST_ANS:// Список мандатов к которым допущен пользоватль
                emit needShowAuthWindow(this->user_name);
                qDebug() <<body.split(";:;");
                this->mandatModel->setStringList(QStringList() << body.split(";:;"));
                this->test_cmd();
                break;
            case MANDAT_LIST_EMPTY_ANS:// У данного пользователя нет ни одного мадата
                msg =QObject::trUtf8("У данного пользователя нет ни одного мандата");
                this->parseError(msg);
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

//**************************************** protected ******************************************
void Mediator::createModels()
{
    stampModel = new  QStringListModel(this);
    doc_model = new QStandardItemModel(this);
    mandatModel = new  QStringListModel(this);
}

void Mediator::test_cmd()
{
    if (this->connect_state){
        QString message =QString("/cmd;:;%1;:;%2;:;CC").arg(sid).arg(GET_SEC_LEVEL_CMD,0,10).arg(this->user_mandat);
        qDebug() << Q_FUNC_INFO << message;
        net_plugin->sendData(message);


    }
}
