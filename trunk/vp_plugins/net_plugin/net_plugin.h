#ifndef NET_PLUGIN_H
#define NET_PLUGIN_H

#include <QObject>
#include <QtPlugin>
#include <QTcpSocket>

#include "inet_plugin.h"

#include "mytypes.h"
#include  "message.h"

using namespace VPrn;

class net_plugin :public QObject, Inet_plugin
{
    Q_OBJECT
    Q_INTERFACES(Inet_plugin)

public:
    net_plugin(QObject *parent=0);
    //socketState state() const;
    void init(const QString &host,int port,const QString &sid);
    void sendMessage(const Message &s_msg);

signals:
    void error(pluginsError errCode,QString error_message);
    /**
      * @fn void messageReady( const Message &msg );
      * @brief Как только полученно полноценное сообщение из сети оно в 
      * формате Message передается основному приложению
      */
    void messageReady( const Message &msg );
    //void stateChanged(socketState state);

private slots:
    void readyRead();
    void onConnected();
    /**
      * @fn selectError(QAbstractSocket::SocketError err);
      * @brief Обработчик ошибок возникших в сокете
      */
    void selectError(QAbstractSocket::SocketError err);


private:
    /**
      * @var HostName;         Имя хоста для связи
      * @var Port;             Порт для связи
      * @var client;           Указатель на клиента
      * @var packetSize;       Размер передаваемого блока данных
      * @var Sid;              Уникальный номер      
      * @var m_state;          Статус Локального сервера
      * @var e_info;           Последняя возникшая ошибка
      */

    QString HostName;
    quint16 Port;
    QTcpSocket *client;  
    qint32 packetSize;
    QString Sid;
    //socketState m_state;
    QString e_info;
    /**
      * @fn setError(const QString &info);
      * @brief Устанавливает состояние ошибки,  записывает новое описание ошибки
      */
    void setError(const QString &info);
    /**
      * @fn void setState(socketState state);
      * @brief Меняет состояние и испускает соответсвующий сигнал
      */
    //void setState(socketState state);
};


#endif
