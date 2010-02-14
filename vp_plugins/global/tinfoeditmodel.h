#ifndef tInfoEditModel_H
#define tInfoEditModel_H

#include <QSqlQueryModel>
#include <QDebug>
#include <QSqlDatabase>

#include "mytypes.h"

/** 
  * @class Редактируемая SQl модель позволяет записывать данные в 
  *  следующие столбцы:
  * реж  №   Описание
  *  r | 0  |ID
  * rw | 1  |Имя шаблона
  * rw | 2  |Описание шаблона;
  *  r | 3  |Ссылка на таблицу размеры страниц 
  * rw | 4  |Поворот (град.)
  * rw | 5  |Время создания
  * rw | 6  |Время изменения
  * rw | 7  |Автор
  * rw | 8  |Отступ сверху (мм)
  * rw | 9  |Отступ снизу (мм)
  * rw | 10 |Отступ слева (мм)
  * rw | 11 |Отступ справа (мм)
  */



using namespace VPrn;

class TemplateInfoEditModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    TemplateInfoEditModel(QObject *parent = 0);

    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    void refresh();

private:

    bool setTemplateName   (int Id, const QString &templateName);
    bool setTemplateDesc   (int Id, const QString &templateDesc);
    bool setTemplatePageId (int Id, int pageId);
    bool setTemplateAngle  (int Id, int angle);
    bool setTemplateCTime  (int Id, int cTime);
    bool setTemplateMTime  (int Id, int mTime);
    bool setTemplateAuthor (int Id, const QString &templateAuthor);
    bool setTemplateMTop   (int Id, int mTop);
    bool setTemplateMBottom(int Id, int mBottom);
    bool setTemplateMLeft  (int Id, int mLeft);
    bool setTemplateMRight (int Id, int mRight);

};

#endif
