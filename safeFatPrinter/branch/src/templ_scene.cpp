#include <QGraphicsItem>
#include <QtCore>

#include "templ_scene.h"

templ_scene::templ_scene(QObject *parent) : QGraphicsScene(parent)
{

}

void templ_scene::setPageSize(qreal width,qreal height)
{
    page_width =  MM_TO_POINT(width);
    page_height=  MM_TO_POINT(height);
    this->setSceneRect(0,0,page_width,page_height);
}

void templ_scene::compilTemplates(int RowId)
{
	//if (pModel->
	//pModel->toFirst();
/*
	for (int i=0;i<RowId;i++){
	    this->pModel->toNext();
	}
*/

}
void templ_scene::load_from_file(QString &file_name)
{
    QFile file;
    qreal w, h;
    int elem_count;
    int elem_type;
    QPointF ps;
    QFont fnt;
    QColor col;
    QString elem_name;

    QStringList pList;
    QPixmap  picture;

    if (file.exists(file_name)){
	file.open(QIODevice::ReadOnly);
	QDataStream in(&file);
	in.setVersion(QDataStream::Qt_4_5);
	in >> page_orient;
	in >> w >> h;
	this->setPageSize(w,h);
	// читаем и создаем  основной лист
	QGraphicsRectItem *paper_rect = new QGraphicsRectItem (QRectF(0,0, this->page_width,this->page_height));
	paper_rect->setPen(QPen(Qt::black));
	paper_rect->setBrush(QBrush(Qt::white));
	paper_rect->setZValue(-1000.0);
	paper_rect->setData(ObjectName, "Paper");
	this->addItem(paper_rect);

	in >> elem_count; // Лицевая сторона
	for (int i=0;i<elem_count;i++){
	    in >> elem_type;
	    switch (elem_type){
		case 0:
		    // элемент шаблона имеющий шрифт, цвет,позицию
		    pList.clear();
		    in >>elem_name>>ps >>fnt >>col >>pList;
		    SimpleItem *pItem = new SimpleItem();
		    pItem->setPos(ps);
		    pItem->setText(pList);
		    pItem->setData(ObjectName, elem_name);
		    pItem->setFlag(QGraphicsItem::ItemIsMovable);
		    front_side_page.insert(i,pItem);
		    break;
		case 1:

		    in >> ps >>picture;
		    QGraphicsPixmapItem *gItem = new QGraphicsPixmapItem();
		    gItem->setPos(ps);
		    gItem->setPixmap(picture);
		    gItem->setData(ObjectName, "Graph_Item");
		    gItem->setFlag(QGraphicsItem::ItemIsMovable);
		     front_side_page.insert(i,gItem);
		    break;
	    }
	}

	in >> elem_count;

	for (int i=0;i<elem_count;i++){
	    in >> elem_type;
	    switch (elem_type){
		case 0:
		    // элемент шаблона имеющий шрифт, цвет,позицию
		    pList.clear();
		    in >>ps >>fnt >>col >>pList;
		    SimpleItem *pItem = new SimpleItem();
		    pItem->setPos(ps);
		    pItem->setText(pList);
		    pItem->setData(ObjectName, "Simple");
		    pItem->setFlag(QGraphicsItem::ItemIsMovable);
		     front_side_page.insert(i,pItem);
		    break;
		case 1:

		    in >> ps >>picture;
		    QGraphicsPixmapItem *gItem = new QGraphicsPixmapItem();
		    gItem->setPos(ps);
		    gItem->setPixmap(picture);
		    gItem->setData(ObjectName, "Graph_Item");
		    gItem->setFlag(QGraphicsItem::ItemIsMovable);
		     front_side_page.insert(i,gItem);
		    break;
	    }
	}

	file.close();
    }
}

void templ_scene::setModel (QStandardItemModel * model)
{
    //pModel=model;
  //  pModel->toLast();
}
