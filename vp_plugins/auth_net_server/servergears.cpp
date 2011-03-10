#include "servergears.h"
#include "templatesinfo.h"

#include <QtCore/QDebug>

#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QByteArray>
#include <QtCore/QDataStream>
#include <QtCore/QUuid>
#include <QtCore/QRegExp>
#include <QtCore/QMapIterator>
#include <QtCore/QFileInfoList>
#include <QTimer>

#include <QtGui/QMessageBox>

using namespace VPrn;

serverGears::serverGears(QObject *parent,const QString &srvName)
        : QLocalServer(parent)
        , packetSize(-1)
        , e_info(QString())
        , net_plugin(0)
        , gs_plugin(0)
        , tmpl_plugin(0)
        , u_login(QString())
        , u_mandat(QString())
        , netDemonReady(false)

{
    m_serverName = srvName;   
    /// Создаем локальный сервер
    m_server = new QLocalServer(this);
    if (!m_server->listen(m_serverName)) {
        emit error(VPrn::SocketError,
                   QObject::trUtf8("Не могу запустить локальный сервер: %1.\n%2")
                   .arg(m_server->errorString())
                   .arg(QString(Q_FUNC_INFO))
                   );
        setCheckPoint(VPrn::loc_CantStartListen);
    }else{
        // До загрузки всех плагинов не принимаем сообщения от клиентов
        m_server->setMaxPendingConnections(0);
        connect(m_server, SIGNAL(newConnection()),
                this,     SLOT(client_init()));
        setCheckPoint(VPrn::loc_ServerStart);
    }
}


MyCheckPoints serverGears::checkPoints() const
{
    return m_checkPoint;
}

void serverGears::setAuthData(const QString &userName,const QString &userMandat)
{
    if ( !userName.isEmpty()){
        u_login = userName;
    }
    if ( !userMandat.isEmpty()){
        u_mandat = userMandat;        
    }
    this->init();
}

void serverGears::setNetPlugin(Inet_plugin *NetPlugin)
{
    if (NetPlugin){
        net_plugin = NetPlugin;
        this->init();
    }else{
        emit error(VPrn::PluginsNotLoad,
                   QObject::trUtf8("Ошибка при попытке использования сетевого плагина.\n %1.")
                   .arg(QString(Q_FUNC_INFO))
                   );
    }    
}

void serverGears::setGsPlugin(Igs_plugin *GsPlugin)
{
    if (GsPlugin){
        gs_plugin = GsPlugin;
        this->init();
    }else{
        emit error(VPrn::PluginsNotLoad,
                   QObject::trUtf8("Ошибка при попытке использования PostcSript плагина.\n %1.")
                   .arg(QString(Q_FUNC_INFO))
                   );
    }
}

void serverGears::setTmplPlugin(Itmpl_sql_plugin *TmplPlugin)
{
    if (TmplPlugin){
        tmpl_plugin = TmplPlugin;
        this->init();
    }else{        
        emit error(VPrn::PluginsNotLoad,
                   QObject::trUtf8("Ошибка при попытке использования плагина работы с шаблонами.\n %1.")
                   .arg(QString(Q_FUNC_INFO))
                   );
    }
}

void serverGears::sayGoodBayAllClients()
{
    Message loc_msg(this);
    QString str = QObject::trUtf8("Так как GateKeeper получил команду на завершение работы, приложение будет закрыто!");
    loc_msg.setType(VPrn::GoodBay);
    loc_msg.setMessageData(  str.toUtf8() ); // Пробразуем в QByteArray

    foreach(QLocalSocket *client, clients){
        // Скажем все пора завершать работу! Большой папа уходит в мир иной
        sendMessage( loc_msg,client);
    }
}

void serverGears::findTemplatesInPath(const QString &t_path)
{
    if (t_path.isEmpty()){
        return;
    }    

    QStringList filters;
    QDir dir;

    filters << "*.tmpl" << "*.TMPL";
    // Читаем шаблоны
    dir.setPath(t_path);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setNameFilters(filters);
    QFileInfoList list = dir.entryInfoList();

    for (int i = 0; i < list.size(); ++i) {
        templatesFileInfoList.append( list.at(i).absoluteFilePath() );
    }
}

//------------------------- PRIVATE SLOTS --------------------------------------
void serverGears::prepareError(QLocalSocket::LocalSocketError socketError)
{
    QLocalSocket *client = (QLocalSocket*)sender();
    QString clientName = clients_name.value(client);

    switch (socketError) {
    case QLocalSocket::ConnectionRefusedError:
        emit error(VPrn::NetworkError,
                   QObject::trUtf8("Соединение было сброшенно клиентом [%1].\n %2.")
                   .arg(clientName).arg(QString(Q_FUNC_INFO))
                   );
        break;
    default:
        emit error(VPrn::NetworkError,
                   QObject::trUtf8("При работе с клиентом [%1] произошла ошибка:%2.\n %3.")
                   .arg(clientName, client->errorString())
                   .arg(QString(Q_FUNC_INFO))
                   );
    }
    setCheckPoint(VPrn::net_CommonError);
}

