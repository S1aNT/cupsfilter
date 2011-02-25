#ifndef FILETRANSMITTER_H
#define FILETRANSMITTER_H

#include <QObject>
#include "message.h"
#include "mytypes.h"

using namespace VPrn;

class fileTransmitter : public QObject
{
    Q_OBJECT

public:
    explicit fileTransmitter(QObject *parent = 0);
    ~fileTransmitter();

    void convertFileToMsg(const QString &unicFileId,const QString &fileName);

signals:
    void fileReadyToSend( const Message &msg );
    void error(VPrn::AppErrorType eCode, QString e_info);

private:
    QString m_fileName;

    /// @todo  Хардкодим по маленьку
    static const qint32 m_MaxInt32= 2147483647;
};

#endif // FILETRANSMITTER_H
