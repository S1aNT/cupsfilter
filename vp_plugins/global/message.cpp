#include "message.h"
#include <QtCore/QDebug>
#include <QtCore/QMetaType>
#include <QtCore/QDataStream>

Message::Message(QObject *parent)
    : QObject(parent)
    , messageType( VPrn::NoMsgType )
    , msgData()
    , ready( false )

{

}

bool Message::isMessageReady()
{
    return ready;
}

void Message::setType( MessageType tp )
{
    messageType =  tp ;
}

MessageType Message::type() const
{
    return messageType;
}

void Message::setMessageData( const QByteArray &m_data )
{
    msgData = m_data;
}

void Message::setMessageData( QStringList &m_list )
{
    if (!m_list.isEmpty()) {        
        QDataStream out(&msgData, QIODevice::WriteOnly );
        out.setVersion(QDataStream::Qt_3_0);
        out << m_list;
    }
}

QByteArray Message::messageData() const
{
    return msgData;
}


QStringList Message::messageDataList() const
{
    QStringList list;
    QDataStream in( msgData );
    in.setVersion(QDataStream::Qt_3_0);
    in >> list;
    return list;
}

void Message::clear()
{
    setType(VPrn::NoMsgType);
    msgData.clear();

}

QByteArray Message::createPacket() const
{
//    qDebug() << "createPacket(): before creating: \n"
//             << "messageData() "  << this->msgData
//             << "type() "         << this->type();

    QByteArray packet;
    qint32 packet_size(0);
    QDataStream out(&packet, QIODevice::WriteOnly );

    out.setVersion(QDataStream::Qt_3_0);
    // Вставим размер пакета равный нулю, но отведем под него 8 байт
    out << packet_size;

    //Вставим Тип сообщения и само сообщение в пакет
    out << ( int ) this->type();
    out << this->msgData;
    // возврат на начало блока
    out.device()->seek(0);
    // Вычислим размер блока данных и запишем их на свое место
    packet_size =(qint32)(packet.size() - sizeof(qint32));
    out << ( qint32 )  packet_size;
    qDebug() << Q_FUNC_INFO << " packet_size "  << packet_size << "\n";
    return packet;
}

