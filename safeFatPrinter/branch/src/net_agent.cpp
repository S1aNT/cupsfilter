#include "net_agent.h"

// format is: [/me|/cmd|/sql];:;command_type;:;uid;:;command
net_agent::net_agent(QObject *parent)
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
}

 void net_agent::do_connect(QString &host,int port,QString &sid)
 {
     qDebug() << "connecting to server " <<host <<port << sid;
     client_sid=sid;
     if (host.isEmpty()){
	 emit error_signal(HOSTNAME_EMPTY_ERR,QObject::trUtf8("Ошибка в файле конфигурации. Имя сервера не может быть пустым."));
     }else  if (port<1024 or port >65536){
	 emit error_signal (INVALID_PORT_RANGE_ERR,QObject::trUtf8("Ошибка в файле конфигурации. Порт %1 вне допустимого диапазона.").arg(port));
     }else{
	 socket->connectToHost(host, port);
	 if( !socket->waitForConnected(5000) ){
	     emit error_signal(socket->error(), socket->errorString());
	     qDebug() << QObject::trUtf8("failed to connect %1 port %2").arg(host,port);
	     return;
	 }

     }
 }

 void net_agent::send2Server(commands_t cmd,QString &body)
 {
     qDebug() << Q_FUNC_INFO << socket->state();
     if (socket->state()==QAbstractSocket::ConnectedState){

	 QString message;
	 message =QString("/cmd;:;%1;:;%2;:;%3").arg(cmd,0,10).arg(client_sid).arg(body);
	qDebug()  << Q_FUNC_INFO << "Write to socket " << message;
	 socket->write(QString(message + "\n").toUtf8());
     }
 }
 void net_agent::execQuery(QString &query)
 {
     /*
     select documents.Stamp , documents.mb , documents.punkt ,documents.inv_number
		documents.number_ex, documents.doc_des, documents.print_last_page_stamp,
	     documents.print_overside_stamp,   users.FIO as executor_fio,users.phone, users.FIO pressman_id integer
	 */
 }

 void net_agent::connected()
 {
      QString message =QString("/me;:;%1;:;%2").arg(REGISTER_CMD,0,10).arg(client_sid);
      qDebug()  << Q_FUNC_INFO << "write to socket" << message;
      socket->write(QString(message + "\n").toUtf8());
 }

 void net_agent::readyRead()
 {

     while(socket->canReadLine()){
	 // Here's the line the of text the server sent use UTF-8
	 QString line = QString::fromUtf8(socket->readLine()).trimmed();
	 qDebug()  << Q_FUNC_INFO <<"Read from socket" <<line;
	 if (!line.isEmpty()){
	     emit serverSay(line);
	 }
     }
 }

