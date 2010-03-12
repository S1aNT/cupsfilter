#include "servergears.h"
#include <QByteArray>
#include <QDataStream>

serverGears::serverGears(QObject *parent,const QString &srvName)
    : QObject(parent)
    , connState(false)
    , currentDataBlock(0)
    , blockSize(0)

{
    m_serverName = srvName;
    socket = new QLocalSocket(this);
    connect(socket, SIGNAL(readyRead()),
            this,   SLOT(readData()));
    connect(socket, SIGNAL(error(QLocalSocket::LocalSocketError)),
            this,   SLOT  (prepareError(QLocalSocket::LocalSocketError)));
    socket->connectToServer(m_serverName);
    connState = socket->waitForConnected(1000);
}

//------------------------- PRIVATE SLOTS --------------------------------------
void serverGears::readData()
{
    QByteArray block;
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_0);

    if (blockSize == 0) {
        if (socket->bytesAvailable() < (int)sizeof(quint16))
            return;
        in >> blockSize;
    }
    if (in.atEnd())
        return;
    in >> block;
    // Анализ ответа полученного от сервера
}

void serverGears::prepareError(QLocalSocket::LocalSocketError socketError)
{
/*
    switch (socketError) {
    case QLocalSocket::ServerNotFoundError:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QLocalSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the fortune server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    case QLocalSocket::PeerClosedError:
        break;
    default:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The following error occurred: %1.")
                                 .arg(socket->errorString()));
    }

*/
}

//-------------------------- PRIVATE -------------------------------------------
void serverGears::sendCommand(const QString &cmd)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)0;

    out << cmd;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    socket->write(block);
    socket->flush();
}

/*
void Server::sendFortune()
{

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)0;
    out << fortunes.at(qrand() % fortunes.size());
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    QLocalSocket *clientConnection = server->nextPendingConnection();
    connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));

    clientConnection->write(block);
    clientConnection->flush();
    clientConnection->disconnectFromServer();

}

    server = new QLocalServer(this);
    if (!server->listen("fortune")) {
        QMessageBox::critical(this, tr("Fortune Server"),
                              tr("Unable to start the server: %1.")
                              .arg(server->errorString()));
        close();
        return;
    }


    fortunes << tr("You've been leading a dog's life. Stay off the furniture.")
             << tr("You've got to think about tomorrow.")
             << tr("You will be surprised by a loud noise.")
             << tr("You will feel hungry again in another hour.")
             << tr("You might have mail.")
             << tr("You cannot kill time without injuring eternity.")
             << tr("Computers are not intelligent. They only think they are.");

    //connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(server, SIGNAL(newConnection()), this, SLOT(sendFortune()));

*/
           mainGearmainGear