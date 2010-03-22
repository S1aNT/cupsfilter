#include "message.h"

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

void Message::setMessage( const QByteArray &msg )
{
    msgData = msg;
}

QByteArray Message::messageData() const
{
    return msgData;
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
    out.setVersion(QDataStream::Qt_4_0);
    // ¬ставим размер пакета равный нулю, но отведем под него 4 байта
    out << (qint32)0;

    //¬ставим формат протокола
    out<<VPrn::format;

    //¬ставим “ип сообщени€ и само сообщение в пакет
    out << ( int ) this->type();
    out << this->msgData;
    // возврат на начало блока
    out.device()->seek(0);
    // ¬ычислим размер блока данных и запишем их на свое место
    out << (qint32)(packet.size() - sizeof(qint32));

//    qDebug() << "\ncreatePacket(): after creating "
//             << " packet " << packet
//             << " full packet size: " << packet.size();
    return packet;
}

