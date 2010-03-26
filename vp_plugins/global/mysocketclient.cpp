#include "mysocketclient.h"
#include "message.h"


mySocketClient::mySocketClient(QLocalSocket *parent)
        : packetSize(-1)
        , m_chekpoints(VPrn::glob_Init)
        , e_info(QString())
{
    connect(this,SIGNAL(connected()),
            this,SLOT(do_connected())
            );
    connect(this,SIGNAL(readyRead()),
            this,SLOT(readyRead()));
    connect(this,SIGNAL(error(QLocalSocket::LocalSocketError)),
            this,SLOT(do_error(QLocalSocket::LocalSocketError))
            );
}


void mySocketClient::readyRead()
{
    qDebug() << Q_FUNC_INFO << QObject::trUtf8(" Recive %1 bytes \n").arg(this->bytesAvailable(),0,10);
    //Свяжем поток и сокет
    QDataStream in ( this );
    in.setVersion(QDataStream::Qt_4_0);

    while (this->bytesAvailable() > 0){
        if (packetSize == -1) {
            //Определим количество байт доступных для чтения;
            //на этом шаге необходимо получить больше 4-х байт
            if( this->bytesAvailable() < sizeof(qint32) ){
                return;
            }
            //Читаем размер пакета
            in >> packetSize;
        }
        //Проверим что в сокет пришел весь пакет а не его огрызки
        if (this->bytesAvailable() < packetSize){
            return;
        }
        //Сбросим размер пакета, для обработки следующего
        packetSize = -1;

//        //Прочтем и проверим формат протокола
//        qint32 m_format;
//        in >> m_format;
//        if( m_format != format ) {
//            setError(QObject::trUtf8("Ошибка в формате протокола, при обмене данными с сервером %1")
//                     .arg(this->serverName()));
//            return;
//        }

        // Прочтем тип сообщения
        int m_Type;
        in >> m_Type;

        //Прочтем само сообщение
        QByteArray msg;
        in >> msg;
        Message message( this );
        message.setType((MessageType) m_Type); //Проверить как конвертирует
        message.setMessage( msg );        
        // Отправка сообщения
        qDebug() << "Construct message. Type: " << m_Type << " Body " << msg;
        messageReady( message );
    }
}

void mySocketClient::do_connected()
{
    setCheckPoint(VPrn::loc_Connected);   
}

void mySocketClient::setCheckPoint(MyCheckPoints m_cpoint)
{
    m_chekpoints = m_cpoint;
    emit checkPointChanged(m_chekpoints);
}

void mySocketClient::setError(const QString &info)
{
    e_info = info;
    setCheckPoint(VPrn::glob_Error);
}

void mySocketClient::sendMessage(const Message &msg)
{
    if (this->state() == QLocalSocket::ConnectedState){
        this->write(msg.createPacket());
        this->flush();
    }else{
        setError(QObject::trUtf8("Попытка записи данных в закрытый сокет!"));
    }
}

void mySocketClient:: do_error(QLocalSocket::LocalSocketError r_error)
{
    switch(r_error){
    case QLocalSocket::ServerNotFoundError:
        setCheckPoint(VPrn::loc_ServerNotFound);
        break;
    default:
        setError(QObject::trUtf8("При работе с % произошла ошибка %2")
                 .arg(this->serverName(),this->errorString()));
        break;
    }
}
