#ifndef TEMPL_SCENE_H
#define TEMPL_SCENE_H
#include <qStandardItemModel>
#include <QGraphicsScene>
#include <QMap>
#include <QFile>
#include <QDataStream>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QPixmap>


#define MM_TO_POINT(mm) ((mm)*2.83465058)
static const int ObjectName = 0;
static const int ObjectType = 1;

#include "simpleitem.h"

class templ_scene : public QGraphicsScene
{
    Q_OBJECT
public:
    templ_scene(QObject *parent = 0);
    void setModel (QStandardItemModel *model);
public slots:
    void load_from_file(QString &file_name);
    void compilTemplates(int RowId);
    
private:
    void setPageSize(qreal width,qreal height);

protected:
    QMap<int, QGraphicsItem *> front_side_page; // Лицевая сторона листа
    QMap<int, QGraphicsItem *> back_side_page; // Обратная сторона листа
    QMap<int, QGraphicsItem *> last_page; // Последняя страница
    qreal page_width;
    qreal page_height;
    bool page_orient;
    //QStandartItemModel *pModel;
};

#endif // TEMPL_SCENE_H
