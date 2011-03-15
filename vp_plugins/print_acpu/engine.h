#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>
#include "msgsockettransmitter.h"
#include "mytypes.h"
#include "message.h"

using namespace VPrn;

class Engine : public QObject
{
    Q_OBJECT
public:
    explicit Engine(QObject *parent = 0);
    ~Engine();

signals:
    void error(VPrn::AppErrorType eCode, QString e_info);



public slots:
    void connectToServer(const QString &serverName);

private slots:
    void processingMessage(const QString &cuid,const Message &s_msg);
    void setConnState(bool m_state);
private:
    msgSocketTransmitter *m_socketRW;
    bool m_connState; // Соединение с локальным сервером

};

#endif // ENGINE_H
