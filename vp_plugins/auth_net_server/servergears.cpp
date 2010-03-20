#include "servergears.h"
#include <QDebug>
#include <QtCore/QByteArray>
#include <QtCore/QDataStream>
#include <QtCore/QUuid>


serverGears::serverGears(QObject *parent,const QString &srvName)
    : QLocalServer(parent)   
    , packetSize(0)
    , m_state(VPrn::InitServerState)
    , e_info(QString())

{
    /// Регистрируем типы @todo Надо сделать отдельную функцию,
    /// в которой регистрировать все типы и вызвать ее из main

    qRegisterMetaType<VPrn::MessageType>("MessageType");

    m_serverName = srvName;
    /// Создаем локальный сервер
    m_server = new QLocalServer(this);
    if (!m_server->listen(m_serverName)) {
        setError(tr("Не могу запустить локальный сервер: %1.")
                 .arg(m_server->errorString()));       
        setState(VPrn::NotListenError);
    }else{
        connect(m_server, SIGNAL(newConnection()),
                this,     SLOT(client_init()));

    }
}


LocalServerState serverGears::state() const
{
    return m_state;
}

QString serverGears::getUuid() const
{
    return QUuid::createUuid().toString().replace("{","").replace("}","");
}
//------------------------- PRIVATE SLOTS --------------------------------------


void serverGears::prepareError(QLocalSocket::LocalSocketError socketError)
{
    QLocalSocket *client = (QLocalSocket*)sender();
    QString clientName = clients_uuid[client];

    switch (socketError) {
    case QLocalSocket::ConnectionRefusedError:
        setError(tr("Соединение было сброшенно клиентом [%1]").arg(clientName));
        break;
    case QLocalSocket::PeerClosedError:
        break;
    default:
        setError(tr("При работе с клиентом [%1] произошла ошибка:%2")
                 .arg( clientName, client->errorString() )
                 );
    }
    setState(VPrn::NetworkCommonError);
}

void serverGears::readyRead()
{
    QLocalSocket *client = (QLocalSocket*)sender();
    QString clientName = clients_uuid[client];

    //Свяжем поток и сокет
    QDataStream in ( client );
    in.setVersion(QDataStream::Qt_4_0);

    while (client->bytesAvailable() > 0){
        if (packetSize == -1) {
            //Определим количество байт доступных для чтения;
            //на этом шаге необходимо получить больше 4-х байт
            if( client->bytesAvailable() < sizeof(qint32) ){
                return;
            }
            //Читаем размер пакета
            in >> packetSize;
        }
        //Проверим что в сокет пришел весь пакет а не его огрызки
        if (client->bytesAvailable() < packetSize){
            return;
        }
        //Сбросим размер пакета, для обработки следующего
        packetSize = -1;

        //Прочтем и проверим формат протокола
        qint32 m_format;
        in >> m_format;
        if( m_format != format ) {
            setError(tr("Ошибка в формате протокола, при обмене данными с клиентом %1")
                     .arg(clientName));   
            return;
        }

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
    qDebug() << "Client disconnected:" << client->fullServerName();

    clients.remove(client);

    QString clientName = clients_uuid[client];
    clients_uuid.remove(client);

    //foreach(QLocalSocket *client, clients){
        // Оповестим тех кто заинтересован в том что клиент отвалился
        //client->write(QString("Server:" + user + " has left.\n").toUtf8());
    //}

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

}


//-------------------------- PRIVATE -------------------------------------------


void serverGears::setError(const QString &info)
{
    e_info  = info;
}
void serverGears::setState(LocalServerState state)
{
    m_state = state;
    emit stateChanged(m_state);
}

void serverGears::parseMessage( const Message &m_msg, QLocalSocket *client)
{
    Message message( this );
    QString clientName = clients_uuid[client];
    QByteArray msg_body;

    switch (m_msg.type()){
        // Клиент только подключился, сообщим ему что он авторизирован и вернем
        // ему присвоенный uuid.Тело сообщения  пустое
    case VPrn::Que_Register:
        message.setType(VPrn::Ans_Register);
        message.setMessage( clientName.toUtf8() ); // Пробразуем в QByteArray
        sendMessage(message,client);
        break;
    }

}

void serverGears::sendMessage( const Message &m_msg, QLocalSocket *client)
{
    //Сформируем пакет И пошлем его ветром гонимого клиенту
    client->write(m_msg.createPacket());
    client->flush();
}

