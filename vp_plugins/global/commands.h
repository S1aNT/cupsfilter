#ifndef COMMANDS_H
#define COMMANDS_H

#include "mytextitem.h"
#include "mytypes.h"

#include <QUndoCommand>
#include <QGraphicsScene>
#include <QtGui/QFont>
#include <QtGui/QColor>
#include <QtGlobal>

using namespace VPrn;

class CommandTextItemRotate : public QUndoCommand
{
public:
    CommandTextItemRotate( QGraphicsScene* scene,  myTextItem*  TextItem, qreal direct )
    {
        m_TextItem   = TextItem;
        m_scene      = scene;
        oldDirect    = m_TextItem->getAngle();
        newDirect    = direct;
        if ( direct  == 90 ){
            setText( QObject::tr("Элемент %1 повернут вправо")
                     .arg( m_TextItem->getETag() ) );
        }else{
            setText( QObject::tr("Элемент %1 повернут влево")
                     .arg( m_TextItem->getETag() ) );
        }

    }


    virtual void undo()    { m_TextItem->setAngle(oldDirect); }
    virtual void redo()    { m_TextItem->setAngle(newDirect); }

private:
    myTextItem*      m_TextItem;
    QGraphicsScene*  m_scene;
    qreal oldDirect;
    qreal newDirect;
};


class CommandTextItemChangeTag : public QUndoCommand
{
public:
    CommandTextItemChangeTag( QGraphicsScene* scene,  myTextItem*  TextItem, QString &mTag)
    {
        m_TextItem = TextItem;
        m_scene    = scene;
        oldTag     = m_TextItem->getETag();
        newTag     = mTag;
        setText( QObject::tr("Элемент %1 изменен на %2")
                 .arg(oldTag,newTag) );
    }


    virtual void undo()    { m_TextItem->setETag(oldTag); }
    virtual void redo()    { m_TextItem->setETag(newTag); }

private:
    myTextItem*      m_TextItem;
    QGraphicsScene*  m_scene;
    QString oldTag;
    QString newTag;
};

class CommandTextItemChangeColor : public QUndoCommand
{
public:
    CommandTextItemChangeColor( QGraphicsScene* scene,  myTextItem*  TextItem, QColor &clr )
    {
        m_TextItem = TextItem;
        m_scene    = scene;
        oldColor    = m_TextItem->defaultTextColor();
        newColor    = clr;
        setText( QObject::tr("У элемента %1 изменен цвет на %2")
                 .arg(m_TextItem->getETag(),clr.name()) );
    }


    virtual void undo()    { m_TextItem->setDefaultTextColor(oldColor); }
    virtual void redo()    { m_TextItem->setDefaultTextColor(newColor); }

private:
    myTextItem*      m_TextItem;
    QGraphicsScene*  m_scene;
    QColor oldColor;
    QColor newColor;
};

class CommandTextItemChangeFont : public QUndoCommand
{
public:
    CommandTextItemChangeFont( QGraphicsScene* scene,  myTextItem*  TextItem, QFont &fnt )
    {
        m_TextItem = TextItem;
        m_scene    = scene;
        oldFont    = m_TextItem->font();
        newFont    = fnt;
        setText( QObject::tr("У элемента %1 изменен шрифт на %2")
                 .arg(m_TextItem->getETag(),fnt.family()) );
    }


    virtual void undo()    { m_TextItem->setFont(oldFont); }
    virtual void redo()    { m_TextItem->setFont(newFont); }

private:
    myTextItem*      m_TextItem;
    QGraphicsScene*  m_scene;
    QFont oldFont;
    QFont newFont;
};

class CommandTextItemAdd : public QUndoCommand
{
public:
    CommandTextItemAdd( QGraphicsScene* scene,  myTextItem*  TextItem )
    {
        m_TextItem = TextItem;
        m_scene    = scene;
        setText( QObject::tr("Элемент %1 создан").arg(m_TextItem->getETag()) );
    }

    ~CommandTextItemAdd()
    {

        if ( !m_scene->items().contains( m_TextItem ) )
            delete m_TextItem;
    }

    virtual void undo()    { m_scene->removeItem( m_TextItem ); }
    virtual void redo()    { m_scene->addItem( m_TextItem ); }

private:
    myTextItem*      m_TextItem;   // station being added
    QGraphicsScene*  m_scene;      // scene where station being added
};

class CommandTextItemDelete : public QUndoCommand
{
public:
    CommandTextItemDelete( QGraphicsScene* scene,  myTextItem*  TextItem )
    {
        m_scene   = scene;
        m_TextItem =  TextItem;
        setText( QObject::tr("Элемент %1 удален").arg(m_TextItem->getETag()) );
    }

    virtual void undo()    { m_scene->addItem(  m_TextItem ); }

    virtual void redo()    { m_scene->removeItem(  m_TextItem ); }

private:
    QGraphicsScene*  m_scene;      // scene where station being deleted
    myTextItem*      m_TextItem;
};

class CommandTextItemMove : public QUndoCommand
{
public:
    CommandTextItemMove(  myTextItem*  TextItem, qreal fromX, qreal fromY, qreal toX, qreal toY )
    {
        m_TextItem =  TextItem;
        m_from    = QPointF( fromX, fromY );
        m_to      = QPointF( toX, toY );
        setText( QObject::tr("Элемент %5 перемещен %1,%2 -> %3,%4")
                 .arg(fromX).arg(fromY).arg(toX).arg(toY).arg(m_TextItem->getETag()) );
    }

    virtual void undo()    { m_TextItem->setPos( m_from ); }

    virtual void redo()    { m_TextItem->setPos( m_to ); }

private:
    myTextItem*      m_TextItem;       // station being moved
    QPointF    m_from;          // original coords
    QPointF    m_to;            // new coords
};

#endif  // COMMANDS

