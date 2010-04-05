#include "engine.h"
#include "mysocketclient.h"
#include "message.h"

#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QFileInfoList>

#include <QtCore/QSettings>
#include <QtCore/QProcess>
#include <QtCore/QString>
#include <QtCore/QMetaType>
#include <QtCore/QPluginLoader>
#include <QtCore/QTimer>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>



Engine::Engine(QObject *parent,const QString &app_path,
               const QString &client_name)
                   : QObject(parent)
                   , e_info(QString())
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
    //Читаем список шаблонов
    QString pref;
    if (!local_t_path.isEmpty()){
        pref = QObject::trUtf8(".Личный");
        setFileList(local_t_path,pref);
    }else{
        emit error(QObject::trUtf8("Ошибка, не задан каталог Личных шаблонов"));
    }
    if (!global_t_path.isEmpty()){
        pref = QObject::trUtf8(".Общий");
        setFileList(global_t_path,pref);
    }else{
        emit error(QObject::trUtf8("Ошибка, не задан каталог Общих шаблонов"));
    }
}

Engine::~Engine()
{
    if (m_LocalClient){
        m_LocalClient->disconnectFromServer();
    }
}

void Engine::init()
{
    if (!link_name.isEmpty() &&  !gatekeeper_bin.isEmpty()){
        launchAndConnect();
    }else{
        emit error(QObject::trUtf8("Ошибка в файле конфигурации. Проверьте параметры:\n"
                                   " Раздел [SERVICE] параметр link_name\n"
                                   " Раздел [USED_DIR_FILE] параметр gatekeeper_bin"));
    }

}

void Engine::prepareFileToPrint(const QString & file_name)
{
    // Запись в сокет сообщения требую  преобразовать ps -> pdf
    Message msg(this);
    msg.setType(VPrn::Que_Convert2Pdf);
    msg.setMessage( file_name.toUtf8() ); // Пробразуем в QByteArray
    sendMessage2LocalSrv(msg);
}

void Engine::setUserMandat(const QString & u_name,const QString & u_mandat)
{
    currentUserName = u_name;
    currentUserMandat = u_mandat;
    emit ReciveUserName();
    emit ReciveUserMandat();

    // Запись в сокет сообщения запрос доступных уровней секретности для
    Message msg(this);
    msg.setType(VPrn::Que_SEC_LEVEL);
    msg.setMessage( currentUserMandat.toUtf8() ); // Пробразуем в QByteArray
    sendMessage2LocalSrv(msg);
}

void Engine::sendMessage2LocalSrv(const Message &s_msg)
{
    if (m_LocalClient){
        m_LocalClient->sendMessage(s_msg);
    }
}

void Engine::authUserToPrinter(const QString &printer_uri)
{
    // Запись в сокет сообщения запрос авторизации пользователя на доступ к ресурсу
    Message msg(this);
    msg.setType( VPrn::Que_AUTHOR_USER  );
    msg.setMessage( findPrinterInDeviceURIList(printer_uri).toUtf8() ); // Пробразуем в QByteArray
    sendMessage2LocalSrv(msg);
}

void Engine::checkMB( const QString &sql_query )
{
    Message msg(this);
    msg.setType( VPrn::Que_IS_MB_EXIST  );
    msg.setMessage( sql_query.toUtf8() );
    sendMessage2LocalSrv(msg);
}

void Engine::registerMB ( const QString &sql_query )
{
    Message msg(this);
    msg.setType( VPrn::Que_RegisterDocInBase  );
    msg.setMessage( sql_query.toUtf8() );
    sendMessage2LocalSrv(msg);
}

