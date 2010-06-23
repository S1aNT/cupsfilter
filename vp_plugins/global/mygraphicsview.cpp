#include "mygraphicsview.h"
#include "mytypes.h"
#include <QVarLengthArray>

myGraphicsView::myGraphicsView( QWidget * parent)
    :QGraphicsView(parent)
    ,gridInterval(int (MM_TO_POINT(5)))
    ,gridShow(true)
{
}
myGraphicsView::myGraphicsView(QGraphicsScene * scene, QWidget * parent)
    :QGraphicsView(scene,parent)
    ,gridInterval(int (MM_TO_POINT(5)))
    ,gridShow(true)
{
}

void myGraphicsView::setShowGrid(bool g_show)
{
    gridShow = g_show;
    this->repaint();
}

void myGraphicsView::setInterval(int i)
{
    gridInterval = int(MM_TO_POINT(i) >MM_TO_POINT(5) ? MM_TO_POINT(i) :MM_TO_POINT(5));
    gridShow = true;
   this->repaint();
}

void myGraphicsView::drawForeground  ( QPainter * painter, const QRectF & rect )
{
    painter->setWorldMatrixEnabled(true);

    if ( gridShow ){
        //interval to draw grid lines at
        QPen pen(Qt::green, 1, Qt::SolidLine);

        painter->setPen(pen);
        qreal left = int(rect.left()) - (int(rect.left()) % gridInterval );
        qreal top = int(rect.top()) - (int(rect.top()) % gridInterval );

        QVarLengthArray<QLineF> linesX;
        for (qreal x = left; x < rect.right(); x += gridInterval )
            linesX.append(QLineF(x, rect.top(), x, rect.bottom()));

        QVarLengthArray<QLineF> linesY;
        for (qreal y = top; y < rect.bottom(); y += gridInterval )
            linesY.append(QLineF(rect.left(), y, rect.right(), y));

        painter->drawLines(linesX.data(), linesX.size());
        painter->drawLines(linesY.data(), linesY.size());
    }else{
        painter->setBrush(Qt::transparent);
        painter->drawRect(rect);

    }
}
