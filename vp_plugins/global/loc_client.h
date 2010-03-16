#ifndef LOC_CLIENT_H
#define LOC_CLIENT_H

#include "message.h"
#include "mytypes.h"

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QSet>

#include <QtNetwork/QLocalSocket>


class QByteArray;

using namespace VPrn;

class LocalClient : public QObject
{
    Q_OBJECT

public:
    explicit LocalClient(QObject *parent = 0);
    ~LocalClient();
   void setServerName(const QString &sName);
   QString lastError() const;
   LocalClientState state() const;

public slots:
    void readData();
    void prepareError(QLocalSocket::LocalSocketError socketError);
    void makeHandShake();
signals:
    void error(const QString eInfo);
    void connected();
    void stateChanged(LocalClientState state);

private:
    QLocalSocket * socket;
    QString m_srvName;
    QString e_info;
    bool ready;
    qint32 packetSize;
    LocalClientState m_state;

    void setError(const QString &info);
    void sendMessage( const Message &m_msg);
    void parseMessage(const Message &m_msg);
    void setState(LocalClientState state);
};


#endif

