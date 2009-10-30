#include "template_painter.h"
#include "simpleItem.h"
template_painter::template_painter()
{
    scene = new QGraphicsScene();

}
QGraphicsScene * template_painter::getScene()
{
    return scene;
}

void template_painter::read4filedraw2scene(QString &file_name,int page)
{
    QFile file(file_name);
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_5);
    in >> main_rect;
    scene->setSceneRect(main_rect);
    in >> page_orientation;
    // Читаем блок обратной стороны
    int block_type;
    int count_elem;
    int elem_type;
    in >> block_type;
    in >> count_elem;
    for (int i=0;i<count_elem;++i){
	in >> elem_type;
	switch (elem_type){
	    case 0:
		// Элемент моего типа
		SimpleItem* pItem = new SimpleItem;
		QPointF ps;
		QFont fnt;
		QColor col;
		QStringList pList;
		in >>ps >>fnt >>col >>pList;
		pItem->setPos(ps);
		pItem->setText(pList);
		if (page==block_type){
		    scene->addItem(pItem);
		}
		break;
	}
    }
    // Читаем блок лицевой стороны
    // Читаем блок последней страницы
    file.close();
    //
}

void template_painter::setPaperSize(const QString & psize)
{

}


