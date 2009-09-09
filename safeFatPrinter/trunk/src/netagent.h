#ifndef NETAGENT_H
#define NETAGENT_H
#include <QDebug>
#include <QObject>
#include <QTcpSocket>

class netAgent : public QObject{
      Q_OBJECT
      Q_ENUMS(errorCode)

public:
    netAgent( QObject *parent = 0 );
	enum errorCode
	{
	    noError ,
	    hostnameEmpty ,
	    invalidRangePort ,
	    HostNotFoundError ,
	    ConnectionRefusedError
	};
    void setSid(const QString & sid);

public slots:
	void on_login(QString & hostname,int Port);
	void send2Server(const QString &cmd,const QString &body);
	void readyRead();
	void connected();
	void prepareError(int socketError, const QString &message);
signals:
	void error_signal(netAgent::errorCode Code,const QString &errorStr);
private:
	// This is the socket that will let us communitate with the server.
	QTcpSocket *socket;
	QString client_sid;
};

#endif // NETAGENT_H
