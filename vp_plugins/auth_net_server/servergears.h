#ifndef SERVERGEARS_H
#define SERVERGEARS_H

#include "message.h"
#include "mytypes.h"

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QSet>

#include <QtNetWork/QLocalServer>
#include <QtNetWork/QLocalSocket>


class QByteArray;

using namespace VPrn;

class serverGears : public QLocalServer
{
    Q_OBJECT

public:
    explicit serverGears(QObject *parent = 0,const QString &srvName = QString());    

    bool isError(){return e_state;};
    QString lastError(){return e_info;};

signals:
    void messageReady( const Message &msg );
    void networkProtocolError();

//public slots:
private slots:
    /**
      * @fn void readyRead()
      * @brief Клиент передает данные, прочтем  и определим что он там шлет
      */
    void readyRead();
    /**
      * @fn void disconnected();
      * @brief Клиент покинул, наше общество! Вычеркнем предателя из списка живых
      */
    void disconnected();
    /**
      * @fn void prepareError(QLocalSocket::LocalSocketError socketError);
      * @brief Обработчик ошибок возникших в сокете
      */
    void prepareError(QLocalSocket::LocalSocketError socketError);

    void client_init();

protected:
    /**
      * @fn void incomingConnection(quintptr socketDescriptor);
      * @brief Как только происходит новое подключение к локальному серверу,
      * происходит вызов этой функции, и создание нового Локального сокета
      * работающего с клиентом
      */
    //void incomingConnection(quintptr socketDescriptor);

private:
    /**
      * @var m_serverName;     Имя локального сервера для общения с Локальным миром
      * @var packetSize;       Размер передаваемого блока данных
      * @var currentDataBlock; Текущий полученный блок данных
      * @var e_state;          Статус Есть ошибка или нет
      * @var e_info;           Последняя возникшая ошибка
      * @var clients;          Список подключенных клиентов
      * @var clients_name;     Список имен подключенных клиентов
      */

    QString m_serverName;    
    QLocalServer *m_server;
    qint32 packetSize;
    bool e_state;
    QString e_info;
    QSet<QLocalSocket *> clients;
    QMap<QLocalSocket *,QString> clients_uuid;
//-----------------------------------------------------------------------------
    /**
      * @fn void parseMessage( const Message &msg, const QLocalSocket *client);
      * @brief Обработка сообщения от конкретного клиента
      */
    void parseMessage( const Message &m_msg, QLocalSocket *client);

    /**
      * @fn void sendMessage( const Message &msg, const QLocalSocket *client);
      * @brief Отправка сообщения конкретному клиенту
      */
    void sendMessage( const Message &m_msg, QLocalSocket *client);

    /**
      * @fn setError(const QString &info);
      * @brief Устанавливает состояние ошибки,  записывает новое описание ошибки
      */
    void setError(const QString &info);

};

#endif // SERVERGEARS_H
