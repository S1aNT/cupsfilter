#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>


class myGraphicsView : public QGraphicsView
{

public:
    myGraphicsView(QWidget * parent = 0);

    myGraphicsView(QGraphicsScene * scene, QWidget * parent = 0 );

    void setInterval(int i);
    void setShowGrid(bool g_show);

protected:
    void drawForeground  ( QPainter * painter, const QRectF & rect );
    int gridInterval;
    bool gridShow;
};

#endif // MYGRAPHICSVIEW_H
