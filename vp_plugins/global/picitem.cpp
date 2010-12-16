#define _USE_MATH_DEFINES
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>

#include "picitem.h"
#include <qmath.h>


PicItem::PicItem(QGraphicsItem * parent)
    :QGraphicsPixmapItem(parent)
    ,lastPos(QPointF(0.0,0.0))
    ,m_Angle(0.0)
    ,m_fd_xy(1.0)
{
}

PicItem::PicItem(const QPixmap & pixmap, QGraphicsItem * parent)
    :QGraphicsPixmapItem(pixmap, parent)
    ,lastPos(QPointF(0.0,0.0))
    ,m_Angle(0.0)
    ,m_fd_xy(1.0)
{

}

void PicItem::wheelEvent(QGraphicsSceneWheelEvent * event)
{
    if ( hasFocus() )
    {
        // int delta = event->delta();
        qreal scale_xy = (event->delta() > 0)?1.01:0.99;       
        m_fd_xy = (scale_xy > 1.0)?m_fd_xy+=0.01:m_fd_xy-=0.01;

        scale(scale_xy, scale_xy);
    }
}



qreal PicItem::getScaledSize()  const
{
    return m_fd_xy;
}

void PicItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    if ( event->button() == Qt::MidButton ){
        lastPos = event->pos();
        //return;
    }

    QGraphicsPixmapItem::mousePressEvent(event);
}

void PicItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // при обработке mouseMoveEvent функция event->button() не работает
    // поэтому нужно проверять значение функции event->buttons()!!!

    if (event->buttons() & Qt::MidButton)
    {
        qreal det = lastPos.x() * event->pos().y() - lastPos.y() * event->pos().x();
        qreal mod_a = sqrt(pow(lastPos.x(), 2) + pow(lastPos.y(), 2));
        qreal mod_b = sqrt(pow(event->pos().x(), 2) + pow(event->pos().y(), 2));
        qreal angl = asin(det / (mod_a * mod_b)) * 180 / M_PI;
        m_Angle  +=angl;
        rotate(angl);
        return;
    }

    QGraphicsPixmapItem::mouseMoveEvent(event);
}


void PicItem::setMyTransform(const qreal m_angle,const qreal m_scale)
{
    m_Angle = m_angle;
    m_fd_xy = m_scale;

    QTransform transform;

    transform.rotate(m_Angle);
    transform.scale(m_fd_xy, m_fd_xy);
    this->setTransform(transform);
}

/*
void PicItem::setScaledSize(qreal fd_xy)
{
    m_fd_xy = (fd_xy > 1.0)?m_fd_xy+=0.01:m_fd_xy-=0.01;

    scale(fd_xy, fd_xy);
}
void PicItem::setAngle(qreal angle)
{
    m_Angle     =angle ;
    rotate(m_Angle);
    //rotate(90);
}
*/
