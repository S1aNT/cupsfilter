#include "engine.h"

Engine::Engine(QObject *parent)
    : QObject(parent)
    , m_socketRW(0)
    , m_connState(false)
{
    // создаем читателя сокета
    m_socketRW = new msgSocketTransmitter();

    connect(m_socketRW, SIGNAL (error(VPrn::AppErrorType, QString )),
            this, SIGNAL (error(VPrn::AppErrorType,QString))
            );

    connect(m_socketRW, SIGNAL(receivedMessageFromSocket(QString,Message)),
            this,SLOT(processingMessage(QString,Message))
            );
    connect(m_socketRW, SIGNAL(connState(bool)),
            this, SLOT(setConnState(bool))
            );

}

void Engine::connectToServer(const QString & serverName)
{
    if (!serverName.isEmpty() && m_socketRW !=0){
        m_socketRW->connectToServer (serverName);
    }
}

Engine::~Engine()
{
    if (m_socketRW !=0){
        m_socketRW->deleteLater ();
    }
}

// ----------------------- PRIVATE Slots

void Engine::processingMessage(const QString &cuid,const Message &s_msg)
{

}

void Engine::setConnState(bool m_state)
{
    m_connState = m_state;
}
