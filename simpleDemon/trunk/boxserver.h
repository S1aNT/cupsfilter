#ifndef BOXSERVER_H
#define BOXSERVER_H

#include <QStringList>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QMapIterator>
#include <QSet>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>


#include "tech_global.h"

class BoxServer : public QTcpServer
{
    Q_OBJECT
public:
    BoxServer(QObject *parent=0);
private slots:
	void readyRead();
	void disconnected();
//        void sendUserList();

protected:
	void incomingConnection(int socketfd);
	void doCommand(const QString &user,int command,const QString &body);
private:
	QSet<QTcpSocket*> clients;
	QMap<QTcpSocket*,QString> clientsUID; //Соединение, SID

};

#endif // BOXSERVER_H
