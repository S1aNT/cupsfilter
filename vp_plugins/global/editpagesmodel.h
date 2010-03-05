#ifndef EDITPAGESMODEL_H
#define EDITPAGESMODEL_H

#include <QSqlQueryModel>
#include "mytypes.h"

/**
  * @class EditPagesModel Редактируемая SQl модель
  * @brief позволяет записывать данные в  следующие столбцы:
  * реж  №   Описание
  * r  | 0  |ID
  * r  | 1  |Порядковый номер
  * r  | 2  |Тип страницы;
  * r  | 3  |Название страницы
  * rw | 4  |Видна страница да/нет 1/0
  */

using namespace VPrn;

class EditPagesModel : public QSqlQueryModel
{
Q_OBJECT
public:
    explicit EditPagesModel(QObject *parent = 0);

    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    void refresh();
private:
    bool setPagesName   (int id, QString &name);
    bool setPagesNumber (int id, int number);
    bool setPagesType   (int id, int type);
    bool setPageVisible (int id, int page_visible);
};

#endif // EDITPAGESMODEL_H
