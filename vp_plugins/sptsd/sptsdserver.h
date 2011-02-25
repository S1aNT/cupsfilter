#ifndef SPTSDSERVER_H
#define SPTSDSERVER_H

#include <qserversocket.h>
#include "sptsd.h"

class sptsdServer : public QServerSocket
{
    Q_OBJECT

public:
    sptsdServer(Q_UINT16 port, int conn, QObject* parent=0 );
    ~sptsdServer();
    void newConnection(int socket);
    QString peerIP;

public slots:
    void clientDisconnected();

signals:
    void newConnect(sptsd*);

};
#endif


