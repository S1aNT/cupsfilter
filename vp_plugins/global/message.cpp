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
    QDataStream out(&packet, QIODevice::WriteOnly );

    out.setVersion(QDataStream::Qt_3_0);
    // Вставим размер пакета равный нулю, но отведем под него 4 байта
    out << (qint32)0;

    //Вставим Тип сообщения и само сообщение в пакет
    out << ( int ) this->type();
    out << this->msgData;
    // возврат на начало блока
    out.device()->seek(0);
    // Вычислим размер блока данных и запишем их на свое место
    out << (qint32)(packet.size() - sizeof(qint32));
    //int t_size = packet.size();
    //int t_type = ( int ) this->type();
    //QByteArray t_arr = packet;

    //qDebug() << "\ncreatePacket(): after creating "
    //         << " packet " << t_arr
    //         << " full packet size: " << t_size;


    return packet;
}