void serverGears::readyRead()
{
    QLocalSocket *client = (QLocalSocket*)sender();
    QString client_uuid = clients_uuid.value(client);

    //Свяжем поток и сокет
    QDataStream in ( client );
    in.setVersion(QDataStream::Qt_3_0); // Так сообщение может переслаться в сетевой сокет, то надо подумать и о Мише с его 3 QT
    qDebug() << "client->bytesAvailable() =" << client->bytesAvailable() <<"\n";

    while (client->bytesAvailable() > 0){
        if (packetSize == -1) {
            //Определим количество байт доступных для чтения;
            //на этом шаге необходимо получить больше 4-х байт
            if( (qint32) client->bytesAvailable() < (qint32) sizeof(packetSize) ){
                return;
            }
            //Читаем размер пакета
            in >> packetSize;
            qDebug() << Q_FUNC_INFO << " packet size "  << packetSize << "\n";

        }
        //Проверим что в сокет пришел весь пакет а не его огрызки
        if (client->bytesAvailable() < packetSize){
            return;
        }
        if (in.atEnd()){
            return;
        }
        qDebug() << Q_FUNC_INFO << "recive full packet size "
                << packetSize << "\n";

        //Сбросим размер пакета, для обработки следующего
        packetSize = -1;
        // Прочтем тип сообщения
        int m_Type;
        in >> m_Type;

        //Прочтем само сообщение
        QByteArray msg;
        in >> msg;
        Message message( this );
        message.setType((MessageType) m_Type);
        message.setMessageData( msg );
        //message.prepareMessage();

        // Обработка сообщения
        parseMessage(message,client_uuid);
        //foreach(QLocalSocket *client, clients)
    }
}

void serverGears::reciveNetworkMessage(const Message &r_msg)
{
    Message loc_msg(this);
    QString str;
    QLocalSocket *client(0);
    QString m_uuid;
    QString m_body;

    // Проверим это сообщение о регистрации ?
    if (r_msg.getType() == VPrn::Ans_RegisterGlobal){
        //Успешная регистрация у мишиного демона
        setCheckPoint(VPrn::net_Connected);
        netDemonReady = true;
    }else{
        str.append(r_msg.getMessageData());
        // Разберем тело ответа на части [кому];:;что_передали
        QRegExp rx("\\[(.+)\\];:;(.*)");
        //rx.setMinimal(true);

        if(rx.indexIn(str) != -1){
            m_uuid  = rx.cap(1);
            m_body  = rx.cap(2);
            // По UUID определим какому клиенту надо было это сообщение
            client = findClient(m_uuid);
            loc_msg.clear();

            if (client){
                switch (r_msg.getType()){

                case VPrn::Ans_RegisterDocInBase:
                    // Получили ответ о регистрации документа , отдадим его клиенту
                    loc_msg.setType( VPrn::Ans_RegisterDocInBase );
                    loc_msg.setMessageData( m_body.toUtf8() );
                    break;
                case VPrn::Ans_MB_NOT_EXIST:
                    str = QObject::trUtf8("Документ в базе учета не зарегистрирован!");
                    loc_msg.setType(  VPrn::Ans_MB_NOT_EXIST );
                    loc_msg.setMessageData(  str.toUtf8() );
                    break;
                case VPrn::Ans_MB_EXIST_AND_BRAK:
                    // Для этих случаев требуется дополнительная проверка,
                    // т.е основное приложение должно передать список всех
                    // полей документа, для глубокого поиска
                    loc_msg.setType(VPrn::Ans_MB_EXIST_AND_BRAK);
                    break;
                case VPrn::Ans_MB_EXIST_AND_NOT_BRAK:
                    // Тут все просто приложение должно показать страничку
                    // где данный документ будет помечен как брак, но после
                    // дополнительного глубокого поиска
                    loc_msg.setType( VPrn::Ans_MB_EXIST_AND_NOT_BRAK );
                    break;

                case VPrn::Ans_MB_LIST:
                    // Получили затребованный список документов, отдами его клиенту
                    loc_msg.setType( VPrn::Ans_MB_LIST );
                    loc_msg.setMessageData( r_msg.getMessageData() );
                    break;
                case VPrn::Ans_PRINT_ALLOWED:
                    str = QObject::trUtf8("Вам разрешена печать на выбранный принтер!");
                    loc_msg.setType(VPrn::Ans_PRINT_ALLOWED);
                    loc_msg.setMessageData( str.toUtf8() );
                    break;
                case VPrn::Ans_PRINT_DENIED:
                    str = QObject::trUtf8("Вам запрещена печать на выбранный принтер!");
                    loc_msg.setType(VPrn::Ans_PRINT_DENIED);
                    loc_msg.setMessageData(  str.toUtf8() );
                    break;
                case VPrn::Ans_PRINTER_NOT_FOUND:
                    str = QObject::trUtf8("Выбранный Вами принтер, в настоящее время отключен от системы или удален!");
                    loc_msg.setType(VPrn::Ans_PRINTER_NOT_FOUND);
                    loc_msg.setMessageData(  str.toUtf8() );
                    break;
                case VPrn::Ans_PRINTER_LIST:
                    loc_msg.setType(VPrn::Ans_PRINTER_LIST);
                    str.append( m_body );
#ifdef DEBUG_MODE_OFF
                    /**
                      * @todo Сделал только для показ работы на локальном компе
                      * Список принтеров взят из ini файла и записан в виде
                      * Выводимое имя;:;ip сервера;:;имя очереди печати(принтера)
                    */
                    str.clear();
                    for (int i=0; i< prn_list.size();i++){
                        str.append(prn_list.at(i).name).append(";:;")
                                .append(prn_list.at(i).ip).append(";:;")
                                .append(prn_list.at(i).p_qqueue);
                    }

                    qDebug() << Q_FUNC_INFO << str << "\n";
#endif
                    loc_msg.setMessageData( str.toUtf8() );
                    break;
                case VPrn::Ans_PRINTER_LIST_EMPTY:
                    loc_msg.setType(VPrn::Ans_SrvStatusNotReady);
                    str = QObject::trUtf8("Данному пользователю не назначен ни один принтер!");
                    loc_msg.setMessageData(  str.toUtf8() );
                    break;
                case VPrn::Ans_MANDAT_LIST:
                    loc_msg.clear();
                    loc_msg.setType(VPrn::Ans_SrvStatusPartReady);
                    //str = QObject::trUtf8("[%1];:;%2").arg(this->u_login,m_body);
                    str = QObject::trUtf8("[%1];:;%2").arg(this->u_login,"CC;:;NS;:;DSP");
                    loc_msg.setMessageData(  str.toUtf8() );
                    break;
                case VPrn::Ans_MANDAT_LIST_EMPTY:
                    loc_msg.setType(VPrn::Ans_SrvStatusNotReady);
                    str = QObject::trUtf8("Данному пользователю не назначен ни один мандат!");
                    loc_msg.setMessageData(  str.toUtf8() );
                    break;
                case VPrn::Ans_STAMP_LIST:
                    //m_body содержит список уровней секретности
                    loc_msg.setType(VPrn::Ans_STAMP_LIST);
                    loc_msg.setMessageData(  m_body.toUtf8() );
                    break;
                case VPrn::Ans_CheckFileSizeSuccess:
                    sendFileToDemon(m_uuid); // Место есть можно отправлять демону файл
                    //debugDirectPrint(m_uuid);
                    break;
                case VPrn::Ans_CheckFileSizeFailure:
                    str = QObject::trUtf8("Недостаточно места на сервере печати!");
                    loc_msg.setType(VPrn::Err_Message);
                    loc_msg.setMessageData(  str.toUtf8() );
                    break;
                case VPrn::Ans_PrintThisFileSuccess:
                    checkCurrentFile(m_uuid); // Проверим следующий файл
                    break;
                case VPrn::Ans_PrintThisFileFailure:
                    str = QObject::trUtf8("Ошибка при печати документа %1!")
                          .arg(m_body);
                    loc_msg.setType(VPrn::Err_Message);
                    loc_msg.setMessageData(  str.toUtf8() );
                    break;
                default:
                    /* do nothing */
                    break;
                }
                if (loc_msg.getType() != VPrn::NoMsgType ){
                    // Запись в локальный слот клиенту
                    sendMessage(loc_msg,client);
                }
            }
        }else{            
            emit error(VPrn::InternalAppError,
                       QObject::trUtf8("Ошибочный ответ от демона СУРД!.\n %1.")
                       .arg(QString(Q_FUNC_INFO))
                       );
        }
    }
}

