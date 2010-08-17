#include "myscene.h"
#include "mytextitem.h"
#include "picitem.h"

#include "commands.h"
#include "mytypes.h"

#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QGraphicsSceneContextMenuEvent>
#include <QtGui/QMenu>
#include <QtGui/QAction>
#include <QtGui/QUndoStack>
#include <QtGui/QFontDialog>
#include <QtGui/QFont>
#include <QtGui/QColorDialog>
#include <QtGui/QInputDialog>
#include <QtGui/QGraphicsPixmapItem>

#include <QtCore/QLineF>
#include <QtCore/QVarLengthArray>
#include <QPainter>
using namespace VPrn;

myScene::myScene(int Number, QUndoStack* undoStack, QObject *parent )
    : QGraphicsScene(parent)
    , m_mode (true)
    , m_angle(0.0)
    , m_undoStack(0)
    , m_Number(-1)
{
    // initialise variables
    m_undoStack     = undoStack;
    connect( this, SIGNAL(selectionChanged()),
             this, SLOT (selectItems()) );
    m_Number = Number;
}


QUndoStack *myScene::undoStack() const
{
    return m_undoStack;
}

void  myScene::createPageForPrint(qreal width,qreal height)
{
    this->clear();
    this->setSceneRect(0, 0, width,height);
    this->setBackgroundBrush(Qt::white);

    QPixmap pixmap = QPixmap(width,height);
    pixmap.fill(Qt::white);

    QGraphicsPixmapItem *paper_rect =  new QGraphicsPixmapItem();
    paper_rect->setPixmap(pixmap);
    paper_rect->setData(ObjectName, "Paper");
    paper_rect->setZValue(-1000.0);
    paper_rect->setShapeMode(QGraphicsPixmapItem::HeuristicMaskShape);
    this->addItem(paper_rect);
}



void  myScene::createPage(qreal width,qreal height,qreal m_top,qreal m_bottom,
                          qreal m_right,qreal m_left)
{
    QPixmap pixmap;
    QPixmap pixmap2;

    this->clear();
    this->setSceneRect(0, 0, width,height);
    //this->setBackgroundBrush(Qt::white);
    this->setBackgroundBrush(Qt::transparent);

    QGraphicsRectItem *paper_rect =
            new QGraphicsRectItem (QRectF(0,0, width,height));
    paper_rect->setPen(QPen(Qt::black));
    paper_rect->setBrush(QBrush(Qt::white));
    paper_rect->setZValue(-1000.0);
    paper_rect->setData(ObjectName, "Paper");
    this->addItem(paper_rect);

    QGraphicsRectItem *border_rect =
            new QGraphicsRectItem (
                    QRectF(m_left, m_top,width-m_left-m_right,height-m_top-m_bottom)
                    );

    border_rect->setPen(QPen(Qt::black,2,Qt::DotLine));
    border_rect->setBrush(QBrush(Qt::white));
#if QT_VERSION >= 0x040500
    border_rect->setOpacity(1);
#endif

    border_rect->setZValue(-900);
    border_rect->setData(ObjectName, "Border");
    border_rect->setParentItem(paper_rect);

}

QGraphicsItem * myScene::getPaperItem()
{
    QGraphicsItem *item(0);

    // Поиск  указателя на бумагу
    for (int i = 0; i < this->items().size(); ++i){
        item = this->items().at(i);
        if ( item->data(ObjectName).toString()=="Paper"){
            break;
        }
    }
    return item;

}


void myScene::AddImgElement(const QString & file_name)
{
    QPixmap pixmap(file_name);

    PicItem *Item = new PicItem(pixmap,this->getPaperItem());
    Item->setData(ObjectName, "tImg");
    Item->setZValue(100);
    Item->setFlags(QGraphicsItem::ItemIsMovable |
                   QGraphicsItem::ItemIsSelectable |
                   QGraphicsItem::ItemIsFocusable);
    Item->setOffset(-0.5*QPointF(pixmap.width(), pixmap.height()));
    Item->setTransformationMode(Qt::SmoothTransformation);

}


void myScene::addImgElem(const QPointF &ps,const qreal m_angle,const qreal m_scaled,const QPixmap &img )
{
    PicItem *Item;

    Item = new PicItem(img);
    Item->setData(ObjectName, "tImg");

    Item->setZValue(100);
    Item->setPos(ps);

    Item->setFlags(QGraphicsItem::ItemIsMovable |
                   QGraphicsItem::ItemIsSelectable |
                   QGraphicsItem::ItemIsFocusable);

    Item->setOffset(-0.5*QPointF(img.width(), img.height()));
    Item->setTransformationMode(Qt::SmoothTransformation);

    Item->setMyTransform(m_angle,m_scaled);
    Item->setParentItem(this->getPaperItem());
}


void myScene::addBaseElem(const QString &tag,const QString &text,const QPointF &ps,
                          const QFont &fnt,const QColor &col,const qreal m_angle)
{
    myTextItem * pItem = new myTextItem(this->getPaperItem());

    pItem->setPos(ps);
    pItem->setFont(fnt);
    pItem->setDefaultTextColor(col);

    pItem->setETag(tag);

    if (text.isEmpty()){
        pItem->setEText(tag);
    }else{
        pItem->setEText(text);
    }
    if (m_mode){
        pItem->setPlainText(tag);
    }else{
        pItem->setPlainText(text);
    }


    pItem->setZValue(100);
    pItem->setFlag(QGraphicsItem::ItemIsMovable, true);
    pItem->setFlag(QGraphicsItem::ItemIsSelectable, true);
    pItem->setFlag(QGraphicsItem::ItemIsFocusable, true);

    pItem->setData(ObjectName, "tElem");
    if (m_undoStack){
        m_undoStack->push( new CommandTextItemAdd( this, pItem ) );
        m_undoStack->push( new CommandTextItemRotate( this, pItem,m_angle ) );
    }else{
        pItem->setAngle(m_angle);

    }
}

