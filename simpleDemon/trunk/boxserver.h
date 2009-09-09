#ifndef BOXSERVER_H
#define BOXSERVER_H

#include <QStringList>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QMapIterator>
#include <QSet>



class BoxServer : public QTcpServer
{
    Q_OBJECT
    Q_ENUMS(TypeCmd)
public:
    BoxServer(QObject *parent=0);

	enum TypeCmd
	{
	    getPrinterListCmd,
	    setDocStateCmd
	};

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
