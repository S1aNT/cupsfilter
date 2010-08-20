#include "datamodule.h"

#include <QDebug>

#include <QtGui/QStandardItemModel>
#include <QtGui/QStringListModel>
#include <QtGui/QStandardItem>
#include <QtCore/QFile>

#include "mytypes.h"
using namespace VPrn;

DataModule::DataModule(QObject *parent)
    : QObject(parent)

    , m_Mandats_model(0)
    , m_Stamps_model(0)
    , m_DocCard_model(0)
    , m_Tempalates_model(0)
    , m_Printers_model(0)
    , m_Work_file(QString())
{
    m_Mandats_model     = new QStringListModel(this);
    m_Stamps_model      = new QStringListModel(this);

    m_DocCard_model     = new QStandardItemModel(this);
    m_Tempalates_model  = new QStandardItemModel(this);
    m_Printers_model    = new QStandardItemModel(this);

    // Настройка моделей
    m_Printers_model->setHorizontalHeaderItem(0,
                                              new QStandardItem(QObject::trUtf8("Принтер"))
                                              );
    m_Printers_model->setHorizontalHeaderItem(1,
                                              new QStandardItem(QObject::trUtf8("IP"))
                                              );
    m_Printers_model->setHorizontalHeaderItem(2,
                                              new QStandardItem(QObject::trUtf8("Очередь"))
                                              );
    m_Printers_model->setHorizontalHeaderItem(3,
                                              new QStandardItem(QObject::trUtf8("Выбран"))
                                              );

    m_DocCard_model->setHorizontalHeaderItem( VPrn::cards_ID,
                                               new QStandardItem(QObject::trUtf8("ID")));
    m_DocCard_model->setHorizontalHeaderItem( VPrn::cards_DOC_NAME,
                                               new QStandardItem(QObject::trUtf8("Имя документа")));
    m_DocCard_model->setHorizontalHeaderItem( VPrn::cards_STAMP,
                                               new QStandardItem(QObject::trUtf8("Гриф")));
    m_DocCard_model->setHorizontalHeaderItem( VPrn::cards_MB_NUMBER,
                                               new QStandardItem(QObject::trUtf8("Номер МБ")));
    m_DocCard_model->setHorizontalHeaderItem( VPrn::cards_PUNKT,
                                               new QStandardItem(QObject::trUtf8("Пункт")));
    m_DocCard_model->setHorizontalHeaderItem( VPrn::cards_PAGE_COUNT,
                                               new QStandardItem(QObject::trUtf8("Число страниц")));
    m_DocCard_model->setHorizontalHeaderItem( VPrn::cards_COPY_COUNT,
                                               new QStandardItem(QObject::trUtf8("Всего экз")));
    m_DocCard_model->setHorizontalHeaderItem( VPrn::cards_CURRENT_COPY,
                                               new QStandardItem(QObject::trUtf8("Текущий экз")));
    m_DocCard_model->setHorizontalHeaderItem( VPrn::cards_SELECT_ALL_COPY,
                                               new QStandardItem(QObject::trUtf8("Все экз")));
    m_DocCard_model->setHorizontalHeaderItem( VPrn::cards_TEMPLATE_NAME,
                                               new QStandardItem(QObject::trUtf8("Имя файла шаблона")));
    m_DocCard_model->setHorizontalHeaderItem( VPrn::cards_EXECUTOR,
                                               new QStandardItem(QObject::trUtf8("Исполнитель")));
    m_DocCard_model->setHorizontalHeaderItem( VPrn::cards_PRINTMAN,
                                               new QStandardItem(QObject::trUtf8("Отпечатал")));
    m_DocCard_model->setHorizontalHeaderItem( VPrn::cards_PHONE,
                                               new QStandardItem(QObject::trUtf8("Телефон")));
    m_DocCard_model->setHorizontalHeaderItem( VPrn::cards_INV_NUMBER,
                                               new QStandardItem(QObject::trUtf8("Инв.номер")));
    m_DocCard_model->setHorizontalHeaderItem( VPrn::cards_PRINT_DATE,
                                               new QStandardItem(QObject::trUtf8("Дата печати")));
    m_DocCard_model->setHorizontalHeaderItem( VPrn::cards_RECIVER_1,
                                               new QStandardItem(QObject::trUtf8("Получатель_1")));
    m_DocCard_model->setHorizontalHeaderItem( VPrn::cards_RECIVER_2,
                                               new QStandardItem(QObject::trUtf8("Получатель_2")));
    m_DocCard_model->setHorizontalHeaderItem( VPrn::cards_RECIVER_3,
                                               new QStandardItem(QObject::trUtf8("Получатель_3")));
    m_DocCard_model->setHorizontalHeaderItem( VPrn::cards_RECIVER_4,
                                               new QStandardItem(QObject::trUtf8("Получатель_4")));
    m_DocCard_model->setHorizontalHeaderItem( VPrn::cards_RECIVER_5,
                                               new QStandardItem(QObject::trUtf8("Получатель_5")));
    m_DocCard_model->setHorizontalHeaderItem( VPrn::cards_STATUS,
                                               new QStandardItem(QObject::trUtf8("Статус_Документа")));

    qDebug() << "m_DocCard_model ROW:COL" << m_DocCard_model->rowCount() <<" : "<< m_DocCard_model->columnCount();
}

bool DataModule::setWorkFile(const QString & i_file)
{
    if (QFile::exists(i_file)){
        m_Work_file = i_file;
    }else{
        emit error(VPrn::FileNotFound,
                   QObject::trUtf8("Файл %1 заданный для печати не существует!\n%2")
                   .arg(i_file).arg(QString(Q_FUNC_INFO))
                   );
        return false;
    }
    return true;
}

