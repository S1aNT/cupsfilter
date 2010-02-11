#ifndef tInfoEditModel_H
#define tInfoEditModel_H

#include <QSqlQueryModel>


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

enum pSizeColumnOrder{
        pSize_id     = 0, // ID
        pSize_page   = 1, // Размер листа
        pSize_code   = 2, // Код
        pSize_width  = 3, // Ширина (мм)
        pSize_height = 4  // Высота (мм)

};
enum tInfoColumnOrder{
            tInfo_id     = 0,  //ID
            tInfo_name   = 1,  //Имя шаблона
            tInfo_desc   = 2,  //Описание шаблона;
            tInfo_pageID = 3,  //Ссылка на таблицу размеры страниц
            tInfo_angle  = 4,  //Поворот (град.)
            tInfo_ctime  = 5,  //Время создания
            tInfo_mtime  = 6,  //Время изменения
            tInfo_author = 7, //Автор
            tInfo_mtop   = 8, //Отступ сверху (мм)
            tInfo_mbottom = 9,//Отступ снизу (мм)
            tInfo_mleft   = 10,//Отступ слева (мм)
            tInfo_mright  = 11 //Отступ справа (мм)

};



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
    bool setTemplateAngle  (int Id, double angle);
    bool setTemplateCTime  (int Id, int cTime);
    bool setTemplateMTime  (int Id, int mTime);
    bool setTemplateAuthor (int Id, const QString &templateAuthor);
    bool setTemplateMTop   (int Id, double mTop);
    bool setTemplateMBottom(int Id, double mBottom);
    bool setTemplateMLeft  (int Id, double mLeft);
    bool setTemplateMRight (int Id, double mRight);


};

#endif
