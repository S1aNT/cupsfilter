#include <QtCore/QDebug>
#include <QtCore/QDataStream>
#include "mysocketclient.h"
#include "message.h"



mySocketClient::mySocketClient(QLocalSocket *parent)
        : QLocalSocket (parent)
        , packetSize(-1)
{
    connect(this,SIGNAL(connected()),
            this,SIGNAL(connected2client())
            );
    connect(this,SIGNAL(readyRead()),
            this,SLOT(readyRead()));
    connect(this,SIGNAL(error(QLocalSocket::LocalSocketError)),
            this,SLOT(do_error(QLocalSocket::LocalSocketError))
            );
}


void mySocketClient::readyRead()
{
    //qDebug() << Q_FUNC_INFO << QObject::trUtf8(" Recive %1 bytes \n").arg(this->bytesAvailable(),0,10);
    //Свяжем поток и сокет
    QDataStream in ( this );
    in.setVersion(QDataStream::Qt_3_0);

    while (this->bytesAvailable() > 0){
        if (packetSize == -1) {
            //Определим количество байт доступных для чтения;
            //на этом шаге необходимо получить больше 8-х байт
            if( (PACKET_SIZE) this->bytesAvailable() < (PACKET_SIZE) sizeof(packetSize) ){
                return;
            }
            //Читаем размер пакета
            in >> packetSize;
            //qDebug() << Q_FUNC_INFO << " packet size "  << packetSize << "\n";
        }
        //Проверим что в сокет пришел весь пакет а не его огрызки
        if (this->bytesAvailable() < packetSize){
            return;
        }
        //Сбросим размер пакета, для обработки следующего
        packetSize = -1;
        // Прочтем тип сообщения
        int m_Type;
        in >> m_Type;

        //Прочтем само сообщение
        QByteArray data;
        in >> data;
        Message message( this );
        message.setType((MessageType) m_Type); //Проверить как конвертирует
        message.setMessageData( data );
        // Отправка сообщения
        messageReady( message );
    }
}



void mySocketClient::sendMessage(const Message &msg)
{
    if (this->state() == QLocalSocket::ConnectedState){
        this->write(msg.createPacket());
        this->flush();
    }else{
        emit error(VPrn::SocketError,
                   QObject::trUtf8("Попытка записи данных в закрытый сокет!\n%1")
                   .arg(QString(Q_FUNC_INFO))
                   );
    }
}

void mySocketClient:: do_error(QLocalSocket::LocalSocketError r_error)
{
    switch(r_error){
    case QLocalSocket::ConnectionRefusedError:
        emit error(VPrn::SocketError,
                   QObject::trUtf8("В соединении отказано!\n%1")
                   .arg(QString(Q_FUNC_INFO))
                   );
        break;
    default:
        emit error(VPrn::SocketError,
                   QObject::trUtf8("При работе с %1 произошла ошибка %2.\n%3")
                   .arg(this->serverName(),this->errorString())
                   .arg(QString(Q_FUNC_INFO))
                   );
        break;
    }
}
