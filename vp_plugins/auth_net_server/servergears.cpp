#include "servergears.h"
#include <QtCore/QDebug>
#include <QtCore/QByteArray>
#include <QtCore/QDataStream>
#include <QtCore/QUuid>
#include <QtCore/QRegExp>
#include <QtCore/QMapIterator>

#include <QtGui/QMessageBox>


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
        setError(QObject::trUtf8("Не могу запустить локальный сервер: %1.")
                 .arg(m_server->errorString()));

        setCheckPoint(VPrn::loc_CantStartListen);
    }else{
        connect(m_server, SIGNAL(newConnection()),
                this,     SLOT(client_init()));
        setCheckPoint(VPrn::loc_ServerStart);
    }
}


MyCheckPoints serverGears::checkPoints() const
{
    return m_checkPoint;
}

QString serverGears::getUuid() const
{
    return QUuid::createUuid().toString().replace("{","").replace("}","");
}

void serverGears::setAuthData(const QString &userName,const QString &userMandat)
{
    u_login = userName;
    u_mandat = userMandat;
}

void serverGears::setNetPlugin(Inet_plugin *NetPlugin)
{
    if (NetPlugin){
        net_plugin = NetPlugin;
    }else{
        setError(QObject::trUtf8("Ошибка при попытке использования сетевого плагина."));
    }
}

void serverGears::setGsPlugin(Igs_plugin *GsPlugin)
{
    if (GsPlugin){
        gs_plugin = GsPlugin;
    }else{
        setError(QObject::trUtf8("Ошибка при попытке использования сетевого плагина."));
    }
}

void serverGears::setTmplPlugin(Itmpl_sql_plugin *TmplPlugin)
{
    if (TmplPlugin){
        tmpl_plugin = TmplPlugin;
    }else{
        setError(QObject::trUtf8("Ошибка при попытке использования плагина работы с шаблонами."));
    }
}

void serverGears::sayGoodBayAllClients()
{
    Message loc_msg(this);
    QString str;
    str = QObject::trUtf8("Так как GateKeeper получил команду на завершение работы, приложение будет закрыто!");
    loc_msg.setType(VPrn::GoodBay);
    loc_msg.setMessage(  str.toUtf8() ); // Пробразуем в QByteArray

    foreach(QLocalSocket *client, clients){
        // Скажем все пора завершать работу! Большой папа уходит в мир иной
        sendMessage( loc_msg,client);
    }
}

