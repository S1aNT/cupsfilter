#include "engine.h"
#include "mysocketclient.h"
#include "message.h"

#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtCore/QProcess>
#include <QtCore/QString>
#include <QtCore/QMetaType>
#include <QtCore/QPluginLoader>
#include <QtCore/QTimer>
#include <QtCore/QRegExp>

Engine::Engine(QObject *parent,const QString &app_path,
               const QString &client_name)
                   : e_info(QString())
                   , e_state(false)
                   , m_LocalClient(0)
                   , stopLaunch(false)
                   , m_appPath(QString())
                   , gs_plugin(0)
                   , client_uuid(QString())

{
    qRegisterMetaType<VPrn::MessageType>("MessageType");
    m_appPath = app_path;
    QString ini_path =QString("%1/Technoserv/safe_printer.ini")
                      .arg(m_appPath);
    this->readConfig(ini_path);
    this->clientName = client_name;
}

void Engine::init()
{
    this->setError();
    if (loadPlugin()){
        emit pluginsLoad();
    }

    if (!link_name.isEmpty() &&  !gatekeeper_bin.isEmpty()){
        launchAndConnect();
    }else{
        setError(QObject::trUtf8("Ошибка в файле конфигурации. Проверьте параметры:\n"
                    " Раздел [SERVICE] параметр link_name\n"
                    " Раздел [USED_DIR_FILE] параметр gatekeeper_bin"
                    ));
    }

}


void Engine::setUserMandat(const QString & mandat)
{
    emit ReciveUserMandat();
    // Запись в сокет сообщения запрос доступных уровней секретности для
    Message msg(this);
    msg.setType(VPrn::Que_SEC_LEVEL);
    msg.setMessage( mandat.toUtf8() ); // Пробразуем в QByteArray
    sendMessage2LocalSrv(msg);
}

void Engine::sendMessage2LocalSrv(const Message &s_msg)
{
    if (m_LocalClient){
        m_LocalClient->sendMessage(s_msg);
    }
}

//------------------------------- PRIVATE --------------------------------------
void Engine::readConfig(const QString &ini_file)
{
    this->setError();
    if (QFile::exists(ini_file)){
        QSettings settings (ini_file,QSettings::IniFormat);
        settings.setIniCodec("UTF-8");
        settings.beginGroup("SERVICE");
        serverHostName = settings.value("server").toString();
        serverPort     = settings.value("port").toInt();
        link_name      = settings.value("link_name").toString();
        settings.endGroup();

        settings.beginGroup("POSTSCRIPT");
        gsBin = settings.value("gs_bin").toString();
        settings.endGroup();
        settings.beginGroup("PDF");
        pdftkBin = settings.value("pdfTK").toString();
        settings.endGroup();

        settings.beginGroup("USED_DIR_FILE");
        spoolDir       = settings.value("spool_dir").toString();
        gatekeeper_bin = settings.value("gatekeeper_bin").toString();
        //rcp_file = settings.value("rcp_file").toString();
        settings.endGroup();


        settings.beginGroup("TEMPLATES");
        local_t_path  = settings.value("local_templates").toString();
        global_t_path = settings.value("global_templates").toString();
        settings.endGroup();

    }else{
        setError(QObject::trUtf8("Файл с настройками программы %1 не найден!").arg(ini_file));
    }
}

bool Engine::loadPlugin()
{   
    QDir pluginsDir(m_appPath);
    Igs_plugin *gs_plugin_Interface;

    if (pluginsDir.dirName().toLower() == "debug" ||
        pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
#if defined(Q_OS_MAC)
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
            bool needUnloadPlugin = true;
            {
                if (!gs_plugin){
                    /// Загрузка плагина работы с ghostscript
                    gs_plugin_Interface = qobject_cast<Igs_plugin *> (plugin);
                    if (gs_plugin_Interface){
                        needUnloadPlugin = false;
                        gs_plugin = gs_plugin_Interface;

                    }
                }
            }
            if ( needUnloadPlugin ){
                // Выгрузим его нафиг не наш он плагин, сто пудово :)
                qDebug() << QObject::trUtf8("Plugin's [%1] unload").arg(pluginsDir.absoluteFilePath(fileName));
                pluginMessageer.unload();
            }
        }
    }
    return (gs_plugin);
}

void Engine::setError(const QString &info)
{
    e_info  = info;
    e_state = true;   
}

void Engine::setError()
{
    e_state = false;
    e_info  = QString();
}

