#ifndef NET_AGENT_H
#define NET_AGENT_H
#include <QDebug>
#include <QObject>
#include <QTcpSocket>
#include <QRegExp>
#include <QTimer>

#include "tech_global.h"

class net_agent : public QObject
{
    Q_OBJECT

public:
    net_agent(QObject *parent = 0 );
public slots:
    void do_connect(QString &host,int port,QString &sid);
    void send2Server(commands_t cmd,QString &body);
    void execQuery(QString &query);
signals:
	void error_signal(int Code,const QString &errorStr);
	void serverSay(QString &message);

private slots:
    void readyRead();
    void connected();
private:
	// This is the socket that will let us communitate with the server.
	QTcpSocket *socket;
	QString client_sid;
};

#endif // NET_AGENT_H
