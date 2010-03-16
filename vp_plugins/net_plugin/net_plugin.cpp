#include <QDebug>


#include "net_plugin.h"


net_plugin::net_plugin(QObject *parent)
{

}

void net_plugin::onConnected()
{
    QString message =QString("/me;:;%1;:;%2").arg(Sid).arg(REGISTER_CMD,0,10);
    client->write(QString(message+"\n").toUtf8());

}
void net_plugin::readyRead()
{
    qDebug() << "readyRead";
    while (client->canReadLine()) {
        QString line = QString::fromUtf8(client->readLine()).trimmed();
        qDebug() << "Read line and send to app:" << line;
        emit serverResponse(line);
    }

}

void net_plugin::init(const QString &host, int port,const QString &sid)
{
    this->HostName = host;
    this->Port = port;
    this->Sid=sid;
    client = new QTcpSocket(this);
    connect(client, SIGNAL(readyRead()),
            this, SLOT(readyRead()));
    connect(client, SIGNAL(connected()),
            this, SLOT(onConnected()));
    connect(client, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(selectError(QAbstractSocket::SocketError)));
    client->connectToHost(HostName, Port);
}

void net_plugin::selectError(QAbstractSocket::SocketError err)
{
    QString e_msg;
    switch(err)
    {
    case QAbstractSocket::ConnectionRefusedError :
        e_msg =QObject::trUtf8("Cоединение не выполнено | connection refused");
        break;
    case QAbstractSocket::HostNotFoundError :
        e_msg =QObject::trUtf8("Удаленный сервер не найден | host not found");
        break;
    case QAbstractSocket::SocketTimeoutError :
        e_msg =QObject::trUtf8("Превышено время ожидания ответа от сервера | The socket operation timed out.");
        break;
    default:
        e_msg = QObject::trUtf8("Error code %1").arg(err,0,10);
    }
    emit error(VPrn::NetworkTransError,e_msg);
}

void net_plugin::sendData(const QString &cmd)
{
    qDebug()<< Q_FUNC_INFO << cmd << client->state();

    if (!cmd.isEmpty()) {
        client->write(QString(cmd+"\n").toUtf8());
    }
}

Q_EXPORT_PLUGIN2(Inet_plugin, net_plugin)
;