void serverGears::client_init()
{
    QLocalSocket *client = m_server->nextPendingConnection();

    if (gs_plugin){
        clients.insert(client);

        QString c_uuid =  gs_plugin->getUuid();
        clients_uuid.insert(client,c_uuid);
        qDebug() << Q_FUNC_INFO << c_uuid;

        gs_plugin->createClientData(c_uuid);
        connect(client, SIGNAL(readyRead()),
                this,   SLOT(readyRead()));
        connect(client, SIGNAL(disconnected()),
                this,   SLOT(disconnected()));
        connect(client, SIGNAL(error(QLocalSocket::LocalSocketError)),
                this,   SLOT(prepareError(QLocalSocket::LocalSocketError)));
        setCheckPoint(VPrn::loc_NewClientStarted);
        /** @todo Подумать как объединить дублированние информации
          * ClientData из gs_plugin и  PrintTask
          */
        // Создаем учетные данные для печати документа
        PrintTask *pTask = findpTack(c_uuid);
        if (!pTask){
            emit error(VPrn::InternalAppError,
                       QObject::trUtf8("Ошибка приложения, задание на печать не существует\n%1")
                       .arg(QString(Q_FUNC_INFO))
                       );
            return;
        }
    }
}

void serverGears::disconnected()
{
    QLocalSocket *client = (QLocalSocket*)sender();
    QString clientName = clients_name.value(client);
    qDebug() << "Client disconnected:" << clientName;
    QString c_uuid =  clients_uuid.value(client);


    if (gs_plugin){
        gs_plugin->deleteClientData(c_uuid);
    }
    clients.remove(client);
    clients_uuid.remove(client);
    clients_name.remove(client);

    setCheckPoint(VPrn::loc_Disconnected);
    //Потребуем  от сервера удалить все файлы, которые создавали в интересах клиента
    /// @todo FIXME
    emit clearClientSpool( c_uuid );
}
//-------------------------- PRIVATE -------------------------------------------

void serverGears::setCheckPoint(MyCheckPoints cp)
{
    m_checkPoint = cp;
    emit checkPointChanged(m_checkPoint);
}

