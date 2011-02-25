#include <qapplication.h>
#include <qglobal.h>
#include <qstring.h>
#include <qdatastream.h>
#include <qsocket.h>
#include <qfile.h>
#include <qwaitcondition.h>

class ClientSocket : public QSocket
{
    Q_OBJECT
    
public:
    ClientSocket( const QString &host, Q_UINT16 port )
    {	
	socket = new QSocket(this);
	connect( socket, SIGNAL(connected()), SLOT(socketConnected()) );
	connect( socket, SIGNAL(readyRead()), SLOT(readSocketData()) );
	connect( socket, SIGNAL(connectionClosed()), SLOT(socketConnectionClosed()) );

	// connect to the server
        socket->connectToHost( host, port );
    }

    ~ClientSocket()
    {
    }
    
private slots:

    void socketConnected()
    {
        qDebug("Connected");
	Q_INT32 packetSize = 0, commandType = 10;
	QString s = "";
	QDataStream out(socket);	
	QFile psFile("./test.txt");
	QByteArray ba, cba;

	switch (commandType) {
	
	    case 10:
		s = "[123];:;";
		break;
	    case 100:
		s = "[123];:;qqqq;:;sva;:;NC";
		break;
	    case 101:
		s = "[123];:;NC;:;";
		break;
	    case 202:
		s = "[123];:;INSERT INTO SAMP (id) VALUES (300); SELECT * FROM SAMP;;:;";
		break;
	    case 300:
		s = "[123];:;NC;:;";
		break;
	    case 500:
		break;
	    case 600:
		s = "[123];:;usr1;:;";	    
		break;
	    case 6000:
		psFile.open(IO_ReadOnly);
		ba =  psFile.readAll();
		psFile.close();
	//	qDebug("Uncompressed size is " + QString("%1").arg(ba.size()) + " bytes");
		cba = qCompress(ba);
		break;
	    default:
		qDebug("Unknown command");
		return;
	}
	
	if (commandType == 6000) 
	    cba = qCompress(ba);
	else
	    cba = s.utf8();
	
	packetSize = sizeof(packetSize) + sizeof(commandType) + cba.size();
	qDebug("Writing " + QString("%1").arg(cba.size()) + " bytes of data (" + QString("%1").arg(packetSize) + " bytes total packet size)" );
	out << packetSize;
	out << commandType;
	if (s != "")
	    qDebug("Sending string: " + s);
	out << cba;

    socket->flush();

//    QEventLoop loop; QTimer::singleShot(10000, &loop, SLOT(quit())); loop.exec();
    QWaitCondition sleep;
    sleep.wait(1000);   // wait here

    commandType = 600;
    s = "[123];:;usr1;:;";
    cba = s.utf8();
    packetSize = sizeof(packetSize) + sizeof(commandType) + cba.size();
    qDebug("Writing " + QString("%1").arg(cba.size()) + " bytes of data (" + QString("%1").arg(packetSize) + " bytes total packet size)" );
    out << packetSize;
    out << commandType;
    qDebug("Sending string: " + s);
    out << cba;
    socket->flush();
    sleep.wait(7000);   // wait here

    commandType = 300;
    s = "[123];:;NC;:;";
    cba = s.utf8();
    packetSize = sizeof(packetSize) + sizeof(commandType) + cba.size();
    qDebug("Writing " + QString("%1").arg(cba.size()) + " bytes of data (" + QString("%1").arg(packetSize) + " bytes total packet size)" );
    out << packetSize;
    out << commandType;
    qDebug("Sending string: " + s);
    out << cba;
    socket->flush();
    sleep.wait(7000);   // wait here

    commandType = 400;
    s = "[123];:;usr1;:;CC;:;";
    cba = s.utf8();
    packetSize = sizeof(packetSize) + sizeof(commandType) + cba.size();
    qDebug("Writing " + QString("%1").arg(cba.size()) + " bytes of data (" + QString("%1").arg(packetSize) + " bytes total packet size)" );
    out << packetSize;
    out << commandType;
    qDebug("Sending string: " + s);
    out << cba;
    socket->flush();
    sleep.wait(14000);   // wait here

    commandType = 100;
    s = "[123];:;SL9PRT.PDF;:;CC;:;usr1;:;";
    cba = s.utf8();
    packetSize = sizeof(packetSize) + sizeof(commandType) + cba.size();
    qDebug("Writing " + QString("%1").arg(cba.size()) + " bytes of data (" + QString("%1").arg(packetSize) + " bytes total packet size)" );
    out << packetSize;
    out << commandType;
    qDebug("Sending string: " + s);
    out << cba;
    socket->flush();
    sleep.wait(4000);   // wait here

    socket->close();
    return;
//	qApp->quit();
    }

void readSocketData()
{
    QDataStream socketStream(this);
    Q_INT32 size, type;
    QByteArray ba;
    QString s;
    socketStream >> size;
    socketStream >> type;
    socketStream >> ba;
    s = ba.data();
    qDebug(s);
    return;
}
    
void socketConnectionClosed()
{
    qDebug("Disconnected");
    qApp->quit();
}
    
private:

    QSocket *socket;
    
};




int main(int argc, char *argv[])
{
    bool useGUI = false;

    QApplication qApp(argc, argv, useGUI);

    qDebug ("\n");

    qDebug("Connecting to localhost:4243");
    ClientSocket sock("localhost", 4243);
        
    return qApp.exec();
}

#include "main.moc"


