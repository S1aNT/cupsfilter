#include "boxserver.h"
#include <QTcpSocket>
#include <QRegExp>

BoxServer::BoxServer(QObject *parent) : QTcpServer(parent)
{
}

void BoxServer::incomingConnection(int socketfd)
{
    QTcpSocket *client = new QTcpSocket(this);
    client->setSocketDescriptor(socketfd);
    clients.insert(client);

    qDebug() << "New client from:" << client->peerAddress().toString();

    connect(client, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

void BoxServer::doCommand(const QString &user,int command,const QString &body)
{
    QTcpSocket *client;
    QMapIterator<QTcpSocket*,QString> i(clientsUID);
    while (i.hasNext()) {
	i.next();

	if (i.value()==user){
	    client = i.key();
	    if (client->isValid()){
	        qDebug() <<client->isValid()<<command<<body;
		switch (command) {
		case GET_PRINTER_LIST_CMD:
		    // разбор тела команды. Формат имя_пользователя:мандат
		    QRegExp rx("^(.+)~(.+)$");
		    if (rx.indexIn(body)!=-1)
		    {
		      QString message="Testprinter1,TestPrinter2,TestPrinter3,TestPrinter4";
	              client->write(QString(message+"\n").toUtf8());
		    }
		    
		    break;
		}
	    }else {
		qDebug() << "Not valid client with ID " << user << " and adress :" <<client->peerAddress().toString();
	    }
	}
    }
}
void BoxServer::readyRead()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    while(client->canReadLine())
    {
	QString line = QString::fromUtf8(client->readLine()).trimmed();

	qDebug() << "Read line:" << line;
	// Формат строки /me:{UID}
	QRegExp meRegex("^/me:(.*)$");

	if(meRegex.indexIn(line) != -1)
	{
	    QString cUID = meRegex.cap(1);
	    qDebug() << QString("Connected new client with UID: %1 ").arg(cUID) << "\n";

	    clientsUID[client] = cUID;
	    client->write(QString("/answer:Client " + cUID + " register.\n").toUtf8());
	    /*
	    foreach(QTcpSocket *client, clients)
		client->write(QString("Server:" + user + " has joined.\n").toUtf8());
	    sendUserList();
	    */
	}else {
	    if(clientsUID.contains(client))
	    {
		QString message = line;
		QString user = clientsUID[client];
		qDebug() << "User:" << user<< " Message:" << message;
		//TODO Обработка запроса клиента
		// Формат /cmd:код_команды:тело_команды
		QRegExp rx("^/cmd:(.+):(.+)$");
		if(rx.indexIn(line) != -1)
		{
		    QString cmd =rx.cap(1);
		    QString body = rx.cap(2);
		    qDebug() <<QString("Recived command %1 with args %2 from client: ").arg(cmd,body) <<client->peerAddress().toString() ;
		    doCommand(user,cmd.toInt(),body);
		}

	    }else
		{
		qWarning() << "Got bad message from client:" << client->peerAddress().toString() << line;
	    }
	}
    }
}

void BoxServer::disconnected()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    qDebug() << "Client disconnected:" << client->peerAddress().toString();
    clients.remove(client);
    clientsUID.remove(client);
}
