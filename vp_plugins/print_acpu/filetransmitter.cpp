#include <QtCore/QDebug>

#include "filetransmitter.h"

#include "QFile"

fileTransmitter::fileTransmitter(QObject *parent)
    : QObject(parent)
    , m_fileName (QString())
{

}

fileTransmitter::~fileTransmitter()
{
}

void fileTransmitter::convertFileToMsg(const QString &unicFileId,const QString &fileName)
{
    if ( unicFileId.isEmpty ()){
        emit error(VPrn::InternalAppError,
                   QObject::trUtf8("Отсутствует индетификатор файла.\n%1")
                   .arg(QString(Q_FUNC_INFO))
                   );
        return;
    }

    if ( !QFile::exists(fileName) ){
        return;
    }

    QFile file_in(fileName);

    if (!file_in.open(QIODevice::ReadOnly) ){
        emit error(VPrn::FileIOError,
                   QObject::trUtf8("Не могу открыть запрошенный файл: %1.\n%2")
                   .arg(fileName)
                   .arg(QString(Q_FUNC_INFO))
                   );
        return ;
    }

    qint64 fSize64 = file_in.size();


    if (fSize64 > m_MaxInt32 - sizeof(qint32) ){
        emit error(VPrn::FileIOError,
                   QObject::trUtf8("Файл %1 имеет слишком большой размер: %2 байт.\n%3")
                   .arg(fileName)
                   .arg(fSize64,0,10)
                   .arg(QString(Q_FUNC_INFO))
                   );
        return ;
    }


    QByteArray data = qCompress( file_in.readAll() );
    file_in.close();

    if (data.isEmpty()){
        emit error(VPrn::FileIOError,
                   QObject::trUtf8("Файл %1 не содержит данных или ошибка чтения.\n%2")
                   .arg(fileName)
                   .arg(QString(Q_FUNC_INFO))
                   );
        return ;
    }

    QByteArray task_data;
    QDataStream out(&task_data, QIODevice::WriteOnly );
    out.setVersion(QDataStream::Qt_3_0);
    out << unicFileId;          // уникальный индетификатор отправляемого файла
    out << (qint32) fSize64;    // Размер не сжатого файла
    out << data;                // Файл для печати в формате QByteArray (сжатый)

    // Формируем сообщение Msg
    Message message;
    message.setType(VPrn::Que_ReciveFile);
    message.setMessageData (task_data);
    emit fileReadyToSend(message);
}
