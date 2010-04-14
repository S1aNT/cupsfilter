#include "templatesinfo.h"

#include <QtGui/QStandardItemModel>
#include <QtGui/QStandardItem>

#include <QtCore/QDataStream>
#include <QtCore/QFile>
#include <QtCore/QList>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

#include "mytypes.h"

using namespace VPrn;

TemplatesInfo::TemplatesInfo(QObject *parent)
    : QObject(parent)
    , m_model(0)
    , last_error(QString())
{
    m_model = new QStandardItemModel(this);
}

void TemplatesInfo::setHorizontalHeaderLabels()
{
    if (m_model){
        m_model->clear();

        // Настройка колонок
        m_model->setHorizontalHeaderItem( VPrn::metaInfo_name,
                                          new QStandardItem("Имя шаблона"));
        m_model->setHorizontalHeaderItem( VPrn::metaInfo_fname,
                                          new QStandardItem("Путь к шаблону"));
        m_model->setHorizontalHeaderItem( VPrn::metaInfo_desc,
                                          new QStandardItem("Описание шаблона"));
        m_model->setHorizontalHeaderItem( VPrn::metaInfo_orient,
                                          new QStandardItem("Ориентация"));
        m_model->setHorizontalHeaderItem( VPrn::metaInfo_ctime,
                                          new QStandardItem("Время создания"));
        m_model->setHorizontalHeaderItem( VPrn::metaInfo_mtime,
                                          new QStandardItem("Время изменения"));
        m_model->setHorizontalHeaderItem( VPrn::metaInfo_author,
                                          new QStandardItem("Автор шаблона"));
        m_model->setHorizontalHeaderItem( VPrn::metaInfo_mtop,
                                          new QStandardItem("Отступ сверху (мм)"));
        m_model->setHorizontalHeaderItem( VPrn::metaInfo_mbottom,
                                          new QStandardItem("Отступ снизу (мм)"));
        m_model->setHorizontalHeaderItem( VPrn::metaInfo_mleft,
                                          new QStandardItem("Отступ слева (мм)"));
        m_model->setHorizontalHeaderItem( VPrn::metaInfo_mright,
                                          new QStandardItem("Отступ справа (мм)"));
        m_model->setHorizontalHeaderItem( VPrn::metaInfo_p_width,
                                          new QStandardItem("Ширина страницы (мм)"));
        m_model->setHorizontalHeaderItem( VPrn::metaInfo_p_height,
                                          new QStandardItem("Высота страницы (мм)"));
    }
}

void TemplatesInfo::fromByteArray(QByteArray m_data)
{
    if ( m_model && !m_data.isEmpty() ){
        //Обработка данных
        QDataStream in(&m_data, QIODevice::ReadOnly );
        in.setVersion(QDataStream::Qt_3_0);
        int rows;
        int columns;
        in >> rows;
        in >> columns;
        for (int i=0;i<rows;i++){
            for (int j=0;j<columns;j++){                
                QStandardItem *item = new QStandardItem();
                QString txt;
                in >> txt;
                item->setText(txt);
                m_model->setItem(i,j,item);
            }
        }
    }
}

QByteArray TemplatesInfo::toByteArray() const
{
    QByteArray data_;

    if ( m_model ){
        qDebug() << Q_FUNC_INFO
                << "\nModel row = "    << m_model->rowCount()
                << "\nModel column = " << m_model->columnCount();
        data_.clear();
        //Обработка данных
        QDataStream out(&data_, QIODevice::WriteOnly );
        out.setVersion(QDataStream::Qt_3_0);
        out << m_model->rowCount();
        out << m_model->columnCount();
        for (int i=0; i < m_model->rowCount(); i++){
            for (int j=0; j < m_model->columnCount(); j++){
                QStandardItem *item;
                item = m_model->item(i,j);
                QString txt = item->text();
                out << item->text();
            }
        }
    }
    qDebug() << "Model to QByteArray size:" << data_.size();
    return data_;
}

