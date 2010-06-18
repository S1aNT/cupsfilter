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
public:
  PicItem(QGraphicsItem * parent = 0);
  PicItem(const QPixmap & pixmap, QGraphicsItem * parent = 0);
  void setMyTransform(const qreal m_angle,const qreal m_scale);

  qreal  getAngle()  const {return m_Angle;}
  qreal  getScaledSize()  const;

  //void setAngle(qreal angle);
  //void setScaledSize(qreal fd_xy);

protected:
  void wheelEvent (QGraphicsSceneWheelEvent * event);
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
private:
  //QAction *delElemAction;
  QPointF lastPos;
  qreal   m_Angle;
  qreal   m_fd_xy;
};

#endif