void serverGears::parseMessage( const Message &m_msg, const QString &c_uuid)
{

    QLocalSocket *client(0);    
    Message message( this );
    QString str;

    setCheckPoint(VPrn::loc_MessageRecive);
    // По UUID определим какому клиенту надо было это сообщение
    client = findClient(c_uuid);
    if (client){
        switch (m_msg.getType()){
        case VPrn::Ans_UserNeedMarkCopies:
            str.append(m_msg.getMessageData()); /// В теле сообщения параметры
            // Просто перешлем в сеть
            message.setType   ( VPrn::SaveToBaseCopiesMarker );
            message.setMessageData(
                    QString("[%1];:;%2").arg( c_uuid, str ).toUtf8()
                    );
            //Запись в сетевой канал
            if (net_plugin){
                net_plugin->sendMessage(message);
            }
            break;
        case VPrn::Que_UserDemands2Restart:
            str.append(m_msg.getMessageData()); /// В теле сообщения параметры
            //markDocInBaseAsFault(c_uuid,str);// Пометка документа в базе как бракованный

            //Сформируем набор сообщений и отправим клиенту
            if (gs_plugin ){
                message.setType ( VPrn::Ans_Convert2PdfFinish);
                sendMessage( message,client );
                gs_plugin->createClientData(c_uuid);
                gs_plugin->calcPageCount(c_uuid);
            }
            break;
        case VPrn::Que_GiveMeTemplatesList:
            {
                // Вернем модель МЕТАДанные о шаблонах преобразованную для передачи в сокет
                TemplatesInfo *tInfo = new TemplatesInfo(this);
                tInfo->setHorizontalHeaderLabels();

                if (tInfo->getMetaInfo(c_uuid,templatesFileInfoList)){
                    message.setType ( VPrn::Ans_GiveMeTemplatesList );
                    message.setMessageData( tInfo->toByteArray() );
                    sendMessage( message,client );
                }
            }
            break;
        case VPrn::Que_RegisterDocInBase:
            str.append(m_msg.getMessageData()); /// В теле сообщения query_sql;
            // Просто перешлем в сеть
            message.setType   ( VPrn::Que_RegisterDocInBase );
            message.setMessageData(
                    QString("[%1];:;%2;:;%3").arg( c_uuid, str, u_login )
                    .toUtf8() );
            //Запись в сетевой канал
            if (net_plugin){
                net_plugin->sendMessage(message);
            }
            break;
        case VPrn::Que_GET_MB_LISTS:
            str.append(m_msg.getMessageData()); /// В теле сообщения query_sql;
            // Просто перешлем в сеть
            message.setType   ( VPrn::Que_GET_MB_LISTS );
            message.setMessageData(
                    QString("[%1];:;%2;:;%3;:;").arg( c_uuid, str, u_login )
                    .toUtf8() );
            //Запись в сетевой канал
            if (net_plugin){
                net_plugin->sendMessage(message);
            }
            break;
        case VPrn::Que_IS_MB_EXIST:
            str.append(m_msg.getMessageData()); /// В теле сообщения query_sql;
            str.append("select * from record");
            // Просто перешлем в сеть
            message.setType   ( VPrn::Que_IS_MB_EXIST );
            message.setMessageData(
                    QString("[%1];:;%2;:;%3;:;").arg( c_uuid, str, u_login )
                    .toUtf8() );
            //Запись в сетевой канал
            if (net_plugin){
                net_plugin->sendMessage(message);
            }
            break;

        case VPrn::Que_AUTHOR_USER:{                
                // Запрос авторизации на устройство
                // Сохраним в очереди печати текущий выбранный принтер
                PrintTask *pTask = findpTack(c_uuid);
                if (pTask){
                    QString str = m_msg.getMessageData();
                    /// В теле сообщения device_uri;
                    pTask->setPrinterQueue(str);
                    // Просто перешлем в сеть

                    //str.append(str.replace(";:;",".")); /// В теле сообщения device_uri;

                    message.setType(VPrn::Que_AUTHOR_USER);
                    message.setMessageData(
                            QString("[%1];:;%2;:;%3;:;%4;:;").arg( c_uuid, str, u_mandat,u_login )
                            .toUtf8() );
                    if (net_plugin){
                        net_plugin->sendMessage(message);
                    }
                }
            }
            break;
        case VPrn::Que_Convert2Pdf:
            /// Клиент потребовал преобразовать ps файл в pdf
            str.append(m_msg.getMessageData()); /// В теле сообщения полный путь к файлу
            if (gs_plugin){
                gs_plugin->convertPs2Pdf( c_uuid,str );
            }
            break;
        case VPrn::Que_CreateFormatedFullDoc:
            // Запрос формирование полного документа, как для печати,
            // нужно вернуть список png страничек сделанного документа
            createFormatedDoc(c_uuid,VPrn::pre_FullMode,m_msg.getMessageData());
            break;
        case VPrn::Que_CreateFormatedPartDoc:
            // Запрос формирование частичного документа, как для печати,
            // нужно вернуть список png страничек сделанного документа
            createFormatedDoc(c_uuid,VPrn::pre_PartMode,m_msg.getMessageData());
            break;
        case VPrn::Que_CreateFormatedFullDocAndPrint:
            // Запрос формирование полного документа, для печати,
            //  и распечатка его
            createFormatedDoc(c_uuid,VPrn::pre_ClearPrintMode,m_msg.getMessageData());
            break;
        case VPrn::Que_PrintCurrentFormatedDoc:
            // запуск печати
            checkCurrentFile(c_uuid);
            break;
        case VPrn::Que_Register:
            message.setType(VPrn::Ans_Register);
            message.setMessageData(  c_uuid.toUtf8() ); // Пробразуем в QByteArray
            sendMessage( message,client) ;            
            break;
        case VPrn::Que_ServerStatus:
            // Клиент запросил состояние сервера
            message.clear();
            str.clear();
            if (!netDemonReady){
                str = QObject::trUtf8("Нет ответа от шлюза в СУРД или отсутсвует сетевое соединение");
                message.setType(VPrn::Ans_SrvStatusNotReady);
                message.setMessageData(  str.toUtf8() );
                // Запись в локальный слот клиенту
                sendMessage(message,client);
            }else{
                if (!this->u_login.isEmpty()){
                    if (this->u_mandat.isEmpty()){
                        //запрос списка мандатов к которым допущен пользователь
                        /// @todo  Показать Мише как разбирать!!!!!!
                        message.setType(VPrn::Que_MANDAT_LIST);
                        str = QObject::trUtf8("[%1];:;%2;:;").arg( c_uuid,u_login );
                        message.setMessageData( str.toUtf8() );
                        //Запись в сетевой канал
                        if (net_plugin){
                            net_plugin->sendMessage(message);
                        }

                    }else{
                        str = QObject::trUtf8("%1;:;%2;:;").arg(u_login,u_mandat);
                        message.setType(VPrn::Ans_SrvStatusFullReady);
                        message.setMessageData(  str.toUtf8() );
                        // Запись в локальный слот клиенту
                        sendMessage(message,client);
                    }
                }else{
                    str = QObject::trUtf8("Ошибка аутинтификации пользователя.Логин не определен или пустой!");
                    message.setType(VPrn::Ans_SrvStatusNotReady);
                    message.setMessageData( str.toUtf8() );
                    // Запись в локальный слот клиенту
                    sendMessage(message,client);
                }
            }
            break;
        case VPrn::Que_SEC_LEVEL:
            // Переправим в сеть данный запрос, оформив его как следует
            /// @todo Подумать как оптимизировать выдачу данных в сеть
            message.clear();
            str.clear();
            if (!netDemonReady){
                str = QObject::trUtf8("Нет ответа от шлюза в СУРД или отсутсвует сетевое соединение");
                message.setType(VPrn::Ans_SrvStatusNotReady);
                message.setMessageData(  str.toUtf8() );
                // Запись в локальный слот клиенту
                sendMessage(message,client);
            }else{
                message.setType(VPrn::Que_SEC_LEVEL);
                if (u_mandat.isEmpty()){
                    u_mandat.append(m_msg.getMessageData());
                }
                str = QObject::trUtf8("[%1];:;%2;:;").arg(c_uuid,u_mandat);
                message.setMessageData( str.toUtf8() );
                //Запись в сетевой канал
                if (net_plugin){
                    net_plugin->sendMessage(message);
                }

            }
            break;
        case VPrn::Que_GET_PRINTER_LIST:
            if (!netDemonReady){
                str = QObject::trUtf8("Нет ответа от шлюза в СУРД или отсутсвует сетевое соединение");
                message.setType(VPrn::Ans_SrvStatusNotReady);
                message.setMessageData(  str.toUtf8() );
                // Запись в локальный слот клиенту
                sendMessage(message,client);
            }else{
                message.setType(VPrn::Que_GET_PRINTER_LIST);
                str = QObject::trUtf8("[%1];:;%2;:;%3;:;").arg(c_uuid,u_login,u_mandat);
                message.setMessageData( str.toUtf8() );
                //Запись в сетевой канал
                if (net_plugin){
                    net_plugin->sendMessage(message);
                }
            }
            break;
        default:
            /* do nothing */
            break;
        }
    }
}

