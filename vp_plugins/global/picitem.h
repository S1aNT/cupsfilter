#ifndef PICITEM_H
#define PICITEM_H

#include <QGraphicsItem>
#include <QGraphicsSceneWheelEvent>
#include <QObject>
QT_BEGIN_NAMESPACE
class QFocusEvent;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneContextMenuEvent;
class QAction;
QT_END_NAMESPACE

class PicItem: public QGraphicsPixmapItem//,public QObject
{
 //Q_OBJECT
private:
  QPointF lastPos;

public:
  PicItem(QGraphicsItem * parent = 0);
  PicItem(const QPixmap & pixmap, QGraphicsItem * parent = 0);
public  slots:
   void delElement();

protected:
  void wheelEvent (QGraphicsSceneWheelEvent * event);
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
private:
  QAction *delElemAction;

};

#endif
