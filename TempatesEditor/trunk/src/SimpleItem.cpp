#include "simpleitem.h"

SimpleItem::SimpleItem(QGraphicsItem * parent)
	:QGraphicsItem(parent)
{
    currentFont = QFont("Times", 10, QFont::Bold);
    //textList << QObject::trUtf8(" old Шаблон ") << QObject::trUtf8(" Шаблон 2 ");
    printFrame=false;
    currentColor =Qt::blue;
    
    changeFontAction = new QAction(QObject::trUtf8("Изменить шрифт"),0);
    changeFontAction->setStatusTip(QObject::trUtf8("Выбор нового шрифта для элемента шаблона"));
    connect(changeFontAction, SIGNAL(triggered()), this, SLOT(changeFont()));
    changeColorAction = new QAction(QObject::trUtf8("Изменить цвет"),0);
    changeColorAction->setStatusTip(QObject::trUtf8("Выбор нового цвета для элемента шаблона"));
    connect(changeColorAction, SIGNAL(triggered()), this, SLOT(changeColor()));
}

QRectF SimpleItem::boundingRect() const
{
		//QPointF ptPosition(pos().x()-nPenWidth,pos().y()-nPenWidth);
		QPointF ptPosition(0-nPenWidth,0-nPenWidth);

		QSize size_pt(nPenWidth*2,nPenWidth*2);
		//qDebug() <<size_pt ;
		size_pt+=calcSize();
		//qDebug() <<size_pt <<"\n";
		return QRectF(ptPosition,QSizeF(size_pt));
}

void SimpleItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
 {
     QMenu menu;
     menu.addAction(changeFontAction);
     menu.addAction(changeColorAction);
     //QAction *printFrameAction = menu.addAction("Печатать рамку");
     //QAction *printNoFrameAction = menu.addAction("Не печатать рамку");

     menu.exec(event->screenPos());

 }
void SimpleItem::paint (QPainter *ppainter,
				const QStyleOptionGraphicsItem *,
				QWidget *)
{
		ppainter->save();

		ppainter->setPen(QPen(Qt::black,nPenWidth,Qt::DotLine));
		//QPoint pt =pos().toPoint();
		QSize size_pt=calcSize();
		//qDebug() << Q_FUNC_INFO <<pt << size_pt <<"\n";

		ppainter->drawRect(0,0,size_pt.width(),size_pt.height());
		QFontMetrics fm(currentFont);
		int pixelsHigh=fm.height();
		ppainter->setPen(QPen(currentColor,0));
		ppainter->setFont(currentFont);
		for (int i = 0; i < textList.size(); ++i){
		    ppainter->drawText(0,0+((i+1)*pixelsHigh),textList.at(i).toLocal8Bit().constData());
		}
		ppainter->restore();
}
void SimpleItem::mousePressEvent(QGraphicsSceneMouseEvent *pe)
{
		QApplication::setOverrideCursor(Qt::PointingHandCursor);


		QGraphicsItem::mousePressEvent(pe);
}
void SimpleItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *pe)
{

		QApplication::restoreOverrideCursor();
		QGraphicsItem::mouseReleaseEvent(pe);
}

void SimpleItem::setText(QStringList &pList)
{
    textList.clear();
    textList.append(pList);
    update();
}

void SimpleItem::changeFont()
{
     bool ok;

    QFont font = QFontDialog::getFont( &ok, QFont("Times", 10,QFont::Bold),0,QObject::trUtf8("Выберите шрифт для элемента "));
    if (ok) {// пользователь нажимает OK, и шрифт устанавливается в выбранный
       currentFont =font;
       update();
    }

}
void SimpleItem::changeColor()
{
    QColor col = QColorDialog::getColor ( Qt::white,0,QObject::trUtf8("Выберите цвет текущего элемента ") ) ;
    if (col.isValid()) {// пользователь нажимает OK, и шрифт устанавливается в выбранный
       currentColor =col;
       update();
    }
}

void SimpleItem::setPrintFrame(bool frm)
{
    printFrame =frm;
}



QSize SimpleItem::calcSize() const
{
    QFontMetrics fm(currentFont);
    int pHigh=0;
    int maxPixelsWide=0; // Максимальная ширина строки
    //qDebug() << Q_FUNC_INFO <<  fm.height() <<"\n";

    for (int i = 0; i < textList.size(); ++i){
	int pixelsWide = fm.width(textList.at(i).toLocal8Bit().constData());
	if (pixelsWide >maxPixelsWide){
	    maxPixelsWide=pixelsWide;
	    //qDebug() << Q_FUNC_INFO <<textList.at(i).toLocal8Bit().constData() << maxPixelsWide <<"\n";
	}
	 pHigh += fm.height();
    }
    pHigh+= (fm.height()/2);
//qDebug() << Q_FUNC_INFO <<QSize(maxPixelsWide+nPenWidth*2,pHigh+nPenWidth*2);
    //return QSize(maxPixelsWide+5+nPenWidth*2,pHigh+nPenWidth*2);
    return QSize(maxPixelsWide,pHigh);
}

QStringList SimpleItem::getText()
{
    qDebug() << Q_FUNC_INFO << textList;
    return textList;
}
QFont SimpleItem::getFont()
{
    qDebug() << Q_FUNC_INFO <<currentFont;
    return currentFont;
}
QColor SimpleItem::getColor()
{
    qDebug() << Q_FUNC_INFO <<currentColor;
    return currentColor;
}

