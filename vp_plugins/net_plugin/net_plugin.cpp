#define DEBUG_MODE

#include <QDebug>

#ifdef DEBUG_MODE

#include <QtCore/QRegExp>
#include <stdlib.h>
#include <QtCore/QDateTime>
#include <QList>

#endif
#include <QtCore/QUuid>
#include "net_plugin.h"

net_plugin::net_plugin(QObject *parent)
        : QObject(parent)
        , HostName(QString())
        , Port (0)
        , client(0)
        , packetSize(-1)
        , e_info(QString())
{

}

//---------------------------- PUBLIC ------------------------------------------
void net_plugin::init(const QString &host, int port)
{
    this->HostName = host;
    this->Port = port;

    client = new QTcpSocket(this);
    connect(client, SIGNAL(readyRead()),
            this, SLOT(readyRead()));
    connect(client, SIGNAL(connected()),
            this, SLOT(onConnected()));
    connect(client, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(selectError(QAbstractSocket::SocketError)));

    client->connectToHost(HostName, Port);
#ifdef DEBUG_MODE
    Message message( this );
    message.setType(Ans_RegisterGlobal);
    QString str = QObject::trUtf8("[%1];:;")
                  .arg(QUuid::createUuid().toString()
                       .replace("{","")
                       .replace("}","")
                       );
    message.setMessageData(  str.toUtf8() );
    emit messageReady(message);
#endif

}

void net_plugin::sendMessage(const Message &s_msg)
{

#ifdef DEBUG_MODE
    // При отладке я просто эмулирую ответы Мишиного демона
    QString m_uuid;
    QString m_body;
    QString doc_list;
    QString str;
    // Генерируем случайное число  1 или 0 или 2

    qsrand(QDateTime::currentDateTime().toTime_t());
    int r = 0;//(int) (3.0*(qrand()/(RAND_MAX + 1.0)));
    Message loc_msg( this );
    if (s_msg.type() == VPrn::Que_PrintThisFile) {

            loc_msg.setType( VPrn::Ans_PrintThisFileSuccess );
            //str = QObject::trUtf8("[%1];:;").arg(m_uuid);
            //loc_msg.setMessageData(  str.toUtf8() );
        emit messageReady(loc_msg);
        return;
    }

    // Разберем тело ответа на части [кому возвращать данные];:;что_передали
    str.append(s_msg.messageData());

    QRegExp rx("\\[(.+)\\];:;(.+)");
    //rx.setMinimal(true);

    if(rx.indexIn(str) != -1){
        m_uuid  = rx.cap(1);
        m_body  = rx.cap(2);
        loc_msg.clear();
        str.clear();
        //формируем ответное сообщение на запрос вроде как пришедшее от Мишиного демона
        switch (s_msg.type()){
        case VPrn::Que_CheckFileSize:
            // Проверка свободного места на диске для приема файла
            loc_msg.setType( VPrn::Ans_CheckFileSizeSuccess );
            str = QObject::trUtf8("[%1];:;").arg(m_uuid);
            break;

        case VPrn::Que_RegisterDocInBase:
            loc_msg.setType( VPrn::Ans_RegisterDocInBase );
            str = QObject::trUtf8("[%1];:;Документ успешно зарегистрирован в БД учета!.").arg(m_uuid);
            break;
        case VPrn::Que_GET_MB_LISTS:
            loc_msg.setType( VPrn::Ans_MB_LIST );
            /**
              * @short  Демон вернул список, в формате
              * ДАТА;:;МБ;:;номер_экз.;:;Название_документа;:;число страниц;:;статус
              */
            // Формируем список документов
            doc_list.append(QObject::trUtf8("123456789;:;МБ-12/12;:;1;:;Приказ по кадрам;:;52;:;РАСПЕЧАТАН###"));
            doc_list.append(QObject::trUtf8("123456789;:;МБ-12/12;:;2;:;Приказ по кадрам;:;52;:;РАСПЕЧАТАН###"));
            doc_list.append(QObject::trUtf8("123456789;:;МБ-12/12;:;3;:;Приказ по кадрам;:;52;:;БРАК###"));
            doc_list.append(QObject::trUtf8("123456789;:;МБ-12/12;:;4;:;Приказ по кадрам;:;52;:;БРАК###"));
            doc_list.append(QObject::trUtf8("123456789;:;МБ-12/12;:;5;:;Приказ по кадрам;:;52;:;РАСПЕЧАТАН###"));
            doc_list.append(QObject::trUtf8("123456789;:;МБ-12/14;:;1;:;Приказ по кадрам 2;:;52;:;РАСПЕЧАТАН###"));
            doc_list.append(QObject::trUtf8("123456789;:;МБ-12/14;:;2;:;Приказ по кадрам 2;:;52;:;РАСПЕЧАТАН###"));
            doc_list.append(QObject::trUtf8("123456789;:;МБ-12/14;:;3;:;Приказ по кадрам 2;:;52;:;БРАК###"));
            doc_list.append(QObject::trUtf8("123456789;:;МБ-12/14;:;4;:;Приказ по кадрам 2;:;52;:;БРАК###"));
            doc_list.append(QObject::trUtf8("123456789;:;МБ-12/14;:;5;:;Приказ по кадрам 2;:;52;:;БРАК###"));
            str = QObject::trUtf8("[%1];:;%2").arg(m_uuid,doc_list);
            break;
        case VPrn::Que_IS_MB_EXIST:
            if (r == 0){
                loc_msg.setType( VPrn::Ans_MB_NOT_EXIST );
            }
            if (r == 1){
                loc_msg.setType( VPrn::Ans_MB_EXIST_AND_BRAK );
            }
            if (r == 2){
                loc_msg.setType( VPrn::Ans_MB_EXIST_AND_NOT_BRAK );
            }
            str = QObject::trUtf8("[%1];:;empty").arg(m_uuid);
            break;
        case VPrn::Que_AUTHOR_USER:
            if (r == 0){
                loc_msg.setType(VPrn::Ans_PRINT_ALLOWED);
            }
            if (r == 1){
                loc_msg.setType(VPrn::Ans_PRINT_DENIED);
            }
            if (r == 2){
                loc_msg.setType(VPrn::Ans_PRINTER_NOT_FOUND);
            }
            str = QObject::trUtf8("[%1];:;empty").arg(m_uuid);

            break;
        case VPrn::Que_MANDAT_LIST:
            if (r == 0){
                loc_msg.setType(VPrn::Ans_MANDAT_LIST);
                str = QObject::trUtf8("[%1];:;CC;:;C;:;NS;:;DSP").arg(m_uuid);

            }else{
                loc_msg.setType(VPrn::Ans_MANDAT_LIST_EMPTY);
                str = QObject::trUtf8("[%1];:;empty").arg(m_uuid);

            }
            break;
        case VPrn::Que_SEC_LEVEL:
            //m_body содержит мандат
            // Формируем ответное сообщение
            loc_msg.setType(VPrn::Ans_STAMP_LIST);
            str = QObject::trUtf8("[%1];:;Сов.Секретно;:;Секретно;:;Не Секретно;:;ДСП").arg(m_uuid);            
            break;
        case VPrn::Que_GET_PRINTER_LIST:
            //m_body содержит мандат
            // Формируем ответное сообщение
            loc_msg.clear();
            str.clear();
            if (r == 0){
                loc_msg.setType(VPrn::Ans_PRINTER_LIST);
                str = QObject::trUtf8("[%1];:;Тестовый;:;192.168.112.2;:;test_prn###").arg(m_uuid);
            }else{
                loc_msg.setType(VPrn::Ans_PRINTER_LIST_EMPTY);
                str = QObject::trUtf8("[%1];:;empty").arg(m_uuid);
            }
            break;
        default:
            break;
        }

        loc_msg.setMessageData(  str.toUtf8() );
        emit messageReady(loc_msg);
    }else{
        setError(QObject::trUtf8("Полученно сообщение неверного формата!"));
    }
#else
    //Сформируем пакет И пошлем его ветром гонимого
    client->write(m_msg.createPacket());
    client->flush();
#endif
}

