#include "netagent.h"

netAgent::netAgent(QObject *parent )
{
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
	    this,   SLOT(prepareError(QAbstractSocket::SocketError)));

}
void netAgent::on_login(QString & hostname,int Port,QString &sid)
{
     client_sid=sid;
    if (hostname.isEmpty()){
	emit error_signal(HOSTNAME_EMPTY_ERR,QObject::trUtf8("Ошибка в файле конфигурации. Имя сервера не может быть пустым."));
	qDebug() <<Q_FUNC_INFO<<  HOSTNAME_EMPTY_ERR << QObject::trUtf8("Ошибка в файле конфигурации. Имя сервера не может быть пустым.");
    }else {
	if (Port<1024 or Port >65536){
	    emit error_signal (INVALID_PORT_RANGE_ERR,QObject::trUtf8("Ошибка в файле конфигурации. Порт %1 вне допустимого диапазона.").arg(Port));
	    qDebug() <<Q_FUNC_INFO<<INVALID_PORT_RANGE_ERR <<QObject::trUtf8("Ошибка в файле конфигурации. Порт %1 вне допустимого диапазона.").arg(Port);
	}else{
		qDebug() <<Q_FUNC_INFO<<QString("Connect to %1:%2").arg(hostname).arg(Port,0,10);
		socket->connectToHost(hostname, Port);
	}
    }
}

void netAgent::prepareError(QAbstractSocket::SocketError socketError)
 {
     switch (socketError) {
     case QAbstractSocket::HostNotFoundError:
	 emit error_signal(HOST_NOT_FOUND_ERR,QObject::trUtf8("Заданный сервер не найден или не доступен"));
	 qDebug() <<Q_FUNC_INFO<<HOST_NOT_FOUND_ERR<< QObject::trUtf8("Заданный сервер не найден или не доступен");
	 break;
     case QAbstractSocket::ConnectionRefusedError:
	 emit error_signal(CONNECTION_REFUSED_ERR,QObject::trUtf8("Соединение было закрыто удаленным сервером"));
	 qDebug() <<Q_FUNC_INFO<<CONNECTION_REFUSED_ERR,QObject::trUtf8("Соединение было закрыто удаленным сервером");
	 break;
     }
 }

void netAgent::send2Server(commands_t cmd,QString &body)
{
    /*
    if (socket->state()!=QAbstractSocket::ConnectedState){
	QTimer::singleShot();
    }
    */
    if (socket->state()==QAbstractSocket::ConnectedState){
	// формируем тело команды
	QString message;
	message =QString("/cmd:%1:%2").arg(cmd,0,10).arg(body);
	qDebug() <<Q_FUNC_INFO<<" Write to socket " << message;
	socket->write(QString(message + "\n").toUtf8());
    }
}

void netAgent::readyRead()
{
    qDebug()  <<Q_FUNC_INFO<< "ReadyRead signal";
    // We'll loop over every (complete) line of text that the server has sent us:
    while(socket->canReadLine())
    {
	// Here's the line the of text the server sent use UTF-8
	QString line = QString::fromUtf8(socket->readLine()).trimmed();
	if (!line.isEmpty()){
	    qDebug() <<Q_FUNC_INFO<< line;
	    emit takeServerResponce(line);
	}
    }
}

/*
void netAgent::setSid(const QString & sid)
{
    client_sid=sid;
    //client_sid.replace("{","").replace("}","");
    qDebug() << client_sid;
}
*/
void netAgent::connected()
{
    if (!client_sid.isEmpty()){
	// And send our client sid to the server.
	qDebug() << "Connect send handshake" <<QString("/me:" +client_sid + "\n").toUtf8();
	QString message =QString("/cmd:%1:%2").arg(REGISTER_CMD,0,10).arg(client_sid);
	socket->write(QString(message + "\n").toUtf8());
    }
}
