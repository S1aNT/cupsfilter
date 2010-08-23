#ifndef MYSOCKETCLIENT_H
#define MYSOCKETCLIENT_H
/**
  * @class Базовый клиент сокетный клиент, является базовым классом для реализации
  * в последующем специализированных клиентов наследующих от \sa QLocalSocket
  * или от \sa QTcpSocket. Клиенты реализуют абстрактые функции:
  * \fn onConnect()    реализующую обработку сигнала connected
  * \fn parseMessage() реализующую обработку полностью полученного сообщения
  * типа \sa \class Message.
  */
#include <QObject>

#include <QtNetwork/QLocalSocket>
#include "message.h"

#include "mytypes.h"
using namespace VPrn;


class mySocketClient : public QLocalSocket
{
    Q_OBJECT
public:


    explicit mySocketClient(QLocalSocket *parent = 0);
    QString lastError() const {return e_info;}
    void sendMessage(const Message &msg);

signals:
    void messageReady( const Message &msg );
    void checkPointChanged(MyCheckPoints state);

private slots:
    /**
      * @fn void readyRead()
      * @brief Сервер передает данные, прочтем, преобразуем в сообщение Message
      */
    void readyRead();
    void do_connected();
    void do_error(QLocalSocket::LocalSocketError r_error);
private:
    /**
      * @var packetSize; Размер передаваемого блока данных
      * @var m_state;    Статус клиента
      * @var e_info;     Последняя возникшая ошибка
      * @var client;     Указательна клиента
      */

    qint32 packetSize;

    QString e_info;
     MyCheckPoints m_chekpoints;
    /**
      * @fn setError(const QString &info);
      * @brief Устанавливает состояние ошибки,  записывает новое описание ошибки
      */
    void setError(const QString &info);
    /**
      * @fn void setMyState(socketState state);
      * @brief Меняет состояние и испускает соответсвующий сигнал
      */
    void setCheckPoint(MyCheckPoints m_cpoint);
};


#endif // MYSOCKETCLIENT_H
