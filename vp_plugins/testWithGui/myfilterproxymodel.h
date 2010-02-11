#ifndef MYFILTERPROXYMODEL_H
#define MYFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class MyFilterProxyModel : public QSortFilterProxyModel
{
public:
    MyFilterProxyModel(QObject *parent=0);
protected:
    bool filterAcceptsColumn(int source_column,
                                     const QModelIndex &source_parent) const;
};

#endif // MYFILTERPROXYMODEL_H
