#include <QDebug>


#include "net_plugin.h"


net_plugin::net_plugin(QObject *parent)
    : QObject(parent)
    , HostName(QString())
    , Port (0)
    , client(0)
    , packetSize(-1)
    , Sid(QString())
  //  , m_state(VPrn::InitClientState)
    , e_info(QString())
{

}

//---------------------------- PUBLIC ------------------------------------------
void net_plugin::init(const QString &host, int port,const QString &sid)
{
    this->HostName = host;
    this->Port = port;
    this->Sid = sid;

    client = new QTcpSocket(this);
    connect(client, SIGNAL(readyRead()),
            this, SLOT(readyRead()));
    connect(client, SIGNAL(connected()),
            this, SLOT(onConnected()));
    connect(client, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(selectError(QAbstractSocket::SocketError)));

    client->connectToHost(HostName, Port);
/*
    Для отладки    	
*/
    Message message( this );
    message.setType(Ans_RegisterGlobal);
    //message.setMessage( Sid.toUtf8() ); // Пробразуем в QByteArray
    emit messageReady(message);
}

void net_plugin::sendMessage(const Message &m_msg)
{
    //Сформируем пакет И пошлем его ветром гонимого
    client->write(m_msg.createPacket());
    client->flush();
}

//socketState net_plugin::state() const
//{
//    return m_state;
//}

//---------------------------- PRIVATE -----------------------------------------
 void net_plugin::setError(const QString &info)
 {
     e_info = info;
 }

// void net_plugin::setState(socketState state)
// {
//     m_state = state;
//     emit stateChanged(m_state);
// }

//---------------------------- PRIVATE SLOTS -----------------------------------
void net_plugin::readyRead()
{
    //Свяжем поток и сокет
    QDataStream in ( client );
    in.setVersion(QDataStream::Qt_3_0);

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
            setError(tr("Ошибка в формате протокола, при обмене данными с сервером %1")
                     .arg(HostName));
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

        // Отправка сообщения
        emit messageReady(message);
    }
}

void net_plugin::onConnected()
{
    //QString m_body =QString("/me;:;%1;:;%2").arg(Sid).arg(REGISTER_CMD,0,10);
/// @todo k МИШЕ УСТРАНИТЬ ДУБЛИРОВАНИЕ ИНФОРМАЦИИ
    Message message( this );
    message.setType(VPrn::Que_RegisterGlobal);
    message.setMessage( Sid.toUtf8() ); // Пробразуем в QByteArray
    sendMessage(message);
}

void net_plugin::selectError(QAbstractSocket::SocketError err)
{
    QString e_msg;
    switch(err)
    {
    case QAbstractSocket::ConnectionRefusedError :
        e_msg =QObject::trUtf8("Соединение отклоненно удаленным сервером [%1]")
                .arg(HostName);
        break;
    case QAbstractSocket::HostNotFoundError :
        e_msg =QObject::trUtf8("Удаленный сервер не найден!")
               .arg(HostName);
        break;
    case QAbstractSocket::SocketTimeoutError :
        e_msg =QObject::trUtf8("Превышено время ожидания ответа от сервера")
               .arg(HostName);
        break;
    default:
        e_msg = QObject::trUtf8("Код ошибки %1").arg(err,0,10);
    }
    emit error(VPrn::NetworkTransError,e_msg);
    //setState(VPrn::InternalError);
}

Q_EXPORT_PLUGIN2(Inet_plugin, net_plugin);

