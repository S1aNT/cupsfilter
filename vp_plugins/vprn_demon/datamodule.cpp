#include <QtCore/QDebug>
#include <QtCore/QDir>

#include "datamodule.h"

DataModule::DataModule(QObject *parent)
        : QObject(parent)
        , db_state(false)
        , spool_dir(QString())
{

}

void DataModule::setSpoolDir(const QString &spool)
{
    QDir dir;
    if (!spool.isEmpty() && dir.cd(spool) ){
        spool_dir = spool;
    }else{
        emit error(VPrn::InternalAppError,
                   QObject::trUtf8(" аталог %1 не существует или нет прав доступа\n%2")
                   .arg(spool)
                   .arg(QString(Q_FUNC_INFO))
                   );

    }
}
//------------------------------- PRIVATE --------------------------------------

void DataModule::checkSqliteReady()
{

}
