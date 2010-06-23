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

/*
void myScene::drawForeground  ( QPainter * painter, const QRectF & rect )
{
    int gridInterval = 50;
    //interval to draw grid lines at
    painter->setWorldMatrixEnabled(true);

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
}
*/

void  myScene::createPage(qreal width,qreal height,qreal m_top,qreal m_bottom,
                          qreal m_right,qreal m_left)
{
    QPixmap pixmap;
    QPixmap pixmap2;

    this->clear();
    this->setSceneRect(0, 0, width,height);
    //this->setBackgroundBrush(Qt::white);
    this->setBackgroundBrush(Qt::transparent);
/*
    this->setBackgroundBrush(Qt::transparent);
    pixmap = QPixmap(width,height);
    pixmap.fill(Qt::transparent);

    QGraphicsPixmapItem *paper_rect =
            new QGraphicsPixmapItem();
    paper_rect->setPixmap(pixmap);
    paper_rect->setData(ObjectName, "Paper");
    paper_rect->setZValue(-1000.0);
    paper_rect->setShapeMode(QGraphicsPixmapItem::HeuristicMaskShape);
    this->addItem(paper_rect);

    pixmap2 = QPixmap(width,height);
    pixmap2.fill(Qt::white);
    QPainter painter(&pixmap2);
    //painter.setBrush(Qt::black);
    //Рисуем сетку
    int gridInterval = 100; //interval to draw grid lines at
    painter.setWorldMatrixEnabled(true);
    QRectF  rect = this->sceneRect();

    qreal left = int(rect.left()) - (int(rect.left()) % gridInterval );
    qreal top = int(rect.top()) - (int(rect.top()) % gridInterval );

    QVarLengthArray <QLineF, 100 > linesX;
    for (qreal x = left; x < rect.right(); x += gridInterval )
        linesX.append(QLineF(x, rect.top(), x, rect.bottom()));

    QVarLengthArray <QLineF, 100 > linesY;
    for (qreal y = top; y < rect.bottom(); y += gridInterval )
        linesY.append(QLineF(rect.left(), y, rect.right(), y));

    //painter.drawRect(m_left, m_top,width-m_left-m_right,height-m_top-m_bottom);
    painter.drawLines(linesX.data(), linesX.size());
    painter.drawLines(linesY.data(), linesY.size());

    QGraphicsPixmapItem *border_rect = new QGraphicsPixmapItem();

    border_rect->setPixmap(pixmap2);
    border_rect->setData(ObjectName, "Border");
    border_rect->setZValue(-900.0);
    border_rect->setShapeMode(QGraphicsPixmapItem::HeuristicMaskShape);
    this->addItem(border_rect);

    */
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
    border_rect->setOpacity(1);
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
    //Item->setScaledSize(m_scaled);
    //Item->setAngle(m_angle);

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
    myTextItem*  textItem = dynamic_cast<myTextItem*>( itemAt( x, y ) );
    if ( textItem == 0 ){
        return;
    }else{
        qreal angle;
        QMenu menu;
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
            m_undoStack->push( new CommandTextItemDelete( this, textItem ) );

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
            QColor col = QColorDialog::getColor ( Qt::white,0,QObject::trUtf8("Выберите цвет текущего элемента ") ) ;
            if (col.isValid()) {// пользователь нажимает OK, и цвет устанавливается в выбранный
                m_undoStack->push( new CommandTextItemChangeColor( this, textItem,col ) );
            }
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



