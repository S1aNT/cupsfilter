#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <QTcpSocket>
#include <QString>
class SocketClient : public QTcpSocket
{
    Q_OBJECT

public:
    SocketClient();

protected slots:
    void onConnected();
    void onReadyRead();
};

#endif // SOCKETCLIENT_H
