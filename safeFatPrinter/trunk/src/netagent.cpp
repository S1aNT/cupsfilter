#include "netagent.h"

netAgent::netAgent(QObject *parent )
{
    socket = new QTcpSocket(this);

    // This is how we tell Qt to call our readyRead() and connected()
    // functions when the socket has text ready to be read, and is done
    // connecting to the server (respectively):
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket,
		    SIGNAL(error(int, const QString &)),
		this,
		    SLOT(prepareError(int, QString &))
		    );
}
void netAgent::on_login(QString & hostname,int Port)
{
    if (hostname.isEmpty()){
	emit error_signal(netAgent::hostnameEmpty,QObject::trUtf8("Ошибка в файле конфигурации. Имя сервера не может быть пустым."));
	qDebug() <<Q_FUNC_INFO<<netAgent::hostnameEmpty << QObject::trUtf8("Ошибка в файле конфигурации. Имя сервера не может быть пустым.");
    }else {
	if (Port<1024 or Port >65536){
	    emit error_signal (netAgent::invalidRangePort,QObject::trUtf8("Ошибка в файле конфигурации. Порт %1 вне допустимого диапазона.").arg(Port));
	    qDebug() <<Q_FUNC_INFO<<netAgent::invalidRangePort <<QObject::trUtf8("Ошибка в файле конфигурации. Порт %1 вне допустимого диапазона.").arg(Port);
	}else{
		socket->connectToHost(hostname, Port);
	}
    }
}

void netAgent::prepareError(int socketError, const QString &message)
 {
     switch (socketError) {
     case QAbstractSocket::HostNotFoundError:
	 emit error_signal(netAgent::HostNotFoundError,QObject::trUtf8("Заданный сервер не найден или не доступен"));
	 qDebug() <<Q_FUNC_INFO<<netAgent::HostNotFoundError<< QObject::trUtf8("Заданный сервер не найден или не доступен");
	 break;
     case QAbstractSocket::ConnectionRefusedError:
	 emit error_signal(netAgent::ConnectionRefusedError,QObject::trUtf8("Соединение было закрыто удаленным сервером"));
	 qDebug() <<Q_FUNC_INFO<<netAgent::ConnectionRefusedError,QObject::trUtf8("Соединение было закрыто удаленным сервером");
	 break;
     }
 }

void netAgent::send2Server(const QString &cmd,const QString &body)
{
    // формируем тело команды
    QString message;
    message =QString("/cmd:%1:%2").arg(cmd,body);
    socket->write(QString(message + "\n").toUtf8());
}

void netAgent::readyRead()
{
    // We'll loop over every (complete) line of text that the server has sent us:
    while(socket->canReadLine())
    {
	// Here's the line the of text the server sent use UTF-8
	QString line = QString::fromUtf8(socket->readLine()).trimmed();

    }
}

void netAgent::setSid(const QString & sid)
{
    client_sid=sid;
}
void netAgent::connected()
{
    if (!client_sid.isEmpty()){
	// And send our client sid to the server.
	socket->write(QString("/me:" +client_sid + "\n").toUtf8());
    }
}
