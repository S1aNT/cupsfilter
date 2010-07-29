#include "datamodule.h"

#include <QDebug>

#include <QtGui/QStandardItemModel>
#include <QtGui/QStringListModel>
#include <QtCore/QFile>

DataModule::DataModule(QObject *parent)
    : QObject(parent)

    , m_Mandats_model(0)
    , m_Stamps_model(0)
    , m_DocCard_model(0)
    , m_Tempalates_model(0)
    , m_Printers_model(0)
    , m_Work_file(QString())
{
    m_Mandats_model = new QStringListModel(this);
    m_Stamps_model      = new QStringListModel(this);

    m_DocCard_model     = new QStandardItemModel(this);
    m_Tempalates_model  = new QStandardItemModel(this);
    m_Printers_model    = new QStandardItemModel(this);
}

bool DataModule::setWorkFile(const QString & i_file)
{
    if (QFile::exists(i_file)){
        m_Work_file = i_file;
    }else{
        emit error("fileNotFound",
                   QObject::trUtf8("Файл %1 заданный для печати не существует!\n%2")
                   .arg(i_file).arg(QString(Q_FUNC_INFO))
                   );
        return false;
    }
    return true;
}

