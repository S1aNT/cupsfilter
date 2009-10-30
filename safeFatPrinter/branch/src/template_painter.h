#ifndef TEMPLATE_PAINTER_H
#define TEMPLATE_PAINTER_H

#include <QObject>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QMap>
#include <QMatrix>
#include <QFile>
#include <QDataStream>

#define POINT_TO_CM(cm) ((cm)/28.3465058)
#define POINT_TO_MM(mm) ((mm)/2.83465058)     ////////  0.352777778
#define POINT_TO_DM(dm) ((dm)/283.465058)
#define POINT_TO_INCH(inch) ((inch)/72.0)

#define MM_TO_POINT(mm) ((mm)*2.83465058)
#define CM_TO_POINT(cm) ((cm)*28.3465058)     ///// 28.346456693
#define DM_TO_POINT(dm) ((dm)*283.465058)
#define INCH_TO_POINT(inch) ((inch)*72.0)
static const int ObjectName = 0;

class template_painter : public QObject
{
    Q_OBJECT
public:
    template_painter();
    QGraphicsScene * getScene();
public slots:
    //void draw_overPage(bool PageOrient,QString &psize,QString &stamp,QString &mb);
    void read4filedraw2scene(QString &file_name,int page);
private:
    void setPaperSize(const QString &psize);
    QMap<QString, int> page_size;
    QMatrix matrix;

    QRect main_rect;
    int page_orientation;

    int page_height;
    int page_width;
    int margin_top,margin_bottom,margin_left,margin_right;
    bool page_orient;
    QGraphicsScene *scene;
    QPainter * paintBox;
};

#endif // TEMPLATE_PAINTER_H