//socketState net_plugin::state() const
//{
//    return m_state;
//}

//---------------------------- PRIVATE -----------------------------------------
void net_plugin::setError(const QString &info)
{
    e_info = info;
}

// void net_plugin::setState(socketState state)
// {
//     m_state = state;
//     emit stateChanged(m_state);
// }

//---------------------------- PRIVATE SLOTS -----------------------------------
void net_plugin::readyRead()
{
    //Свяжем поток и сокет
    QDataStream in ( client );
    in.setVersion(QDataStream::Qt_3_0);

    while (client->bytesAvailable() > 0){
        if (packetSize == -1) {
            //Определим количество байт доступных для чтения;
            //на этом шаге необходимо получить больше 4-х байт
            if( (qint32) client->bytesAvailable() < (qint32) sizeof(packetSize) ){
                return;
            }
            //Читаем размер пакета
            in >> packetSize;
            qDebug() << Q_FUNC_INFO << " packet size "  << packetSize << "\n";
        }
        //Проверим что в сокет пришел весь пакет а не его огрызки
        if (client->bytesAvailable() < packetSize){
            return;
        }
        //Сбросим размер пакета, для обработки следующего
        packetSize = -1;

        // Прочтем тип сообщения
        int m_Type;
        in >> m_Type;

        //Прочтем само сообщение
        QByteArray msg;
        in >> msg;
        Message message( this );
        message.setType((MessageType) m_Type); //Проверить как конвертирует
        message.setMessageData( msg );

        // Отправка сообщения
        emit messageReady(message);
    }
}

void net_plugin::onConnected()
{
    //QString m_body =QString("/me;:;%1;:;%2").arg().arg(REGISTER_CMD,0,10);

    Message message( this );
    message.setType(VPrn::Que_RegisterGlobal);
    /// @todo k МИШЕ Нужен ли этот SID ? для работы
    QString str = QObject::trUtf8("[%1];:;").arg(QUuid::createUuid().toString()
                                                 .replace("{","")
                                                 .replace("}","")
                                                 );
    message.setMessageData( str.toUtf8() ); // Пробразуем в QByteArray
    sendMessage(message);
}

void net_plugin::selectError(QAbstractSocket::SocketError err)
{
    QString e_msg;
    switch(err)
    {
    case QAbstractSocket::ConnectionRefusedError :
        e_msg =QObject::trUtf8("Соединение отклоненно удаленным сервером [%1]")
               .arg(HostName);
        break;
    case QAbstractSocket::HostNotFoundError :
        e_msg =QObject::trUtf8("Удаленный сервер не найден!")
               .arg(HostName);
        break;
    case QAbstractSocket::SocketTimeoutError :
        e_msg =QObject::trUtf8("Превышено время ожидания ответа от сервера")
               .arg(HostName);
        break;
    default:
        e_msg = QObject::trUtf8("Код ошибки %1").arg(err,0,10);
    }
    e_msg.append("\n").append(QString(Q_FUNC_INFO));
    emit error(VPrn::NetworkError,e_msg);
    //setState(VPrn::InternalError);
}

Q_EXPORT_PLUGIN2(Inet_plugin, net_plugin);

