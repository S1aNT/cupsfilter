#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>


class QGraphicsSceneMouseEvent;
class QGraphicsSceneContextMenuEvent;
class QUndoStack;
class QMenu;
class myTextItem;

class myScene : public QGraphicsScene
{
    Q_OBJECT
public:
    myScene(QUndoStack*, QObject *parent = 0 );
protected:
  void  mousePressEvent( QGraphicsSceneMouseEvent* );        // receive mouse press events
  void  mouseReleaseEvent( QGraphicsSceneMouseEvent* );      // receive mouse release events
  void  contextMenuEvent( QGraphicsSceneContextMenuEvent* ); // receive context menu events

public slots:
  void  selectTextItem();     // records selected Item & positions

private:
  typedef QPair<myTextItem*,QPointF>     ItemPos;
  QList<ItemPos>   m_textitems;    // currently selected textitem & start positions
  QUndoStack*         m_undoStack;            // undo stack for undo & redo of commands
};

#endif // MYSCENE_H
