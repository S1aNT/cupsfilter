#include "boxserver.h"
#include <QTcpSocket>
#include <QRegExp>
#include <QDateTime>

BoxServer::BoxServer(QObject *parent) : QTcpServer(parent)
{
}

void BoxServer::incomingConnection(int socketfd)
{
    QTcpSocket *client = new QTcpSocket(this);
    client->setSocketDescriptor(socketfd);
    clients.insert(client);

    qDebug() << "New client from:" << client->peerAddress().toString();

    connect(client, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

void BoxServer::doCommand(const QString &user,int command,const QString &body)
{
    QTcpSocket *client;
    QMapIterator<QTcpSocket*,QString> i(clientsUID);
    while (i.hasNext()) {
	i.next();

	if (i.value()==user){
	    client = i.key();
	    if (client->isValid()){
		qDebug() <<client->isValid()<<command<<body;
		QRegExp rx;
		QString message;
		switch (command) {

		    case GET_SEC_LEVEL_CMD:
			     qsrand(QDateTime::currentDateTime().toTime_t());
			     int i=qrand() % 3; 
			    switch (i){
			      case 0: 
				message=QString(QObject::trUtf8("/%1;:;Несекретно;:;Секретно;:;Совершенно секретно\n")).arg(STAMP_LIST_ANS,0,10);
				break;
			      case 1: 
				message=QString(QObject::trUtf8("/%1;:;Секретно;:;Совершенно секретно\n")).arg(STAMP_LIST_ANS,0,10);
				break;
			      case 2: 
				message=QString(QObject::trUtf8("/%1;:;Несекретно\n")).arg(STAMP_LIST_ANS,0,10);
				break;
			    }
                            
                            client->write(message.toUtf8());
                            qDebug() << message;
                         break;
		    case IS_MB_EXIST_CMD:
			 // разбор тела команды. Формат номер МБ~begin_date~end_date
			 rx.setPattern("^(.+);:;(.+);:;(.+)$");
			 rx.setMinimal(true);
			 if (rx.indexIn(body)!=-1){
			     QString mb_value =rx.cap(1);
			     QString begin_dt =rx.cap(2);
			     QString end_dt =rx.cap(3);
			     //qDebug()<<qrand() % 3;

			     qsrand(QDateTime::currentDateTime().toTime_t());
			     int ans =qrand() % 3; 
			     qDebug() <<"ans" <<ans;
		             switch (ans) {
            		      case 0: message=QString(QObject::trUtf8("/%1;:;%2\n")).arg(MB_NOT_EXIST_ANS,0,10).arg(mb_value);
            		        qDebug() << "0";
            		        break;
            		      case 1: message=QString(QObject::trUtf8("/%1;:;Статус документа=Отпечатан;:;Кол-во листов=%2;:;Название док-та=Длинное имя_%2;:;Гриф=1;:;Пункт перечня=Пункт %2;:;Номер док-та=МБ №12/%2;:;Инв. №=123/%2;:;Номер копии=%2;:;Получатель №1=получатель 1_%2;:;Получатель №2=получатель 2_%2;:;Получатель №3=test 3_%2;:;Получатель №4=получатель 4_%2;:;Получатель №5=получатель 5_%2;:;Исполнитель=Исполнитель_%2 документа;:;Отпечатал=Отпечатал_%2;:;Телефон=127-%2;:;Дата распечатки=%2.09.09;:;Список рассылки=true;:;Штамп последней стр.=true\n")).arg(MB_EXIST_AND_BRAK_ANS,0,10);
            		        qDebug() << "1";
				break;
			      case 2: message=QString(QObject::trUtf8("/%1;:;%2\n")).arg(MB_EXIST_AND_NOT_BRAK_ANS,0,10).arg(mb_value);
                                qDebug() << "2";
			        break;
			     }
                             //
			     qDebug() << "Send to client "<< message <<"\n";
			     client->write(message.toUtf8());
			 }
			break;
		    case GET_MB_PRINTS_TODAY_CMD:
			    // разбор тела команды. Формат номер МБ;:;принтер
			    rx.setPattern("^(.+);:;(.+)$");
			    if (rx.indexIn(body)!=-1){
				QString mb_value =rx.cap(1);
				QString printer_value = rx.cap(2);
				qDebug() << "MB: "<< mb_value <<" Printer: " <<printer_value <<"\n";

				for (int i=0; i<10;++i){
				    message=QString(QObject::trUtf8("/%1;:;Статус документа=Отпечатан;:;Кол-во листов=%2;:;Название док-та=Длинное имя_%2;:;Гриф=1;:;Пункт перечня=Пункт %2;:;Номер док-та=МБ №12/%2;:;Инв. №=123/%2;:;Номер копии=%2;:;Получатель №1=получатель 1_%2;:;Получатель №2=получатель 2_%2;:;Получатель №3=test 3_%2;:;Получатель №4=получатель 4_%2;:;Получатель №5=получатель 5_%2;:;Исполнитель=Исполнитель_%2 документа;:;Отпечатал=Отпечатал_%2;:;Телефон=127-%2;:;Дата распечатки=%2.09.09;:;Список рассылки=true;:;Штамп последней стр.=true\n")).arg(MB_LIST_ANS,0,10).arg(i);
				    client->write(message.toUtf8());
				    qDebug() << "Send to client "<< message <<"\n";
				}
			    }
			break;
		    case GET_PRINTER_LIST_CMD:
			    // разбор тела команды. Формат имя_пользователя;:;мандат
			    rx.setPattern("^(.+);:;(.+)$");
			    if (rx.indexIn(body)!=-1){
				QString user =rx.cap(1);
				QString mandat = rx.cap(2);
				qDebug() << "User: "<< user <<" Mandat: " <<mandat <<"\n";

			     qsrand(QDateTime::currentDateTime().toTime_t());
			     int s =qrand() % 2; 
		             switch (s) {
            		      case 0: 
				message=QString("/%1;:;Testprinter1,TestPrinter2,TestPrinter3,TestPrinter4\n").arg(PRINTER_LIST_ANS,0,10);
            		        break;
            		      case 1: 
                                message=QString("/%1;:;NOT_FOUND_PRINTER\n").arg(PRINTER_LIST_EMPTY,0,10);
				break;
			      }	

				 
				client->write(message.toUtf8());
				qDebug() << "Send to client "<< message <<"\n";

			    }else{
				qDebug() <<"Error in parse message \n";
			     }
		    break;
		}
	    }else {
		qDebug() << "Not valid client with ID " << user << " and adress :" <<client->peerAddress().toString();
	    }
	}
    }
}
void BoxServer::readyRead()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    while(client->canReadLine())
    {
	QString line = QString::fromUtf8(client->readLine()).trimmed();

	qDebug() << "Read line:" << line;
	// Формат строки /me:{UID}
	QRegExp meRegex("^/me;:;(.+);:;(.+)$");

	if(meRegex.indexIn(line) != -1)
	{
	    QString cUID = meRegex.cap(2);
	    qDebug() << QString("Connected new client with UID: %1 ").arg(cUID) << "\n";

	    clientsUID[client] = cUID;

	    QString ans =QString("/%1;:;Client %2 register.\n").arg(REGISTER_ANS,0,10).arg(cUID);
	    qDebug()<< "Write to socket: " << ans << "\n";
	    client->write(ans.toUtf8());
	    /*
	    foreach(QTcpSocket *client, clients)
		client->write(QString("Server:" + user + " has joined.\n").toUtf8());
	    sendUserList();
	    */
	}else {
	    if(clientsUID.contains(client))
	    {
		QString message = line;
		QString user = clientsUID[client];
		qDebug() << "User:" << user<< "\nMessage:" << message;
// format is: [/me|/cmd|/sql];:;command_type;:;uid;:;command_body
                QStringList list = line.split(";:;");
                if (list.at(0)=="/cmd"){
                   QString cmd=list.at(1);
                   QString body;
                   for (int i=3;i<list.size();i++){
                     body.append(list.at(i));
		     if ((i+1)!=list.size()){
			body.append(";:;");
		     }
                   }

                   doCommand(user,cmd.toInt(),body);
                }
/*
	        QRegExp rx("^/(cmd|sql);:;(\\d+);:;(.+);:;(.+)$");  
	        rx.setMinimal(true);
	        
		if(rx.indexIn(line) != -1)
		{
		    QString cmd =rx.cap(2);
		    QString body = rx.cap(4);
		    qDebug() << rx.cap(1) << rx.cap(2) << rx.cap(3)<< rx.cap(4);
		    qDebug() <<QString("Recived command %1 with args %2 from client: ").arg(cmd,body) <<client->peerAddress().toString() ;
		    doCommand(user,cmd.toInt(),body);
		}
*/		 
	    }else
		{
		qWarning() << "Got bad message from client:" << client->peerAddress().toString() << line;
	    }
	}
    }
}

void BoxServer::disconnected()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    qDebug() << "Client disconnected:" << client->peerAddress().toString();
    clients.remove(client);
    clientsUID.remove(client);
}