void myScene::selectItems()
{
    // refresh record of selected stations and their starting positions
    m_TextItems.clear();

    foreach( QGraphicsItem* item, selectedItems() ){

        if ( dynamic_cast<myTextItem*>( item ) ){
            m_TextItems.append(
                    qMakePair( dynamic_cast<myTextItem*>(item), item->pos() )
                    );
        }
    }
}



void  myScene::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{
    // if any TextItem moved, then create undo commands
    foreach( ItemPos TextItemPos ,m_TextItems )
        if ( TextItemPos.first->pos() != TextItemPos.second )
            m_undoStack->push( new CommandTextItemMove( TextItemPos.first,
                                                        TextItemPos.second.x(), TextItemPos.second.y(),
                                                        TextItemPos.first->x(), TextItemPos.first->y() ) );

    // refresh record of selected stations and call base mouseReleaseEvent
    selectItems();
    QGraphicsScene::mouseReleaseEvent( event );
}

void  myScene::contextMenuEvent( QGraphicsSceneContextMenuEvent* event )
{
    // we only want to display a menu if user clicked a textItem
    qreal     x       = event->scenePos().x();
    qreal     y       = event->scenePos().y();
    QMenu menu;
    myTextItem*  textItem = dynamic_cast<myTextItem*>( itemAt( x, y ) );
    PicItem*  picItem = dynamic_cast<PicItem*>( itemAt( x, y ) );
    qreal angle;
    if (textItem == 0 && picItem ==0) return;
    if ( textItem != 0 ){


        setTagAction      = menu.addAction(QIcon(":/edit.png"),
                                           QObject::trUtf8("Изменить значение тега"));
        changeFontAction  = menu.addAction(QIcon(":/fontDialog.png"),
                                           QObject::trUtf8("Изменить шрифт"));
        changeColorAction = menu.addAction(QIcon(":/colorDialog.png"),
                                           QObject::trUtf8("Изменить цвет"));
        menu.addSeparator();
        rotateRightAction = menu.addAction(QIcon(":/rotateRight.png"),
                                           QObject::trUtf8("Вращать вправо на 90град."));
        rotateLeftAction  = menu.addAction(QIcon(":/rotateLeft.png"),
                                           QObject::trUtf8("Вращать влево на 90град."));
        menu.addSeparator();
    }
    delElemAction = menu.addAction(QObject::trUtf8("Удалить элемент"));

    QAction * act = menu.exec(event->screenPos());
    if (act ==setTagAction){
        QString tag = QInputDialog::getText(event->widget(),
                                            QObject::trUtf8("Введите текст"),
                                            QObject::trUtf8("Новый тэг элемента:"),
                                            QLineEdit::Normal, textItem->getETag());
        if (!tag.isEmpty()){
            m_undoStack->push( new CommandTextItemChangeTag( this, textItem,tag ) );
            textItem->setEText(QObject::trUtf8("Новый..."));
            textItem->setPlainText(tag);
        }
    }
    if (act == delElemAction){
        if ( textItem != 0 ){
            m_undoStack->push( new CommandTextItemDelete( this, textItem ) );
        }
        if ( picItem != 0 ){
            this->removeItem(picItem);
        }
    }
    if (act == rotateRightAction){
        angle = -90.0;
        m_undoStack->push( new CommandTextItemRotate( this, textItem,angle ) );

    }
    if (act == rotateLeftAction){
        angle = 90.0;
        m_undoStack->push( new CommandTextItemRotate( this, textItem,angle) );

    }
    if (act == changeFontAction ){
        bool ok;

        QFont font = QFontDialog::getFont( &ok, QFont("Times", 14,QFont::Bold),0,
                                           QObject::trUtf8("Выберите шрифт для элемента "));
        if (ok) {// пользователь нажимает OK, и шрифт устанавливается в выбранный
            //currentFont = font;
            m_undoStack->push( new CommandTextItemChangeFont( this, textItem,font ) );
        }
    }
    if (act == changeColorAction ){
        QColor col ;
#if QT_VERSION >= 0x040500
        col = QColorDialog::getColor ( Qt::white,0,QObject::trUtf8("Выберите цвет текущего элемента ") ) ;
#else

#endif


        if (col.isValid()) {// пользователь нажимает OK, и цвет устанавливается в выбранный
            m_undoStack->push( new CommandTextItemChangeColor( this, textItem,col ) );
        }
    }


}

void  myScene::setViewMode()
{
    m_mode = (m_mode == true ) ? m_mode = false : m_mode = true;

    ///Пройдем по каждому элементу сцены и изменим отображаемый элемент
    QList<QGraphicsItem *> mTxtItem;
    mTxtItem = this->items();
    foreach (QGraphicsItem *item, mTxtItem){
        if ( dynamic_cast<myTextItem*>( item ) ){
            dynamic_cast<myTextItem*>( item )->setMode(m_mode);
        }
    }
}