//------------------------- PRIVATE SLOTS --------------------------------------
void serverGears::prepareError(QLocalSocket::LocalSocketError socketError)
{
    QLocalSocket *client = (QLocalSocket*)sender();
    QString clientName = clients_name.value(client);

    switch (socketError) {
    case QLocalSocket::ConnectionRefusedError:
        setError(QObject::trUtf8("Соединение было сброшенно клиентом [%1]").arg(clientName));
        break;
    case QLocalSocket::PeerClosedError:
        break;
    default:
        setError(QObject::trUtf8("При работе с клиентом [%1] произошла ошибка:%2")
                 .arg( clientName, client->errorString() )
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

    while (client->bytesAvailable() > 0){
        if (packetSize == -1) {
            //Определим количество байт доступных для чтения;
            //на этом шаге необходимо получить больше 4-х байт
            if( client->bytesAvailable() < (int)sizeof(qint32) ){
                return;
            }
            //Читаем размер пакета
            //client->read(reinterpret_cast<char*>(&packetSize), sizeof(qint32));
            in >> packetSize;

        }
        //Проверим что в сокет пришел весь пакет а не его огрызки
        if (client->bytesAvailable() < packetSize){
            return;
        }
        if (in.atEnd()){
            return;
        }
        //Сбросим размер пакета, для обработки следующего
        packetSize = -1;
        // Прочтем тип сообщения
        int m_Type;
        in >> m_Type;

        //Прочтем само сообщение
        QByteArray msg;
        in >> msg;
        Message message( this );
        message.setType((MessageType) m_Type); //Проверить как конвертирует
        message.setMessage( msg );
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
    if (r_msg.type() == VPrn::Ans_RegisterGlobal){
        //Успешная регистрация у мишиного демона
        setCheckPoint(VPrn::net_Connected);
        netDemonReady = true;
    }else{
        str.append(r_msg.messageData());
        // Разберем тело ответа на части [кому];:;что_передали
        QRegExp rx("\\[(.+)\\];:;(.+)");
        //rx.setMinimal(true);

        if(rx.indexIn(str) != -1){
            m_uuid  = rx.cap(1);
            m_body  = rx.cap(2);
            // По UUID определим какому клиенту надо было это сообщение
            client = findClient(m_uuid);
            loc_msg.clear();

            if (client){
                switch (r_msg.type()){
                case VPrn::Ans_RegisterDocInBase:
                    // Получили ответ о регистрации документа , отдадим его клиенту
                    loc_msg.setType( VPrn::Ans_RegisterDocInBase );
                    loc_msg.setMessage( m_body.toUtf8() );
                    break;
                case VPrn::Ans_MB_NOT_EXIST:
                    str = QObject::trUtf8("Документ в базе учета не зарегистрирован!");
                    loc_msg.setType(  VPrn::Ans_MB_NOT_EXIST );
                    loc_msg.setMessage(  str.toUtf8() );
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
                case VPrn::Ans_CHECK_DOC_ATR_EQU:
                    // Результаты глубокого поиска поля док. в БД и введеные юзером совпали
                    loc_msg.setType( VPrn::Ans_CHECK_DOC_ATR_EQU );
                    break;
                case VPrn::Ans_CHECK_DOC_ATR_NEQ:
                    // Результаты глубокого поиска поля док. в БД и введеные юзером не совпали
                    loc_msg.setType( VPrn::Ans_CHECK_DOC_ATR_NEQ );
                    break;
                case VPrn::Ans_MB_LIST:
                    // Получили затребованный список документов, отдами его клиенту
                    loc_msg.setType( VPrn::Ans_MB_LIST );
                    loc_msg.setMessage( r_msg.messageData() );
                    break;
                case VPrn::Ans_PRINT_ALLOWED:
                    str = QObject::trUtf8("Вам разрешена печать на выбранный принтер!");
                    loc_msg.setType(VPrn::Ans_PRINT_ALLOWED);
                    loc_msg.setMessage(  str.toUtf8() );
                    break;
                case VPrn::Ans_PRINT_DENIED:
                    str = QObject::trUtf8("Вам запрещена печать на выбранный принтер!");
                    loc_msg.setType(VPrn::Ans_PRINT_DENIED);
                    loc_msg.setMessage(  str.toUtf8() );
                    break;
                case VPrn::Ans_PRINTER_NOT_FOUND:
                    str = QObject::trUtf8("Выбранный Вами принтер, в настоящее время отключен от системы или удален!");
                    loc_msg.setType(VPrn::Ans_PRINTER_NOT_FOUND);
                    loc_msg.setMessage(  str.toUtf8() );
                    break;
                case VPrn::Ans_PRINTER_LIST:
                    loc_msg.setType(VPrn::Ans_PRINTER_LIST);
                    loc_msg.setMessage( m_body.toUtf8() );
                    break;
                case VPrn::Ans_PRINTER_LIST_EMPTY:
                    loc_msg.setType(VPrn::Ans_SrvStatusNotReady);
                    str = QObject::trUtf8("Данному пользователю не назначен ни один принтер!");
                    loc_msg.setMessage(  str.toUtf8() );
                    break;
                case VPrn::Ans_MANDAT_LIST:
                    loc_msg.clear();
                    loc_msg.setType(VPrn::Ans_SrvStatusPartReady);
                    str = QObject::trUtf8("[%1];:;%2").arg(this->u_login,m_body);
                    loc_msg.setMessage(  str.toUtf8() );
                    break;
                case VPrn::Ans_MANDAT_LIST_EMPTY:
                    loc_msg.setType(VPrn::Ans_SrvStatusNotReady);
                    str = QObject::trUtf8("Данному пользователю не назначен ни один мандат!");
                    loc_msg.setMessage(  str.toUtf8() );
                    break;
                case VPrn::Ans_STAMP_LIST:
                    //m_body содержит список уровней секретности
                    loc_msg.setType(VPrn::Ans_STAMP_LIST);
                    loc_msg.setMessage(  m_body.toUtf8() );
                    break;
                }
                if (loc_msg.type() != VPrn::NoMsgType ){
                    // Запись в локальный слот клиенту
                    sendMessage(loc_msg,client);
                }
            }
        }else{
            setError(QObject::trUtf8("Ошибочный ответ от демона СУРД!"));
        }
    }
}

void serverGears::doJobFinish(const QString &m_uuid, VPrn::Jobs job_id,int code
                              ,const QString &output)
{
    Message loc_msg(this);
    QLocalSocket *client(0);
    // По UUID определим какому клиенту надо было это сообщение
    client = findClient(m_uuid);
    if (client){
        // Жив еще голубчик так уж и быть вернем ему результаты,рабского труда
        if (code !=0){
            // Рабский поток, умер от непосильного труда, посмертное сообщение в output
            loc_msg.setType(VPrn:: Err_Message);
            loc_msg.setMessage( output.toUtf8() );
        }else{
            switch(job_id){
            case VPrn::job_ConvertPs2Pdf:
                loc_msg.setType(VPrn::Ans_Convert2PdfFinish);
                break;
            case VPrn::job_CalcPageCount:
                loc_msg.setType(VPrn::Ans_PageCounting);
                loc_msg.setMessage( output.toUtf8() );  // число страниц в документе
                break;
            case VPrn::job_SplitPageFirst:
                loc_msg.setType(VPrn::Ans_PageSplittedFirst);
                break;
            case VPrn::job_SplitPageOther:
                loc_msg.setType(VPrn::Ans_PageSplittedOther);
                break;
            }
        }
        // Запись в локальный слот клиенту
        sendMessage(loc_msg,client);
    }
}

void serverGears::client_init()
{
    QLocalSocket *client = m_server->nextPendingConnection();
    clients.insert(client);
    clients_uuid.insert(client,this->getUuid());

    connect(client, SIGNAL(readyRead()),
            this,   SLOT(readyRead()));
    connect(client, SIGNAL(disconnected()),
            this,   SLOT(disconnected()));
    connect(client, SIGNAL(error(QLocalSocket::LocalSocketError)),
            this,   SLOT(prepareError(QLocalSocket::LocalSocketError)));
    setCheckPoint(VPrn::loc_NewClientStarted);
}

void serverGears::disconnected()
{
    QLocalSocket *client = (QLocalSocket*)sender();
    QString clientName = clients_name.value(client);
    qDebug() << "Client disconnected:" << clientName;
    QString c_uuid =  clients_uuid.value(client);

    clients.remove(client);
    clients_uuid.remove(client);
    clients_name.remove(client);
    //foreach(QLocalSocket *client, clients){
    // Оповестим тех кто заинтересован в том что клиент отвалился
    //client->write(QString("Server:" + user + " has left.\n").toUtf8());
    //}
    setCheckPoint(VPrn::loc_Disconnected);
    //Потребуем  от сервера удалить все файлы, которые создавали в интересах клиента

    emit clearClientSpool( c_uuid );
    // Проверка что еще остались бойцы
    if (clients.isEmpty()){
        //Нет ни одного подключенного клиента, заканчиваем работу
        setCheckPoint(VPrn::loc_NeedShutdown);
    }
}
//-------------------------- PRIVATE -------------------------------------------
void serverGears::setError(const QString &info)
{
    e_info  = info;
    emit error(e_info);
}

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
        switch (m_msg.type()){
        case VPrn::Que_RegisterDocInBase:
            str.append(m_msg.messageData()); /// В теле сообщения query_sql;
            // Просто перешлем в сеть
            message.setType   ( VPrn::Que_RegisterDocInBase );
            message.setMessage(
                    QString("[%1];:;%2;:;%3").arg( c_uuid, str, u_login )
                    .toUtf8() );
            //Запись в сетевой канал
            if (net_plugin){
                net_plugin->sendMessage(message);
            }
            break;
        case VPrn::Que_GET_MB_LISTS:
            str.append(m_msg.messageData()); /// В теле сообщения query_sql;
            // Просто перешлем в сеть
            message.setType   ( VPrn::Que_GET_MB_LISTS );
            message.setMessage(
                    QString("[%1];:;%2;:;%3").arg( c_uuid, str, u_login )
                    .toUtf8() );
            //Запись в сетевой канал
            if (net_plugin){
                net_plugin->sendMessage(message);
            }
            break;
        case VPrn::Que_IS_MB_EXIST:
            str.append(m_msg.messageData()); /// В теле сообщения query_sql;
            // Просто перешлем в сеть
            message.setType   ( VPrn::Que_IS_MB_EXIST );
            message.setMessage(
                    QString("[%1];:;%2;:;%3").arg( c_uuid, str, u_login )
                    .toUtf8() );
            //Запись в сетевой канал
            if (net_plugin){
                net_plugin->sendMessage(message);
            }
            break;
       case VPrn::Que_CHECK_DOC_ATR:
            str.append(m_msg.messageData()); /// В теле сообщения query_sql;
            // Просто перешлем в сеть
            message.setType   ( VPrn::Que_CHECK_DOC_ATR );
            message.setMessage(
                    QString("[%1];:;%2;:;%3").arg( c_uuid, str, u_login )
                    .toUtf8() );
            //Запись в сетевой канал
            if (net_plugin){
                net_plugin->sendMessage(message);
            }
            break;
        case VPrn::Que_AUTHOR_USER:
            // Запрос авторизации на устройство
            str.append(m_msg.messageData()); /// В теле сообщения device_uri;
            // Просто перешлем в сеть
            message.setType(VPrn::Que_AUTHOR_USER);
            message.setMessage(
                    QString("[%1];:;%2;:;%3;:;%4").arg( c_uuid, str, u_mandat,u_login )
                    .toUtf8() );
            if (net_plugin){
                net_plugin->sendMessage(message);
            }
            break;
        case VPrn:: Que_Convert2Pdf:
            /// Клиент потребовал преобразовать ps файл в pdf
            str.append(m_msg.messageData()); /// В теле сообщения полный путь к файлу
            if (gs_plugin){
                gs_plugin->convertPs2Pdf( c_uuid,str );
            }
            break;
       case VPrn::Que_CreateFormatedFullDoc:
            // Запрос формирование полного документа, как для печати,
            // нужно вернуть список png страничек сделанного документа
            createFormatedDoc(c_uuid,true,false,true, m_msg.messageData());
            break;
       case VPrn::Que_CreateFormatedFullDocAndPrint:
            // Запрос формирование полного документа, для печати,
            //  и распечатка его
            createFormatedDoc(c_uuid,true,true,false, m_msg.messageData());
            break;

       case VPrn::Que_CreateFormatedPartDoc:
            // Запрос формирование частичного документа, как для печати,
            // нужно вернуть список png страничек сделанного документа
            createFormatedDoc(c_uuid,false,true, true,m_msg.messageData());
            break;
        case VPrn::Que_Register:
            /// Клиент только подключился, в теле сообщения его самоназвание запомним его
            /// сообщим ему что он авторизирован и вернем присвоенный uuid в теле сообщения uuid
            str.append(m_msg.messageData());
            //clients_name.insert( lient, str);

            message.setType(VPrn::Ans_Register);
            message.setMessage(  c_uuid.toUtf8() ); // Пробразуем в QByteArray
            sendMessage( message,client) ;

            break;
        case VPrn::Que_ServerStatus:
            // Клиент запросил состояние сервера
            message.clear();
            str.clear();
            if (!netDemonReady){
                str = QObject::trUtf8("Нет ответа от шлюза в СУРД или отсутсвует сетевое соединение");
                message.setType(VPrn::Ans_SrvStatusNotReady);
                message.setMessage(  str.toUtf8() );
                // Запись в локальный слот клиенту
                sendMessage(message,client);
            }else{
                if (!this->u_login.isEmpty()){
                    if (this->u_mandat.isEmpty()){
                        //запрос списка мандатов к которым допущен пользователь
                        /// @todo  Показать Мише как разбирать!!!!!!
                        message.setType(VPrn::Que_MANDAT_LIST);
                        str = QObject::trUtf8("[%1];:;%2").arg( c_uuid,u_login );
                        message.setMessage( str.toUtf8() );
                        //Запись в сетевой канал
                        if (net_plugin){
                            net_plugin->sendMessage(message);
                        }

                    }else{
                        str = QObject::trUtf8("%1;:;%2").arg(u_login,u_mandat);
                        message.setType(VPrn::Ans_SrvStatusFullReady);
                        message.setMessage(  str.toUtf8() );
                        // Запись в локальный слот клиенту
                        sendMessage(message,client);
                    }
                }else{
                    str = QObject::trUtf8("Ошибка аутинтификации пользователя.Логин не определен или пустой!");
                    message.setType(VPrn::Ans_SrvStatusNotReady);
                    message.setMessage( str.toUtf8() );
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
                message.setMessage(  str.toUtf8() );
                // Запись в локальный слот клиенту
                sendMessage(message,client);
            }else{
                message.setType(VPrn::Que_SEC_LEVEL);
                if (u_mandat.isEmpty()){
                    u_mandat.append(m_msg.messageData());
                }
                str = QObject::trUtf8("[%1];:;%2").arg(c_uuid,u_mandat);
                message.setMessage( str.toUtf8() );
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
                message.setMessage(  str.toUtf8() );
                // Запись в локальный слот клиенту
                sendMessage(message,client);
            }else{
                message.setType(VPrn::Que_GET_PRINTER_LIST);
                str = QObject::trUtf8("[%1];:;%2;:;%3").arg(c_uuid,u_login,u_mandat);
                message.setMessage( str.toUtf8() );
                //Запись в сетевой канал
                if (net_plugin){
                    net_plugin->sendMessage(message);
                }
            }
            break;
        }
    }
}

void serverGears::sendMessage( const Message &m_msg, QLocalSocket *client)
{
    QString clientName = clients_uuid.value(client);

    qDebug() << "Send Message type " << m_msg.type()
            << "to Client "  << clientName;

    //Сформируем пакет И пошлем его ветром гонимого клиенту
    client->write(m_msg.createPacket());
    client->flush();
}

void serverGears::createFormatedDoc(const QString &c_uuid,
                       bool full_doc,bool delAfterCreate,bool gen_preview,
                       QByteArray data)
{
    QStringList templ_pages;
    if (tmpl_plugin){
        templ_pages = tmpl_plugin->loadAndFillTemplateCreatePages(c_uuid,data);
    }
}
QLocalSocket *serverGears::findClient(const QString &c_uuid)
{
    QLocalSocket *client(0);
    QMapIterator<QLocalSocket *,QString>  i(clients_uuid);
    while (i.hasNext()) {
        i.next();
        qDebug() << "\nClient Key: " << i.key() << " value: " << i.value();
        if (i.value() == c_uuid ){
            return i.key(); //Я работаю только с живыми клиентами
        }
    }
    // Только некромантам интересны мертвые клиенты
    setError(QObject::trUtf8("Ответ клиенту который уже отсоединился!"));
    return client;
}


