#ifndef NETWORKTHREAD_H
#define NETWORKTHREAD_H

#include <QThread>
#include <QMutex>
#include "message.h"

class NetworkThread : public QThread
{
    Q_OBJECT
public:
    NetworkThread();
    ~NetworkThread();
   // void connectToServer(const QString &srvName,int srvPort);
   // void connectToServer(,int srvPort);

   // void addMessageToQueue(const Message &msg);
protected:
    //void run();
private:
    QMutex mutex;

};

#endif // NETWORKTHREAD_H
