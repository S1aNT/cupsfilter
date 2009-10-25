#ifndef NETAGENT_H
#define NETAGENT_H
#include <QDebug>
#include <QObject>
#include <QTcpSocket>
#include <QRegExp>
#include <QTimer>
#include "tech_global.h"

class netAgent : public QObject{
      Q_OBJECT

public:
    netAgent( QObject *parent = 0 );

    //void setSid(const QString & sid);
    void on_login(QString &hostname,int Port,QString &sid);


public slots:
	void readyRead();
	void connected();
	void prepareError(QAbstractSocket::SocketError socketError);
	void send2Server(commands_t cmd, QString &body);
signals:
	void error_signal(int Code,const QString &errorStr);
	void takeServerResponce(QString &message); // Передает дальше полученный от сервера ответ
private:
	// This is the socket that will let us communitate with the server.
	QTcpSocket *socket;
	QString client_sid;
};

#endif // NETAGENT_H
