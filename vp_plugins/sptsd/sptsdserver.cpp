#include "sptsdserver.h"

sptsdServer::sptsdServer(Q_UINT16 port, int conn,  QObject* parent )
		:QServerSocket(port, conn, parent) { }

sptsdServer::~sptsdServer() { }

void sptsdServer::newConnection( int socket )
{
        sptsd *daemon = new sptsd(socket, this);
	peerIP = daemon->peerAddress().toString();
        qDebug("\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
	qDebug("New connection from [" + peerIP + "]");
//	qDebug("=========================================================================================");
        connect(daemon, SIGNAL(connectionClosed()), this, SLOT(clientDisconnected()));
}

void writeSettings()
{
    QSettings config;
    config.insertSearchPath(QSettings::Unix, "/usr/local/src/daemon-2010-01-22/settings");
    QString file = "sptsd";
    config.writeEntry(file + "/pgsql/pgsql_host", "localhost");
    config.writeEntry(file + "/pgsql/pgsql_port", "5432");
    config.writeEntry(file + "/pgsql/pgsql_database", "cupslog");
    config.writeEntry(file + "/pgsql/pgsql_user", "postgres");
    config.writeEntry(file + "/pgsql/pgsql_password", "postgres");
    config.writeEntry(file + "/sptsd/sptsd_log", "/var/log/cups/sptsd_log");
    config.writeEntry(file + "/sptsd/sptsd_port", "4242");
    config.writeEntry(file + "/seclevels/nc", "Несекретно");
    config.writeEntry(file + "/seclevels/dcp", "Для служебного пользования");
    config.writeEntry(file + "/seclevels/c", "Секретно");
    config.writeEntry(file + "/seclevels/cc", "Совершенно секретно");
    config.writeEntry(file + "/seclevels/num1", "1");
    config.writeEntry(file + "/seclevels/num2", "5");
    config.writeEntry(file + "/seclevels/num3", "10");
    config.writeEntry(file + "/seclevels/num4", "20");
    config.writeEntry(file + "/surd/surd_host", "127.0.0.1");
    config.writeEntry(file + "/surd/surd_port", "80");
}

void sptsdServer::clientDisconnected()
{
	qDebug("Peer [" + peerIP + "] closed connection");
	qDebug ("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
}


										
