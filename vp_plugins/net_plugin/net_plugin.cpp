#include "net_plugin.h"


net_plugin::net_plugin(QObject *parent)
{

}

void net_plugin::onConnected()
{
    QString message=QString("/me:%1");//.arg(SID_string);
    client->write(QString(message + "\n").toUtf8());
   
}
void net_plugin::readyRead()
{
    while(client->canReadLine())
    {
        QString line = QString::fromUtf8(client->readLine()).trimmed();
        qDebug() << "Read line and send to app:" << line;
        emit serverResponse(line);
    }

}


void net_plugin::init(QString &host,int port)
{
    this->HostName = host;
    this->Port = port;

    client = new QTcpSocket(this);
    connect(client, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(client, SIGNAL(connected()), this, SLOT(onConnected()));
    client->connectToHost(HostName,Port);
    if(!client->waitForConnected(i_timeout_connect) ){
        QString msg=QObject::trUtf8("Error: %1\n%2");
        msg.arg(socket->error(),0,10).arg(socket->errorString());
	emit error(msg);
    }

}

void net_plugin::sendData(const QString &cmd)
{
    if(!cmd.isEmpty())
    {
        client->write(QString(cmd + "\n").toUtf8());
    }
}



Q_EXPORT_PLUGIN2(Inet_plugin, net_plugin);
