#include "templatesinfo.h"

#include <QtGui/QStandardItemModel>
#include <QtGui/QStandardItem>

#include <QtCore/QDataStream>

#include "mytypes.h"

using namespace VPrn;

TemplatesInfo::TemplatesInfo(QObject *parent)
    : QObject(parent)
    , tInfoModel(0)
{
    tInfoModel = new QStandardItemModel(this);

}

void TemplatesInfo::setStdHeader()
{
    if (tInfoModel){
        tInfoModel->clear();


        // Настройка колонок
        tInfoModel->setHorizontalHeaderItem( VPrn::metaInfo_id,
                                             new QStandardItem("id"));
        tInfoModel->setHorizontalHeaderItem( VPrn::metaInfo_name,
                                             new QStandardItem("Имя шаблона"));
        tInfoModel->setHorizontalHeaderItem( VPrn::metaInfo_fname,
                                             new QStandardItem("Путь к шаблону"));
        tInfoModel->setHorizontalHeaderItem( VPrn::metaInfo_desc,
                                             new QStandardItem("Описание шаблона"));
        tInfoModel->setHorizontalHeaderItem( VPrn::metaInfo_orient,
                                             new QStandardItem("Ориентация"));
        tInfoModel->setHorizontalHeaderItem( VPrn::metaInfo_ctime,
                                             new QStandardItem("Время создания"));
        tInfoModel->setHorizontalHeaderItem( VPrn::metaInfo_mtime,
                                             new QStandardItem("Время изменения"));
        tInfoModel->setHorizontalHeaderItem( VPrn::metaInfo_author,
                                             new QStandardItem("Автор шаблона"));
        tInfoModel->setHorizontalHeaderItem( VPrn::metaInfo_mtop,
                                             new QStandardItem("Отступ сверху (мм)"));
        tInfoModel->setHorizontalHeaderItem( VPrn::metaInfo_mbottom,
                                             new QStandardItem("Отступ снизу (мм)"));
        tInfoModel->setHorizontalHeaderItem( VPrn::metaInfo_mleft,
                                             new QStandardItem("Отступ слева (мм)"));
        tInfoModel->setHorizontalHeaderItem( VPrn::metaInfo_mright,
                                             new QStandardItem("Отступ справа (мм)"));
        tInfoModel->setHorizontalHeaderItem( VPrn::metaInfo_p_width,
                                             new QStandardItem("Ширина страницы (мм)"));
        tInfoModel->setHorizontalHeaderItem( VPrn::metaInfo_p_height,
                                             new QStandardItem("Высота страницы (мм)"));
    }
}
void TemplatesInfo::fromByteArray(QByteArray m_data)
{
    if ( tInfoModel && !m_data.isEmpty() ){
        //Обработка данных
        QDataStream in(&m_data, QIODevice::ReadOnly );
        in.setVersion(QDataStream::Qt_3_0);
        int rows;
        int columns;
        in >> rows;
        in >> columns;
        for (int i=0;i<rows;i++){
            int r;
            in >> r;
            for (int j=0;j<columns;j++){
                int c;
                in >> c;
                QStandardItem item;
                in >> item;
                tInfoModel->setItem(r,c,&item);
            }
        }


    }
}

QByteArray TemplatesInfo::toByteArray() const
{
    QByteArray data_;
    if ( tInfoModel ){
        //Обработка данных
        QDataStream out(&data_, QIODevice::WriteOnly );
        out.setVersion(QDataStream::Qt_3_0);
        out << tInfoModel->rowCount();
        out << tInfoModel->columnCount();
        for (int i=0; i<tInfoModel->rowCount(); i++){
            out << i;
            for (int j=0; tInfoModel->columnCount(); j++){
                QStandardItem *item;
                item = tInfoModel->item(i,j);
                out << item;
            }
        }
    }
    return data_;
}

TemplatesInfo::~TemplatesInfo()
{
    if ( tInfoModel){
        tInfoModel->deleteLater();
    }
}


