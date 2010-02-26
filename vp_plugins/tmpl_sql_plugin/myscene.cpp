#include "myscene.h"
#include "mytextitem.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QUndoStack>




myScene::myScene( QUndoStack* undoStack, QObject *parent )
        : QGraphicsScene(parent)
{
    // initialise variables
    m_undoStack     = undoStack;

}
