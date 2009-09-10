#include <QtCore/QCoreApplication>
#include "socketclient.h"
#include <QHostAddress>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    SocketClient socket_client;
    socket_client.connectToHost(QHostAddress("127.0.0.1"), 17675);
    return a.exec();
}