void Engine::doMergeDocWithTemplates (QByteArray field_data,bool preview_mode)
{
    Message msg(this);
    if (preview_mode){
       msg.setType( VPrn::Que_CreateFormatedFullDoc  );
   }else{
       msg.setType( VPrn::Que_CreateFormatedPartDoc );
   }

    msg.setMessage( field_data );
    sendMessage2LocalSrv(msg);
}
//------------------------------- PUBLIC SLOTS ---------------------------------
void Engine::convertTemplatesNameToFilePath(QString t_name)
{
    QString template_path;
    // Заменим префикс на .tmpl, в зависимости от префикса добавим каталог

    QString name   = t_name.section(".",0,0);
    QString prefix = t_name.section(".",1,1);
    if (prefix == QObject::trUtf8("Личный")){
        template_path = QObject::trUtf8("%1/%2.tmpl")
                        .arg(local_t_path).arg(name);
    }else {
        template_path = QObject::trUtf8("%1/%2.tmpl")
                        .arg(local_t_path).arg(name);
    }
    // Найдем в списке файлов подходящий шаблон
    for (int i=0; i< templatesFileInfoList.size();i++){
        QFileInfo fileInfo = templatesFileInfoList.at(i);
        if (fileInfo.absoluteFilePath().toUpper() == template_path.toUpper()){
            emit setTemplatesFileName(template_path);
        }
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

void Engine::setError(const QString &info)
{
    e_info  = info;
    //e_state = true;
    emit error(e_info);
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
            qDebug() << "gatekeeper_bin: " << gatekeeper_bin;
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
    int pCnt;
    switch (r_msg.type()){
    case VPrn::Ans_TemplateNotFound:
        str.append( r_msg.messageData() );
        emit MergeDocWithTemplates( false, str);
        break;
    case Ans_SourceDocNotFound: //Исходный документ не найден или не верного формата, в теле сообщения подробности
        str.append( r_msg.messageData() );
        emit MergeDocWithTemplates( false, str);
        break;
    case Ans_CreateFormatedDoc:
        emit MergeDocWithTemplates( true, QObject::trUtf8("Применение шаблона к текущему документу, успешно завершено!"));
        emit PreviewPages( r_msg.messageData() );
        break;
    case VPrn::GoodBay:
        str.append(r_msg.messageData());
        emit reciveGoodBayMsg(str);
        break;
    case VPrn::Ans_MB_NOT_EXIST:
        //Документ не зарегистрирован в БД учета необходима его регистрация, и можно
        // формировать стр. предпросмотра
        emit needRegisterDocInBase();
        break;
    case VPrn::Ans_MB_EXIST_AND_BRAK:
        break;
    case VPrn::Ans_MB_EXIST_AND_NOT_BRAK:
        break;
    case VPrn::Ans_MB_LIST:
        break;
    case VPrn::Ans_RegisterDocInBase:
        //Документ зарегистрирован в БД УЧЕТА
        str.append(r_msg.messageData());
        emit RegisterDocInBase( true, str);
        break;
    case VPrn::Ans_PRINT_ALLOWED:
        str.append(r_msg.messageData());
        emit authToDevice( true,str );
        break;
    case VPrn::Ans_PRINT_DENIED:
        str.append(r_msg.messageData());
        emit authToDevice( false,str );
        break;
    case VPrn::Ans_PRINTER_NOT_FOUND:
        str.append(r_msg.messageData());
        emit authToDevice( false,str );
        break;
    case VPrn::Ans_Convert2PdfFinish:
        emit doc_converted();
        break;
    case VPrn::Ans_PageCounting:
        str.append(r_msg.messageData()); // Число страниц в str
        pCnt = str.toInt();
        emit getPagesCount(pCnt);
        break;
    case VPrn::Ans_PageSplittedFirst:
        emit firstPageSplitted();
        break;
    case VPrn::Ans_PageSplittedOther:
        emit otherPageSplitted();
        break;
    case VPrn::Ans_PRINTER_LIST:
        str.append(r_msg.messageData());
        convertDeviceURIListToPrinterList(str);
        emit RecivePrintersList();
        break;
    case VPrn::Ans_STAMP_LIST: // Получили названия уровней секретности, сохраним
        str.append(r_msg.messageData());
        secLevelList << str.split(";:;");
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
            currentUserName   = rx.cap(1);
            currentUserMandat = rx.cap(2);
            //emit gk_fullReady(currentUserName,currentUserMandat);
            //Запрос уровней секретности для мандата
            setUserMandat(currentUserName,currentUserMandat );
            emit RemoteDemonRegistr();
        }else{
            emit error(QObject::trUtf8("Ошибка при получении имени и мандата текщего пользователя"));
        }
        break;

    }
}

void Engine::setFileList(const QString &t_path,const QString &prefix)
{
    QStringList filters;
    QDir dir;

    filters << "*.tmpl" << "*.TMPL";
    // Читаем шаблоны
    dir.setPath(t_path);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setNameFilters(filters);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        /// @todo В дальнейшем надо получать название шаблона из самого шаблона
        // через плагин
        templatesList.append(tr("%1%2").arg(fileInfo.completeBaseName(),prefix)
                             );
        templatesFileInfoList.append(fileInfo);
    }

}

QString Engine::findPrinterInDeviceURIList(const QString &prn)
{
    QString t_line;
    QMapIterator<QString, QString> i (printer_device_list);
    while (i.hasNext()) {
        i.next();
        t_line = i.key().section(".",1,1);
        if (t_line == prn){
            return i.key();
        }
    }
    return QString();
}

void Engine::convertDeviceURIListToPrinterList(const QString & device_uri_list)
{
    QStringList remote_printer;
    QStringList devices_info;
    QString pline;
    QStringList printersList;
    remote_printer = device_uri_list.split("###;:;");
    for (int i = 0; i < remote_printer.size(); i++) {
        pline = remote_printer.at(i);
        pline.replace("###","");
        devices_info = pline.split(";:;");
        printer_device_list.insert(devices_info.at(0),devices_info.at(1));
        pline = devices_info.at(0);
        printersList.append(pline.section(".",1,1));// Имя принтера после точки
    }
    emit getPrinterList(printersList);
}
