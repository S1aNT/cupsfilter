#include "msgsockettransmitter.h"
#include <QDataStream>

msgSocketTransmitter::msgSocketTransmitter(QLocalSocket *parent)
    : QLocalSocket (parent)
    , m_packetSize (-1)
{
    connect(this, SIGNAL ( readyRead() ),
            this, SLOT   ( assembleMessage() )
            );
}


// -------------------- Public  slots ------------------------------------------
void msgSocketTransmitter::sendMessageToSocket(const QString &cuid,
                                               const Message &s_msg)
{
    if ( cuid.isEmpty() ){
        emit error(VPrn::InternalAppError,
                   QObject::trUtf8("Адресат сообщения не может быть пустым!\n%1")
                   .arg(QString(Q_FUNC_INFO))
                   );
        return;
    }


    if (this->state () == QLocalSocket::ConnectedState ){
        this->write(messageToPacket(cuid,s_msg) );
    }
}

// -------------------- Private slots ------------------------------------------
void msgSocketTransmitter::assembleMessage()
{
    /**
      qDebug() << Q_FUNC_INFO
               << QObject::trUtf8(" Recive %1 bytes \n")
                                 .arg(this->bytesAvailable(),0,10);
    */
    //Свяжем поток и сокет
    QDataStream in ( this );
    in.setVersion(QDataStream::Qt_3_0);

    while (this->bytesAvailable() > 0){
        if (m_packetSize == -1) {
            //Определим количество байт доступных для чтения;
            //на этом шаге необходимо получить больше sizeof(m_packetSize) байт
            if( (qint32) this->bytesAvailable() < (qint32) sizeof(m_packetSize) ){
                return;
            }
            //Читаем размер пакета
            in >> m_packetSize;
            //qDebug() << Q_FUNC_INFO << " packet size "  << packetSize << "\n";
        }
        //Проверим что в сокет пришел весь пакет а не его огрызки
        if (this->bytesAvailable() < m_packetSize){
            return;
        }

        QString client_uuid; // Кому же был адресован пакет
        qint8 m_Type;        // Тип сообщения
        QByteArray data;     // Данные сообщения

        //Сбросим размер пакета, для обработки следующего
        m_packetSize = -1;

        in >> client_uuid;
        in >> m_Type;
        in >> data;
        // Сформиуем сообщение
        Message message( this );
        message.setType((MessageType) m_Type);
        message.setMessageData( data );
        // Отправка сообщения
        emit receivedMessageFromSocket(client_uuid, message );
    }
}

// -------------------- Private ------------------------------------------------
QByteArray msgSocketTransmitter::messageToPacket(const QString &cuid,
                                                 const Message &msg )
{
    /**
   * @short Структура пакета:
   * qint32      размер пакета
   * QString     адресат сообщения (отправитель или получатель)
   * qint8       Тип сообщения @sa enum MessageType в файле @file mytypes.h
   * QByteArray  Данные сообщения
   */


    QByteArray packet;
    qint32 packet_size(0);


    QDataStream out(&packet, QIODevice::WriteOnly );
    out.setVersion(QDataStream::Qt_3_0);

    out << packet_size;             // Размера пакета [резервируем sizeof(qint32)]
    out << cuid;                    // Адресат пакета
    out << ( qint8 ) msg.getType(); // Тип сообщения
    out << msg.getMessageData();

    out.device()->seek(0);          // Возврат на начало блока
    // Вычислим размер блока данных и запишем их на свое место
    out << ( qint32 ) ( packet.size() - sizeof(qint32) );
    //qDebug() << Q_FUNC_INFO << " packet_size "  << packet_size << "\n";
    return packet;
}
