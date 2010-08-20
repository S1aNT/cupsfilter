#include "loc_client.h"

#include <QtCore/QByteArray>
#include <QtCore/QDataStream>
#include <QtCore/QUuid>


LocalClient::LocalClient(QObject *parent)
    :QObject(parent)
    , socket(0)
    , m_srvName(QString())
    , e_info(QString())
    , ready(false)
    , packetSize(-1)
{
    socket = new QLocalSocket(this);

    connect(socket, SIGNAL(readyRead()),
            this,   SLOT(readData()));
    connect(socket, SIGNAL(error(QLocalSocket::LocalSocketError)),
            this,   SLOT(prepareError(QLocalSocket::LocalSocketError)));
    connect(socket,SIGNAL(connected()),
            this,  SLOT(makeHandShake()));
}

void LocalClient::makeHandShake()
{
    Message message( this );
    message.setType(VPrn::Que_Register);
    sendMessage(message);
    //setState(VPrn::Connected);
}

//LocalClientState LocalClient::state() const
//{
//    return m_state;
//}

void LocalClient::sendMessage( const Message &m_msg)
{
    if (socket->state() == QLocalSocket::ConnectedState){

        qDebug() << "\nserver name " << socket->fullServerName()
                << "\npacket " << m_msg.createPacket();
        socket->write(m_msg.createPacket());
        socket->flush();
    }else{
        setError(tr("Отсутствует соединение с локальным сервером!"));
    }
}

void LocalClient::parseMessage(const Message &m_msg)
{
    qDebug() << "Recive message " << m_msg.messageData();

    switch (m_msg.type()) {
    case VPrn::Ans_Register:
        //setState(VPrn::Auhtorized);
        break;
    }
}

void LocalClient::setServerName(const QString &sName)
{
    if (!sName.isEmpty()){
        if (socket->state() == QLocalSocket::ConnectedState){
            socket->disconnectFromServer();
        }
        m_srvName = sName;
        socket->connectToServer(m_srvName);
        if (socket->waitForConnected(1000)){
            //setState(VPrn::Connected);
            qDebug("Connected!");
        }
    }else{
        setError(tr("Отсутствует соединение с локальным сервером!"));
    }
}

LocalClient::~LocalClient()
{
    if (socket->state() == QLocalSocket::ConnectedState){
        socket->disconnectFromServer();
        //setState(VPrn::Disconnected);
    }
}

void LocalClient::readData()
{

    QDataStream in ( socket);
    in.setVersion(QDataStream::Qt_4_0);

    while (socket->bytesAvailable() > 0){
        if (packetSize == -1) {

            if( socket->bytesAvailable() < sizeof(packetSize) ){
                return;
            }

            in >> packetSize;
        }

        if (socket->bytesAvailable() < packetSize){
            return;
        }

        packetSize = -1;

        int m_Type;
        in >> m_Type;

        QByteArray msg;
        in >> msg;
        Message message( this );
        message.setType((MessageType) m_Type);
        message.setMessage( msg );
        parseMessage(message);
    }
}

void LocalClient::prepareError(QLocalSocket::LocalSocketError socketError)
{
    switch (socketError) {
    case QLocalSocket::ConnectionRefusedError:
        emit error(VPrn::SocketError,
			QObject::trUtf8("Соединение отклонено удаленным сервером %1\n%2")
			.arg(m_srvName)
			.arg( QString(Q_FUNC_INFO) )
		   );

        break;
    default:
        emit error(VPrn::SocketError,
			QObject::trUtf8("При работе с сервером %1 произошла ошибка :%2\n%3")
			.arg( m_srvName, socket->errorString() )
			.arg( QString(Q_FUNC_INFO) )
		   );
    }
}