void serverGears::sendMessage( const Message &m_msg, QLocalSocket *client)
{
    QString clientName = clients_uuid.value(client);

    qDebug()<< "\nSend Message:"
            << "\nType " << m_msg.getType()
            << "\nData " << m_msg.getMessageData()
            << "\nto Client\n-----------------------------\n"  << clientName;

    //Сформируем пакет И пошлем его ветром гонимого клиенту
    client->write(m_msg.createPacket());
    client->flush();
}

QLocalSocket *serverGears::findClient(const QString &c_uuid)
{
    QLocalSocket *client(0);
    QMapIterator<QLocalSocket *,QString>  i(clients_uuid);
    while (i.hasNext()) {
        i.next();
        //qDebug() << "\nClient Key: " << i.key() << " value: " << i.value();
        if (i.value() == c_uuid ){
            return i.key(); //Я работаю только с живыми клиентами
        }
    }
    // Только некромантам интересны мертвые клиенты
    emit error(VPrn::InternalAppError,
               QObject::trUtf8("Ответ клиенту который уже отсоединился!\n %1.")
               .arg(QString(Q_FUNC_INFO))
               );
    return client;
}

void serverGears::do_docConvertedToPdf(const QString &client_uuid)
{
    QLocalSocket *client(0);
    Message msg( this );

    // По UUID определим какому клиенту надо было это сообщение
    client = findClient(client_uuid);
    if (client){
        msg.setType(VPrn::Ans_Convert2PdfFinish );

        // Запись в локальный слот клиенту
        sendMessage(msg,client);
    }

}

