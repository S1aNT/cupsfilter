#ifndef MSGSOCKETTRANSMITTER_H
#define MSGSOCKETTRANSMITTER_H

/**
  * @class Базовый клиент сокетный клиент, реализует методы:
  * \fn void connectToServer(const QString &serverName);
  *          соединяется с локальным сокетным сервером
  * \fn void sendMessageToSocket(const QString &cuid,const Message &s_msg);
  *          отправляет в сокет сообщение s_msg типа \sa \class Message
  *          от конкретного клиента cuid
  * \fn reciveMessageFromSocket(const QString &cuid, const Message &r_msg);
  *          преобразует полученный от локального сокетного сервара пакет
             в сообщение для кокретного клиента и отпаравляет сигнал
  *
  */

#include <QObject>
#include <QLocalSocket>
#include "message.h"
#include "mytypes.h"
using namespace VPrn;

class msgSocketTransmitter : public QLocalSocket
{
    Q_OBJECT
public:
    explicit msgSocketTransmitter(QLocalSocket *parent = 0);

signals:    
    void error(VPrn::AppErrorType eCode, QString e_info);
    void receivedMessageFromSocket(const QString &cuid, const Message &r_msg);

public slots:
    void sendMessageToSocket(const QString &cuid,const Message &s_msg);
private slots:
    /**
      * @fn void assembleMessage();
      * @brief сборка сообщения из пакетов
      */
    void assembleMessage();
private:    
    qint32 m_packetSize;

    QByteArray messageToPacket(const QString &cuid, const Message &msg);
};

#endif // MSGSOCKETTRANSMITTER_H
