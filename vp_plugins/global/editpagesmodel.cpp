#include "editpagesmodel.h"
#include <QtSql/QSqlQuery>
#include <QtCore/QModelIndex>

using namespace VPrn;

EditPagesModel::EditPagesModel(QObject *parent) :
        QSqlQueryModel(parent)
{
    qRegisterMetaType<pageDetailColumnOrder>("pageDetailColumnOrder");
}



Qt::ItemFlags EditPagesModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QSqlQueryModel::flags(index);

    if (index.column() != VPrn::PD_id){
        flags |= Qt::ItemIsEditable;
    }
    return flags;
}


bool EditPagesModel::setData(const QModelIndex &index, const QVariant &value, int /* role */)
{
    if (index.column() == VPrn::PD_id)
        return false;

    QModelIndex primaryKeyIndex = QSqlQueryModel::index(index.row(), 0);
    int id = data(primaryKeyIndex).toInt();
    QString tmp_name;
    clear();

    bool ok;
    switch (index.column()){
    case VPrn::PD_p_name:
        tmp_name = value.toString();
        ok = setPagesName   (id,tmp_name);
        break;
    case VPrn::PD_p_number:  ok = setPagesNumber (id,value.toInt());
        break;
    case VPrn::PD_p_type:    ok = setPagesType   (id,value.toInt());
        break;
    case VPrn::PD_p_visible: ok = setPageVisible (id,value.toInt());
        break;
    }

    refresh();
    if (ok){
        emit dataChanged(index, index);
    }
    return ok;
}

bool EditPagesModel::setPagesName   (int id, QString &name)
{
    QSqlQuery query;
    query.prepare("update page_detail set p_name = ? where id = ?");
    query.addBindValue(name);
    query.addBindValue(id);
    return query.exec();
}

bool EditPagesModel::setPagesNumber (int id, int number)
{
    QSqlQuery query;
    query.prepare("update page_detail set p_number = ? where id = ?");
    query.addBindValue(number);
    query.addBindValue(id);
    return query.exec();
}

bool EditPagesModel::setPagesType   (int id, int type)
{
    QSqlQuery query;
    query.prepare("update page_detail set p_type = ? where id = ?");
    query.addBindValue(type);
    query.addBindValue(id);
    return query.exec();

}

bool EditPagesModel::setPageVisible (int id, int page_visible)
{
    QSqlQuery query;
    query.prepare("update page_detail set p_visible = ? where id = ?");
    query.addBindValue(page_visible);
    query.addBindValue(id);
    return query.exec();
}

void EditPagesModel::refresh()
{
    setQuery("SELECT page_detail.id,page_detail.p_number,page_detail.p_type,"
             " page_detail.p_name,page_detail.p_visible FROM template "
             " INNER JOIN rel_templ_page ON templ_id= template.id"
             " INNER JOIN page_detail ON page_detail_id =  page_detail.id"
             " ORDER BY p_number");

    setHeaderData(VPrn::PD_id,        Qt::Horizontal, tr("Id"));
    setHeaderData(VPrn::PD_p_number,  Qt::Horizontal, tr("Порядковый номер"));
    setHeaderData(VPrn::PD_p_type,    Qt::Horizontal, tr("Тип страницы"));
    setHeaderData(VPrn::PD_p_name,    Qt::Horizontal, tr("Название страницы"));
    setHeaderData(VPrn::PD_p_visible, Qt::Horizontal, tr("Видимость страницы"));
}