void serverGears::do_docReady4work (const QString &client_uuid,int pCount)
{
    QLocalSocket *client(0);
    Message msg( this );
    QString str;
    // По UUID определим какому клиенту надо было это сообщение
    client = findClient(client_uuid);
    if (client){
        str = QString("%1").arg(pCount,0,10);
        msg.setType(VPrn::Ans_PageCounting);
        msg.setMessageData( str.toUtf8() );  // число страниц в документе
        // Запись в локальный слот клиенту
        sendMessage(msg,client);
    }
}

void serverGears::do_docReady4print (const QString &client_uuid)
{
    /**
    * @remarks Формируем очередь заданий на печать:
    * @li 111         - Начало экземпляра документа
    * @li fileToPrint - Лицевая сторона
    * @li 222         - Требование перевернуть листы
    * @li fileToPrint - Обратная сторона документа
    * @li fileToPrint - Фонарик
    * @li 333         - Конец экземпляра
    */
    QLocalSocket *client(0);    

    //Обработка неприятностей
    if (!gs_plugin){
        emit error(VPrn::InternalAppError,
                   QObject::trUtf8("Ошибка приложения, плагин работы с GhostScript не загружен\n%1")
                   .arg(QString(Q_FUNC_INFO))
                   );
        return;
    }
    // По UUID определим какому клиенту надо было это сообщение    
    client = findClient(client_uuid);
    if (!client){
        emit error(VPrn::InternalAppError,
                   QObject::trUtf8("Ошибка приложения, клиента защищенного принтера отключен\n%1")
                   .arg(QString(Q_FUNC_INFO))
                   );
        return;
    }
    // Определим его очередь печати
    PrintTask *pTask = findpTack(client_uuid);
    if (!pTask){
        emit error(VPrn::InternalAppError,
                   QObject::trUtf8("Ошибка приложения, задание на печать не существует\n%1")
                   .arg(QString(Q_FUNC_INFO))
                   );
        return;
    }
    //------------------- Настоящая работа ------------------------------------
    // Отправим сообщение локальному клиенту
    Message msg( this );
    msg.setType(VPrn::Ans_CreateFormatedDoc); // На документ успешно наложен шаблон
    sendMessage(msg,client);                  // Запись в локальный слот клиенту
    //очистим очередь печати для клиента
    pTask->clearQueue();    
    QList <int> doc_copies = pTask->getDocCopies();
    for (int i=1; i<= doc_copies.size();i++){
        QStringList files = gs_plugin->findFiles4Copy(client_uuid,i,
                                                      QStringList() << "*out.pdf");
        if (files.count() !=-1){
            QMap <int,QString> orderList;
            // Формируем задание для данного экземпляра
            orderList.insert(0,QString("111"));

            for (int j=0;j<files.size();j++){
                QString filename = files.at( j );
                QRegExp rx("/(.+)/(.+)/(.)-copy/(.+_out).pdf");
                if(rx.indexIn( filename ) != -1){
                    QString copy_num   = rx.cap(3);
                    QString page_type  = rx.cap(4);
                    if ( page_type.compare("face_pages_out",Qt::CaseInsensitive) == 0){
                        orderList.insert(1,filename);
                    }

                    if ( page_type.compare("oversidepage_out",Qt::CaseInsensitive) == 0){
                        orderList.insert(2,QString("222"));
                        orderList.insert(3,filename);
                    }
                    if ( page_type.compare("lastpage_out",Qt::CaseInsensitive) == 0){
                        orderList.insert(4,QString("333"));
                        orderList.insert(5,filename);
                    }
                    orderList.insert(6,QString("444"));
                    orderList.insert(7,QObject::trUtf8("Печатаемый экземпляр №%1").arg(copy_num));
                }
            }
            // Проверка особого случая есть 1 стр, есть фонарик,
            // но нет последующих страниц документа
            if (!orderList.value(1).isEmpty() &&
                orderList.value(3).isEmpty() &&
                !orderList.value(5).isEmpty()
                ){
                orderList.insert(4,QString("333"));
            }
            qDebug() << "--------- Begin order list";
            for (int l=0; l< orderList.count();l++){
                if ( l == 3){
                    pTask->addFileToPrintQueue(orderList.value(l),
                                               pTask->getPageCount()
                                               );
                }else{
                    pTask->addFileToPrintQueue(orderList.value(l),1);
                }
            }
            qDebug() << "--------- End order list";
        }
    }
}

void serverGears::do_docReady4preview (const QString &client_uuid)
{
    QLocalSocket *client(0);
    Message msg( this );
    QStringList files;
    // По UUID определим какому клиенту надо было это сообщение
    client = findClient(client_uuid);
    if (client){

        if (gs_plugin){
            // Обработали все файлы надо пройти по всем каталогам и собрать *.png
            files =  gs_plugin->findFiles(client_uuid,QStringList() << "*.png" );
            msg.setType(VPrn::Ans_ConvertFormatedDocToPng); // Документ конвертирован в png
            msg.setMessageData(files);

            sendMessage(msg,client); // Запись в локальный слот клиенту
        }
    }
}

