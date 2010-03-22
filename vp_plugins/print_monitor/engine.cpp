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

Engine::Engine(QObject *parent,const QString &app_path)
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

}

void Engine::init()
{
    this->setError();
    this->loadPlugin();

    if (!link_name.isEmpty() &&  !gatekeeper_bin.isEmpty()){
        launchAndConnect();
    }else{
        setError(tr("Ошибка в файле конфигурации. Проверьте параметры:\n"
                    " Раздел [SERVICE] параметр link_name\n"
                    " Раздел [USED_DIR_FILE] параметр gatekeeper_bin"
                    ));
    }

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
        setError(tr("Файл с настройками программы %1 не найден!").arg(ini_file));
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
                qDebug() << tr("Plugin's [%1] unload").arg(pluginsDir.absoluteFilePath(fileName));
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
    emit infoMessage(tr("Устанавливаем соединение с GateKeeper!"));
    // Устанавливаем соединение с локальным сервером
    m_LocalClient->connectToServer(link_name);
}

//-------------------------------- PRIVATE SLOTS -------------------------------
void Engine::do_checkPointChanged(MyCheckPoints r_cpoint)
{
    switch (r_cpoint){

    case VPrn::loc_Connected:
        emit infoMessage(tr("Соединение с GateKeeper установленно,запрос авторизации "));
        break;

    case VPrn::loc_ServerNotFound:
        if (!stopLaunch){
            emit infoMessage(tr("GateKeeper не запущен,загружаем его!"));

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
    Message msg(this);
    msg.clear();
    switch (r_msg.type()){
    case VPrn::Ans_Register: // Клиент зарегистрирован

        // Сохраним полученный от GateKeeper уникальный номер
        client_uuid.fromUtf8(r_msg.messageData());
        emit infoMessage(tr("GateKeeper зарегистрировал клиента.UUID = [%1]")
                         .arg(client_uuid));
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
    case VPrn::Ans_ServerStatus:
        QString info;
        info.fromUtf8(r_msg.messageData());
        // Разберем ответ сервера, в формате :
        break;
    }
}



//void Engine::setState(LocalSocketState state)
//{
//    m_state = state;
//    emit stateChanged(m_state);
//}

