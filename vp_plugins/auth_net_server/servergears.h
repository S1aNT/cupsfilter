#ifndef SERVERGEARS_H
#define SERVERGEARS_H

#include <QObject>
#include <QLocalSocket>

class QByteArray;

class serverGears : public QObject
{
    Q_OBJECT
public:
    explicit serverGears(QObject *parent = 0,const QString &srvName = QString());



    //signals:

    //public slots:
private slots:
    /**
      * @fn void readData();
      * @brief Читает блок данных из сокета
      */
    void readData();
    /**
      * @fn void prepareError(QLocalSocket::LocalSocketError socketError);
      * @brief Обработчик ошибок возникших в сокете
      */
    void prepareError(QLocalSocket::LocalSocketError socketError);
private:
    /**
      * @var m_serverName; имя локального сервера для общения с внешним миром
      * @var connState; Статус соединения с локальным сервером
      * @var socket; Основной рабочий сокет
      * @var blockSize; Размер передаваемого блока данных
      * @var currentDataBlock; Текущий полученный блок данных
      */
    bool connState;
    QString m_serverName;
    QLocalSocket *socket;
    //QLocalServer *server;
    QByteArray currentDataBlock;
    quint16 blockSize;

    //-----------------------------------------------------------------------------
    /**
      * @fn void sendCommand(const QString &cmd);
      * @brief Передача локальному серверу команды
      */
    void sendCommand(const QString &cmd);
    /**
      * @fn bool isLocalServerExist();
      * @brief возвращает статус работы, с локальным сервером или с плагинами
      */
    bool isLocalServerExist(){return connState;};
};

#endif // SERVERGEARS_H
