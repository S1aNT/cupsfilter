#ifndef NET_PLUGIN_H
#define NET_PLUGIN_H

#include <QObject>
#include <QtPlugin>
#include <QTcpSocket>

#include "inet_plugin.h"

#include "mytypes.h"

using namespace VPrn;



class net_plugin :public QObject, Inet_plugin
{
    Q_OBJECT
    Q_INTERFACES(Inet_plugin)

public:
    net_plugin(QObject *parent=0);
    void init(const QString &host,int port,const QString &sid);
    void sendData(const QString &cmd);
    //bool state(){return con_state;};

signals:
    void serverResponse(QString &line);
    void error(pluginsError errCode,QString error_message);


private:
    QString HostName;
    quint16 Port;
    QTcpSocket *client;
    //bool con_state;
    QString Sid;

private slots:
    void readyRead();
    void onConnected();
    void selectError(QAbstractSocket::SocketError err);

};


#endif
