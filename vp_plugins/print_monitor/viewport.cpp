#include "viewport.h"
#include <qmath.h>

#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsPixmapItem>
#include <QtGui/QGraphicsTextItem>
#include <QtGui/QPixmap>

#include <QtGui/QHBoxLayout>


ViewPort::ViewPort(QWidget *parent)
    :QFrame(parent)
    ,graphicsView(0)
    ,m_thumb(false)
{
    setFrameStyle(Sunken | StyledPanel);

    graphicsView = new QGraphicsView;
    scene = new QGraphicsScene();
    scene->setBackgroundBrush(QBrush(Qt::gray));
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    graphicsView->setRenderHint(QPainter::Antialiasing, false);
    graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    graphicsView->setScene(scene);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(graphicsView);
    //topLayout->addLayout(zoomSliderLayout, 1, 1);
    setLayout(topLayout);

    connect (scene,SIGNAL(selectionChanged()),
             this,SLOT(onSceneSecletionChnaged()));

    setupMatrix(250);
}

void ViewPort::showThumbNail(bool thumb,QSize t_size)
{
    thumb_size = t_size;
    m_thumb = thumb;
    //resize(thumb_size.width()+20,600);
}

void ViewPort::setPixMapList(PixmapList pList)
{
    m_pixmap_list = pList;
    showPage(0); // Рисуем первую страницу
}

void ViewPort::setDescList(PixmapDescList descImagesList)
{
    m_desc_list = descImagesList;
}

//------------------------------ PUBLIC SLOTS ----------------------------------
void ViewPort::showPage(int p_number)
{
    curPixmapNumber = p_number;
    scene->clear();
    qreal xPos(5);
    qreal yPos(5);
    if (m_thumb){
        // Режим наготков :) рисуем все а стр с номером p_number выделяем
        for (int i=0;i<m_pixmap_list.size();i++){
            if (i == curPixmapNumber){
                yPos = addPairItem(xPos,yPos,i,true);
            }else{
                yPos = addPairItem(xPos,yPos,i,false);
            }
        }
    }else{
        // Рисуем одну страницу на весь экран
        if (m_pixmap_list.contains(curPixmapNumber)){
            yPos = addPairItem(xPos,yPos,curPixmapNumber,false);
        }
    }
}

void ViewPort::setupMatrix(int zoom)
{
    qreal scale = qPow(qreal(2), (zoom - 250) / qreal(50));
    QMatrix matrix;
    matrix.scale(scale, scale);
    graphicsView->setMatrix(matrix);
}

void ViewPort::zoomToFullSize()
{
    setupMatrix(250);
    graphicsView->ensureVisible(QRectF(0, 0, 0, 0));
}

void ViewPort::zoomToFit()
{
    if (!m_thumb){
        QGraphicsPixmapItem *pix_item;
        // Поиск графического объекта рисующего элемент с номером = curPixmapNumber
        for (int i=0;i<scene->items().size();i++){
             pix_item = (QGraphicsPixmapItem *)scene->items().at(i);
             if (pix_item &&
                 pix_item->data(ObjectData).toInt() == curPixmapNumber
                 ){
                 graphicsView->fitInView(scene->items().at(i),Qt::KeepAspectRatio);
                 break;
             }
        }
    }
}
//---------------------------- PRIVATE SLOTS ---------------------------------
void ViewPort::onSceneSecletionChnaged()
{
    QGraphicsPixmapItem *pix_item;
    if (m_thumb){
        QList<QGraphicsItem *> sel_list = scene->selectedItems();
        for (int i = 0; i < sel_list.size(); ++i) {
            pix_item = (QGraphicsPixmapItem *)sel_list.at(i);
            if (pix_item){
                emit selectPixmap(pix_item->data(ObjectData).toInt());
                break;
            }
        }
    }
}


//----------------------------- PRIVATE FUNCTION -------------------------------
qreal ViewPort::addPairItem (qreal xPos,qreal yPos,int p_number,bool selected)
{
    QFont font;
    font.setFamily(QString::fromUtf8("Times New Roman"));
    font.setPointSize(10);
    font.setBold(true);

    QGraphicsTextItem *text_item;
    QGraphicsPixmapItem *pix_item;
    qreal m_xPos = xPos;
    qreal m_yPos = yPos;
    pix_item   = new QGraphicsPixmapItem(m_pixmap_list.value(p_number));
    pix_item->setPos(m_xPos,m_yPos);
    pix_item->setData(ObjectData,p_number); //Сохраним номер страницы
    pix_item->setFlag(QGraphicsItem::ItemIsSelectable, true);
    pix_item->setSelected(selected);
    m_yPos +=pix_item->boundingRect().height()+5;

    if (m_thumb){

        text_item  = new QGraphicsTextItem();        
        text_item->setHtml( m_desc_list.value(p_number) );
        text_item->setFont(font);
        text_item->setPos(xPos,m_yPos);
        m_yPos +=text_item->boundingRect().height()+5;
        scene->addItem(text_item);
    }
    scene->addItem(pix_item);

    return m_yPos;
}