void serverGears::createFormatedDoc(const QString &client_uuid,
                                    VPrn::PreviewMode prn_mode,QByteArray data)
{
    qDebug() << Q_FUNC_INFO << "data.size() " << data.size() << "\n";
    QString t_fileName;
    QList <int> doc_copyes;
    QMap <int,QString> m_tagValue;

    // Получили данные переданные пользователем, разбор данных
    QDataStream in(&data, QIODevice::ReadOnly );
    in.setVersion(QDataStream::Qt_3_0);
    in >> doc_copyes;
    in >> t_fileName;
    // читаем значения
    in >> m_tagValue;    

    PrintTask *pTask = findpTack(client_uuid);

    pTask->setMB( m_tagValue.value( VPrn::cards_MB_NUMBER ) );
    pTask->setDocName( m_tagValue.value( VPrn::cards_DOC_NAME ) );
    pTask->setDocCopies( doc_copyes);
    pTask->setPageCount (m_tagValue.value(VPrn::cards_PAGE_COUNT).toInt() );

    // Формируем страницы шаблона в pdf
    bool Ok = true;
    {
        for (int i=1; i<= doc_copyes.size();i++){
            m_tagValue[VPrn::cards_CURRENT_COPY] = QString("%1")
                                                   .arg(i,0,10);
            Ok &= tmpl_plugin->prepare_template(client_uuid,
                                                t_fileName,
                                                m_tagValue,i);           
        }
    }
    if (Ok &&  gs_plugin ){
        QStringList files = gs_plugin->findFiles(client_uuid,
                                                 QStringList() << "t_*.pdf"
                                                 );
        // Сформируем документ подготовленный к печати  документ + шаблон
        gs_plugin->mergeWithTemplate(client_uuid, files,prn_mode ) ;
    }

}



qint64 serverGears::getCompresedFile(const QString &fileName,
                                     QByteArray &data)
{
    data.clear();

    QFile file_in(fileName);

    if (!file_in.open(QIODevice::ReadOnly) ){
        return 0;
    }
    QByteArray byteArray_in = file_in.readAll();
    file_in.close();
    data = qCompress(byteArray_in);
    return file_in.size();
}


PrintTask *serverGears::findpTack(const QString &c_uuid)
{
    // Формируем  задание для печати
    PrintTask *pTask(0);

    QMapIterator<PrintTask *,QString>  i(clients_printTask);
    while (i.hasNext()) {
        i.next();
        //qDebug() << "\nClient Key: " << i.key() << " value: " << i.value();
        if (i.value() == c_uuid ){
            return i.key(); //Я работаю только с живыми клиентами
        }
    }
    // Нет задания на печать для данного клиента формируем его
    pTask= new  PrintTask(this);
    clients_printTask.insert(pTask,c_uuid);
    return pTask;
}

void serverGears::init()
{
    bool state =  ( !m_serverName.isEmpty() &&
                    !u_login.isEmpty() &&
                    tmpl_plugin &&
                    gs_plugin &&
                    net_plugin && m_server );

    if (state) {
        m_server->setMaxPendingConnections(30);
    }
}

bool serverGears::isReadyToWork(const QString &c_uuid)
{
    bool Ok = true;
    {
        if (!net_plugin){
            emit error(VPrn::InternalAppError,
                       QObject::trUtf8("Ошибка приложения, плагин работы с LAN не загружен\n%1")
                       .arg(QString(Q_FUNC_INFO))
                       );
            Ok &= false;
        }

        if (!gs_plugin){
            emit error(VPrn::InternalAppError,
                       QObject::trUtf8("Ошибка приложения, плагин работы с GhostScript не загружен\n%1")
                       .arg(QString(Q_FUNC_INFO))
                       );
            Ok &= false;
        }
        if (!tmpl_plugin){
            emit error(VPrn::InternalAppError,
                       QObject::trUtf8("Ошибка приложения, плагин работы с Шаблонами не загружен\n%1")
                       .arg(QString(Q_FUNC_INFO))
                       );
            Ok &= false;
        }


        // ------------ Конец всех трагических случайностей ------------------------

    }
    return Ok;
}

void serverGears::debugDirectPrint(const QString &c_uuid)
{
    PrintTask *pTask = findpTack(c_uuid);
    if (!pTask){
        emit error(VPrn::InternalAppError,
                   QObject::trUtf8("Ошибка приложения, задание на печать не существует\n%1")
                   .arg(QString(Q_FUNC_INFO))
                   );
        return;
    }
    // Проверка есть ли файл в очереди печати
    if ( pTask->isNextFileToPrint() ){
        QString fileName = pTask->getFileToPrint();
        qDebug() << "direct print fileName " << fileName;

        // Просто файл для печати отправим на принтер, туда ему и дорога
        gs_plugin->directPrint(c_uuid,fileName,
                               pTask->getPrinterQueue(),
                               pTask->getPagesInDocCount() ) ;

    }
}