TemplatesInfo::~TemplatesInfo()
{
    if ( m_model){
        m_model->deleteLater();
    }
}

bool TemplatesInfo::getMetaInfo(const QString &client_id,
                                const QStringList &list )
{
    bool rez =true;
    {
        if ( m_model && !list.isEmpty() ){
            {
                QSqlDatabase db  = QSqlDatabase::addDatabase(QLatin1String("QSQLITE"),client_id);

                if (db.driver()->lastError().type() != QSqlError::ConnectionError) {
                    //m_model->clear();
                    for (int i = 0; i<list.size(); i++){
                        db.setDatabaseName( list.at(i) );
                        if (db.open()){
                            // Основная работа с шаблонами
                            QSqlQuery query(db);
                            QList<QStandardItem *> itemList;

                            if (query.exec( "SELECT t_name,t_desc,"
                                            "angle,c_time,m_time,author,margin_top,margin_bottom,"
                                            "margin_left,margin_right,page_size.p_width,page_size.p_height  "
                                            "FROM template "
                                            " INNER JOIN page_size ON template.page_size_id=page_size.id")
                                ){
                                int field_t_name   = query.record().indexOf("t_name");
                                int field_t_desc   = query.record().indexOf("t_desc");
                                int field_angle    = query.record().indexOf("angle");
                                int field_c_time   = query.record().indexOf("c_time");
                                int field_m_time   = query.record().indexOf("m_time");
                                int field_author   = query.record().indexOf("author");
                                int field_m_top    = query.record().indexOf("margin_top");
                                int field_m_bottom = query.record().indexOf("margin_bottom");
                                int field_m_left   = query.record().indexOf("margin_left");
                                int field_m_right  = query.record().indexOf("margin_right");
                                int field_p_width  = query.record().indexOf("p_width");
                                int field_p_height = query.record().indexOf("p_height");

                                while (query.next()) {

                                    itemList.append(new QStandardItem (
                                            query.value(field_t_name).toString() )
                                                    );
                                    itemList.append(new QStandardItem( list.at(i) )
                                                    );
                                    itemList.append(new QStandardItem (
                                            query.value(field_t_desc).toString() )
                                                    );
                                    itemList.append(new QStandardItem (
                                            query.value(field_angle).toString() )
                                                    );
                                    itemList.append(new QStandardItem (
                                            query.value(field_c_time).toString() )
                                                    );
                                    itemList.append(new QStandardItem (
                                            query.value(field_m_time).toString() )
                                                    );
                                    itemList.append(new QStandardItem (
                                            query.value(field_author).toString() )
                                                    );
                                    itemList.append(new QStandardItem (
                                            query.value(field_m_top).toString() )
                                                    );
                                    itemList.append(new QStandardItem (
                                            query.value(field_m_bottom).toString() )
                                                    );
                                    itemList.append(new QStandardItem (
                                            query.value(field_m_left).toString() )
                                                    );
                                    itemList.append(new QStandardItem (
                                            query.value(field_m_right).toString() )
                                                    );
                                    itemList.append(new QStandardItem (
                                            query.value(field_p_width).toString() )
                                                    );
                                    itemList.append(new QStandardItem (
                                            query.value(field_p_height).toString() )
                                                    );
                                }
                                m_model->appendRow(itemList);

                            }else{
                                rez &=false;
                                last_error = query.lastError().text();
                            }
                            db.close();
                        }
                    }
                }else{
                    rez &=false;
                }
            }
            QSqlDatabase::removeDatabase(client_id);
        }else{
            rez &=false;
            last_error = QObject::trUtf8("Нет модели или список шаблонов пуст!");
        }
    }
    return rez;
}



/*
void TemplatesInfo::setHorizontalHeaderLabels(const QStringList &header)
{
    if (m_model && !header.isEmpty()){
        m_model->clear();
        m_model->setHorizontalHeaderLabels(header);
    }
}
*/
