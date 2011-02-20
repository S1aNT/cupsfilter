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
    void sendMessage(const Message &msg);
signals:
    void messageReady( const Message &msg );
    void error(VPrn::AppErrorType eCode, const QString &e_info);
    /**
      * @fn void connected2client();
      * @brief при успешном соединении с клиентом испускается сигнал СОЕДИНЕН
      */
    void connected2client();
private slots:
    /**
      * @fn void readyRead()
      * @brief Сервер передает данные, прочтем, преобразуем в сообщение Message
      */
    void readyRead();
   // void do_connected();
    void do_error(QLocalSocket::LocalSocketError r_error);
private:
    /**
      * @var packetSize; Размер передаваемого блока данных
      */
    PACKET_SIZE packetSize;
};

#endif // MYSOCKETCLIENT_H
