#include "servergears.h"
#include <QDebug>
#include <QtCore/QByteArray>
#include <QtCore/QDataStream>
#include <QtCore/QUuid>
#include <QtCore/QRegExp>
#include <QtCore/QMapIterator>


serverGears::serverGears(QObject *parent,const QString &srvName)
    : QLocalServer(parent)   
    , packetSize(-1)
    , m_checkPoint(VPrn::glob_Init)
    , e_info(QString())
    , net_plugin(0)
    , u_login(QString())
    , u_mandat(QString())
    , netDemonReady(false)

{
    /// Регистрируем типы @todo Надо сделать отдельную функцию,
    /// в которой регистрировать все типы и вызвать ее из main

    qRegisterMetaType<VPrn::MessageType>("MessageType");

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
    net_plugin = NetPlugin;
}
//------------------------- PRIVATE SLOTS --------------------------------------
void serverGears::prepareError(QLocalSocket::LocalSocketError socketError)
{
    QLocalSocket *client = (QLocalSocket*)sender();
    QString clientName = clients_name[client];

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
    //    QString clientName = clients_uuid[client];

    //Свяжем поток и сокет
    QDataStream in ( client );
    in.setVersion(QDataStream::Qt_4_0);

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
        /*
        //Прочтем и проверим формат протокола
        qint32 m_format;
        in >> m_format;
        if( m_format != format ) {
            setError(QObject::trUtf8("Ошибка в формате протокола, при обмене данными с клиентом %1")
                     .arg(clientName));   
            return;
        }
*/
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
        parseMessage(message,client);
        //foreach(QLocalSocket *client, clients)
    }
}

void serverGears::disconnected()
{
    QLocalSocket *client = (QLocalSocket*)sender();
    QString clientName = clients_name[client];
    qDebug() << "Client disconnected:" << clientName;

    clients.remove(client);
    clients_uuid.remove(client);
    clients_name.remove(client);
    //foreach(QLocalSocket *client, clients){
    // Оповестим тех кто заинтересован в том что клиент отвалился
    //client->write(QString("Server:" + user + " has left.\n").toUtf8());
    //}
    setCheckPoint(VPrn::loc_Disconnected);
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
            if (client){
                switch (r_msg.type()){
                case VPrn::Ans_PRINTER_LIST:
                    loc_msg.setType(VPrn::Ans_PRINTER_LIST);
                    loc_msg.setMessage(  m_body.toUtf8() );
                    // Запись в локальный слот клиенту
                    sendMessage(loc_msg,client);
                    break;
                case VPrn::Ans_PRINTER_LIST_EMPTY:
                    loc_msg.setType(VPrn::Ans_SrvStatusNotReady);
                    str = QObject::trUtf8("Данному пользователю не назначен ни один принтер!");
                    loc_msg.setMessage(  str.toUtf8() );
                    // Запись в локальный слот клиенту
                    sendMessage(loc_msg,client);
                    break;

                case VPrn::Ans_MANDAT_LIST:
                    loc_msg.clear();
                    loc_msg.setType(VPrn::Ans_SrvStatusPartReady);
                    str = QObject::trUtf8("[%1];:;%2").arg(this->u_login,m_body);
                    loc_msg.setMessage(  str.toUtf8() );
                    // Запись в локальный слот клиенту
                    sendMessage(loc_msg,client);
                    break;
                case VPrn::Ans_MANDAT_LIST_EMPTY:
                    loc_msg.setType(VPrn::Ans_SrvStatusNotReady);
                    str = QObject::trUtf8("Данному пользователю не назначен ни один мандат!");
                    loc_msg.setMessage(  str.toUtf8() );
                    // Запись в локальный слот клиенту
                    sendMessage(loc_msg,client);
                    break;
                case VPrn::Ans_STAMP_LIST:
                    //m_body содержит список уровней секретности
                    loc_msg.setType(VPrn::Ans_STAMP_LIST);
                    loc_msg.setMessage(  m_body.toUtf8() );
                    // Запись в локальный слот клиенту
                    sendMessage(loc_msg,client);
                    break;
                }
            }else{
                setError(QObject::trUtf8("Ответ клиенту который уже отсоединился!"));
            }
        }else{
            setError(QObject::trUtf8("Ошибочный ответ от демона СУРД!"));
        }
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


//-------------------------- PRIVATE -------------------------------------------
void serverGears::setError(const QString &info)
{
    /// @todo Испускать сигнал об ошибке !!!
    e_info  = info;
}

void serverGears::setCheckPoint(MyCheckPoints cp)
{
    m_checkPoint = cp;
    emit checkPointChanged(m_checkPoint);
}

void serverGears::parseMessage( const Message &m_msg, QLocalSocket *client)
{
    setCheckPoint(VPrn::loc_MessageRecive);

    Message message( this );

    QString clientUUID = clients_uuid[client];   
    QString str;
    qDebug() << "Resive Message type " << m_msg.type()
            << "from Client "  << clientUUID;

    switch (m_msg.type()){
    case VPrn::Que_Register:
        /// Клиент только подключился, в теле сообщения его самоназвание запомним его 
        /// сообщим ему что он авторизирован и вернем присвоенный uuid в теле сообщения uuid        
        str.append(m_msg.messageData());
        clients_name.insert(client, str);        
        message.setType(VPrn::Ans_Register);
        message.setMessage(  clientUUID.toUtf8() ); // Пробразуем в QByteArray
        sendMessage(message,client);
        qDebug() << Q_FUNC_INFO <<  "client_uuid = " <<  clientUUID;
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
                    str = QObject::trUtf8("[%1];:;%2").arg( clientUUID,u_login );
                    message.setMessage( str.toUtf8() );
                    //Запись в сетевой канал
                    net_plugin->sendMessage(message);
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
            str = QObject::trUtf8("[%1];:;%2").arg(clientUUID,u_mandat);
            message.setMessage( str.toUtf8() );
            //Запись в сетевой канал
            net_plugin->sendMessage(message);
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
            str = QObject::trUtf8("[%1];:;%2;:;%3").arg(clientUUID,u_login,u_mandat);
            message.setMessage( str.toUtf8() );
            //Запись в сетевой канал
            net_plugin->sendMessage(message);
        }
        break;
    }   
}

void serverGears::sendMessage( const Message &m_msg, QLocalSocket *client)
{
    QString clientName = clients_uuid[client];

    qDebug() << "Send Message type " << m_msg.type()
            << "to Client "  << clientName;

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
        if (i.value() == c_uuid ){
            client = i.key();
            break;
        }
    }
    return client;
}
