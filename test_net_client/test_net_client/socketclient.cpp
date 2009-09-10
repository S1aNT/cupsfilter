#include "socketclient.h"

SocketClient::SocketClient()
{
    connect(this, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(this, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

void /* slot */
SocketClient :: onConnected()
{
    qDebug("Socket has connected, send data: \"/me:xxx-xxxx\"");
    QString  message="/me:xxx-xxxx";
    write(QString(message + "\n").toUtf8());
    message="/cmd:100:xxx~xxxx";
    write(QString(message + "\n").toUtf8());

}

void /* slot */
SocketClient :: onReadyRead()
{
    qDebug() << "ReadyRead signal";
    // We'll loop over every (complete) line of text that the server has sent us:
    while(canReadLine())
    {
	// Here's the line the of text the server sent use UTF-8
	QString line = QString::fromUtf8(readLine()).trimmed();
	if (!line.isEmpty()){
	    qDebug() <<Q_FUNC_INFO<< line;
	}
    }

}
