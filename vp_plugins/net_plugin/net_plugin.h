#ifndef NET_PLUGIN_H
#define NET_PLUGIN_H

#include <QObject>
#include <QDebug>
#include <QTcpSocket>
#include <QtPlugin>

#include "inet_plugin.h"

// timeouts are in ms
static const int i_timeout_connect	= 3000;
static const int i_timeout_read		= 3000;
static const int i_timeout_write	= 3000;

class net_plugin :public QObject, Inet_plugin
{
    Q_OBJECT
    Q_INTERFACES(Inet_plugin)

public:
    net_plugin(QObject *parent=0);
    void init(QString &host,int port);
    void sendData(const QString &cmd);
    bool state(){return conn_state;};

signals:
    void serverResponse(QString line);
    void error(const QString &error_message);

private:
    QString HostName;
    quint16 Port;
    QTcpSocket *client;
    bool con_state;

private slots:
    void readyRead();
    void onConnected();
//    void error (QAbstractSocket::SocketError socketError) 

};


#endif
