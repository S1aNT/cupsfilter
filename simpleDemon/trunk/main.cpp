#include <QDebug>
#include <QtCore/QCoreApplication>
#include "BoxServer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    BoxServer *server = new BoxServer();
    bool success = server->listen(QHostAddress::Any, 17675);
    if(!success)
    {
	qFatal("Could not listen on port 17675.");
    }

    qDebug() << "Ready";

    return a.exec();
}
