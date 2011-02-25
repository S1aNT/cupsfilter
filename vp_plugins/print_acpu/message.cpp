#include "message.h"
#include <QtCore/QDebug>
#include <QtCore/QMetaType>
#include <QtCore/QDataStream>

Message::Message(QObject *parent)
    : QObject(parent)
    , messageType( VPrn::NoMsgType )
    , msgData()
    , m_valid(false)
{
   clear();

}



void Message::setType( MessageType tp )
{
    messageType =  tp ;
}

MessageType Message::getType() const
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

QByteArray Message::getMessageData() const
{
    return msgData;
}


QStringList Message::getMessageDataList() const
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
    m_valid = true;
}