void serverGears::sendFileToDemon(const QString &c_uuid)
{
    PrintTask *pTask = findpTack(c_uuid);
    if (!pTask){
        emit error(VPrn::InternalAppError,
                   QObject::trUtf8("Ошибка приложения, задание на печать не существует\n%1")
                   .arg(QString(Q_FUNC_INFO))
                   );
        return;
    }
    // Проверка есть ли файл в очереди печати
    if ( pTask->isNextFileToPrint() ){
        // Формируем сообщение
        QString fileName = pTask->getFileToPrint();
        QByteArray msg_body;
        QByteArray file_data;
        qint32     file_size = getCompresedFile(fileName,file_data);
        //Формируем файл для печати сжатый
        Message net_msg(this);
        net_msg.setType(VPrn::Que_PrintThisFile);
        /**
                  * @short Que_PrintThisFile Печать файла на выбранный пользователем принтер
                  * @param (QString) JobID (уникально для каждого экз. документа)
                  * @param (QString) Имя принтера (очереди печати на CUPS)
                  * @param (qint32)   copy_number  число копий 1-100
                  * @param (QString) user_name    имя пользователя
                  * @param (QString) job_title    имя задания
                  * @param (qint32) array_size   размер не сжатого буфера
                  * @param (QByteArray) файл для печати в формате QByteArray (сжатый)
                  */
        QDataStream out(&msg_body, QIODevice::WriteOnly );
        out.setVersion(QDataStream::Qt_3_0);
        //Уникальный номер представляет собой МБ
        out     << c_uuid
                << pTask->getMB() /// @todo Проверить нужен ли МБ
                << pTask->getPrinterQueue()
                << pTask->getPagesInDocCount()
                << u_login
                << QString("%1 from %2").arg(pTask->getDocName(),fileName)
                << file_size
                << file_data;
        net_msg.setMessageData(msg_body);
        //Запись в сетевой канал
        net_plugin->sendMessage(net_msg);
    }
}

void serverGears::checkFreeSpaceInDemon(const QString &c_uuid,qint64 fsize)
{
    PrintTask *pTask = findpTack(c_uuid);
    if (!pTask){
        emit error(VPrn::InternalAppError,
                   QObject::trUtf8("Ошибка приложения, задание на печать не существует\n%1")
                   .arg(QString(Q_FUNC_INFO))
                   );
        return;
    }
    //QByteArray msg_body;
    //Формируем файл для печати сжатый
    Message net_msg(this);
    net_msg.setType(VPrn::Que_CheckFileSize);
    net_msg.setMessageData(
            QString("[%1];:;%2").arg( c_uuid).arg(fsize,0,10)
            .toUtf8() );
    //Запись в сетевой канал
    net_plugin->sendMessage(net_msg);
    //debugDirectPrint(c_uuid);
}

void serverGears::checkCurrentFile(const QString &c_uuid)
{
    PrintTask *pTask = findpTack(c_uuid);
    QLocalSocket *client(0);
    QString  fileName;

    if (!pTask){
        emit error(VPrn::InternalAppError,
                   QObject::trUtf8("Ошибка приложения, задание на печать не существует\n%1")
                   .arg(QString(Q_FUNC_INFO))
                   );
        return;
    }

    client = findClient(c_uuid); // По UUID определим какому клиенту надо было это сообщение

    if (!client){
        emit error(VPrn::InternalAppError,
                   QObject::trUtf8("Ошибка приложения, клиент защищенного принтера отключен\n%1")
                   .arg(QString(Q_FUNC_INFO))
                   );
        return;
    }

    if (pTask->isNextFileToPrint()){
        qint64 fsize = pTask->getCurrentFileSizes();
        // Проверим это файл имеющий размер > 0 байтов или маркер
        if (fsize ==0 ){ // Маркер, обработаем ситуацию,т.е отправим юзеру сообщение
            fileName = pTask->getFileToPrint();
            qDebug() << "fileName to print " <<  fileName;
            // Определим действие с ним опробуем преобразовать в Int
            bool Ok;
            int marker = fileName.toInt(&Ok,10);
            if (Ok){// Нашли маркер
                // уберем из очереди числа копий строчку
                pTask->getPagesInDocCount();
                Message loc_msg(this);
                QString str;
                switch (marker){
                case 111:
                    loc_msg.setType(VPrn::Que_BeginPrintCopies);
                    str = QString("%1").arg(pTask->getDocName());
                    loc_msg.setMessageData(  str.toUtf8() );
                    break;
                case 222:
                    loc_msg.setType(VPrn::Que_UserNeedFlipPages);
                    break;
                case 333:
                    loc_msg.setType(VPrn::Que_UserNeedCheckLastPage);
                    break;
                case 444:
                    loc_msg.setType(VPrn::Que_UserNeedMarkCopies);
                    str = QString("%1;:;%2;:;%3;:;%4;:;%5")
                          .arg(pTask->getDocName())
                          .arg(pTask->getPrinterQueue())
                          .arg(pTask->getMB())
                          .arg(pTask->getFileToPrint())
                          .arg(pTask->getPageCount(),0,10 );

                    loc_msg.setMessageData(  str.toUtf8() );
                    break;
                }
                sendMessage(loc_msg,client);// Запись в локальный слот клиенту
            }
        }else{
            checkFreeSpaceInDemon(c_uuid,fsize);
        }
    }
}