void Engine::launchAndConnect()
{
    setError();
    if (!m_LocalClient){

        m_LocalClient = new mySocketClient();
        connect (m_LocalClient, SIGNAL(checkPointChanged(MyCheckPoints)),
                 this, SLOT(do_checkPointChanged(MyCheckPoints))
                 );
        connect (m_LocalClient, SIGNAL(messageReady( const Message &)),
                 this, SLOT(parseMessage(const Message &))
                 );
    }
    //emit infoMessage(QObject::trUtf8("Устанавливаем соединение с GateKeeper!"));
    // Устанавливаем соединение с локальным сервером
    m_LocalClient->connectToServer(link_name);
}

//-------------------------------- PRIVATE SLOTS -------------------------------
void Engine::do_checkPointChanged(MyCheckPoints r_cpoint)
{
    Message msg(this);
    msg.clear();
    switch (r_cpoint){

    case VPrn::loc_Connected:
        //emit infoMessage(QObject::trUtf8("Соединение с GateKeeper установленно,запрос авторизации "));
        emit LocalSrvRegistr();
        //Запрос авторизации
        msg.setType(VPrn::Que_Register);
        msg.setMessage( clientName.toUtf8() ); // Пробразуем в QByteArray
        sendMessage2LocalSrv(msg);
        break;

    case VPrn::loc_ServerNotFound:
        if (!stopLaunch){
            //emit infoMessage(QObject::trUtf8("GateKeeper не запущен,загружаем его!"));

            QProcess proc;
            // Запуск процесса старта сервера
            if (proc.startDetached (gatekeeper_bin)){
                // Попытка номер 2
                stopLaunch = true;

            }else{
                setError(proc.errorString());
            }
            QTimer::singleShot(3000,this,SLOT(launchAndConnect()));
        }
        break;
    }

}

void Engine::parseMessage(const Message &r_msg)
{
    qDebug() << Q_FUNC_INFO << "Recive message."
            << "\nType: "  << r_msg.type()
            << "\nBody: "  << r_msg.messageData();

    Message msg(this);
    msg.clear();
    QString str;

    switch (r_msg.type()){
    case VPrn::Ans_PRINTER_LIST:
        str.append(r_msg.messageData());
        emit getPrinterList(str);
        //emit infoMessage(QObject::trUtf8("Получен список принтеров доступных пользователю"));
        emit RecivePrintersList();
        break;
    case VPrn::Ans_STAMP_LIST: // Получили названия уровней секретности, сохраним
        str.append(r_msg.messageData());
        secLevelList << str.split(";:;");

        //emit infoMessage(QObject::trUtf8("Получен список уровней секретности доступный пользователю"));
        emit ReciveSecLevelList();
        //Запрос списка принтеров доступных пользоваетелю
        msg.setType(VPrn::Que_GET_PRINTER_LIST);
        sendMessage2LocalSrv(msg);
        break;

    case VPrn::Ans_Register: // Клиент зарегистрирован
        // Сохраним полученный от GateKeeper уникальный номер
        client_uuid.clear();
        client_uuid.append(r_msg.messageData());
        emit LocalSrvRegistr();
        // Запрос у локального сервера его состояния возможные ответы:
        // Не готов к работе (Подробностив теле сообщения)
        // Готов к работе (Есть auth данные пользователя, есть связь с демоном)
        // Частично готов (Есть имя пользователя,есть связь с демоном, нет мандата

        msg.setType(VPrn::Que_ServerStatus);
        sendMessage2LocalSrv(msg);
        break;
    case VPrn::Err_Message:
        // Получили сообщение об ошибке Формат: КОД~~описание
        break;
    case VPrn::Ans_SrvStatusNotReady:      
        str.append(r_msg.messageData());
        emit gk_notReady(QObject::trUtf8("GateKeeper не готов к работе по причине [%1]")
                         .arg(str));
        break;
    case VPrn::Ans_SrvStatusPartReady:   
        str.append(r_msg.messageData());
        emit needAuthUser(str);
        emit RemoteDemonRegistr();
        break;
    case VPrn::Ans_SrvStatusFullReady:
        str.append(r_msg.messageData());
        // Разберем ответ сервера, в формате : имя;:;мандат
        QRegExp rx("(.+);:;(.+)");
        //rx.setMinimal(true);
        if(rx.indexIn(str) != -1){
            QString uName   = rx.cap(1);
            QString uMandat = rx.cap(2);
            emit gk_fullReady(uName,uMandat);
            //Запрос уровней секретности для мандата
            setUserMandat(uMandat);
            emit ReciveUserName();
            emit RemoteDemonRegistr();
        }else{
            emit infoMessage(QObject::trUtf8("Ошибка при получении имени и мандата текщего пользователя"));
        }
        break;

    }
}



//void Engine::setState(LocalSocketState state)
//{
//    m_state = state;
//    emit stateChanged(m_state);
//}

