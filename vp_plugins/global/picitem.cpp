#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>

#include "picitem.h"
#include <math.h>

PicItem::PicItem(QGraphicsItem * parent)
  :QGraphicsPixmapItem(parent)
{

}

PicItem::PicItem(const QPixmap & pixmap, QGraphicsItem * parent)
  :QGraphicsPixmapItem(pixmap, parent)
{

}

void PicItem::wheelEvent(QGraphicsSceneWheelEvent * event)
{
  if ( hasFocus() )
  {
    qreal fd_xy = (event->delta() > 0)?1.01:0.99;
    scale(fd_xy, fd_xy);
  }
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
    m_Angle     = asin(det / (mod_a * mod_b)) * 180 / M_PI;
    rotate(m_Angle);

    return;
  }

  QGraphicsPixmapItem::mouseMoveEvent(event);
}


