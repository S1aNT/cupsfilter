#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>

#include "PicItem.h"
#include <math.h>

PicItem::PicItem(QGraphicsItem * parent)
  :QGraphicsPixmapItem(parent)
{

}

PicItem::PicItem(const QPixmap & pixmap, QGraphicsItem * parent)
  :QGraphicsPixmapItem(pixmap, parent)
{
    delElemAction = new QAction (QObject::trUtf8("Удалить элемент"),0);
    //QObject::connect (delElemAction,SIGNAL(triggered()),this,SLOT(delElement()));

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

    if ( event->button() == Qt::MidButton )
  {
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
    qreal angle = asin(det / (mod_a * mod_b)) * 180 / M_PI;

    rotate(angle);

    return;
  }

  QGraphicsPixmapItem::mouseMoveEvent(event);
}

void PicItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    menu.addSeparator();
    menu.addAction(delElemAction);
    //menu.exec(event->screenPos());
    if ( menu.exec( event->screenPos() ) ==  delElemAction ){
        this->delElement();
    }
}
void PicItem::delElement()
{
    this->hide();
}

