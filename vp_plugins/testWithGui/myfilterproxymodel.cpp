#include "myfilterproxymodel.h"
#include <QStandardItem>
#include <QString>

MyFilterProxyModel::MyFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}


bool MyFilterProxyModel::filterAcceptsColumn(int source_column,
                                          const QModelIndex &source_parent) const
{

    QString header =sourceModel()->headerData(
            source_column,Qt::Horizontal,Qt::DisplayRole).toString();
    QString fnd = tr("Размер листа");
    if (header == fnd){
        return true;
    }else{
        return false;
    }
}
