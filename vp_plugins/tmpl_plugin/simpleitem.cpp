#include <QGraphicsRectItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QAction>
#include <QDebug>
//#include <QApplication>
#include <QPen>
#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <QFontDialog>
#include <QColorDialog>
#include <QStringList>
#include <QMenu>

#include <QPoint>
#include <QDrag>
#include <QDir>
#include <QMimeData>
#include <QInputDialog>

#include "simpleitem.h"

SimpleItem::SimpleItem(QGraphicsItem * parent)
    :QGraphicsItem(parent)
{
    currentFont = QFont("Times", 10, QFont::Bold);
    //textList << QObject::trUtf8(" old Шаблон ") << QObject::trUtf8(" Шаблон 2 ");
    printFrame=false;
    currentColor =Qt::black;

    changeFontAction = new QAction(QObject::trUtf8("Изменить шрифт"),0);
    changeFontAction->setStatusTip(QObject::trUtf8("Выбор нового шрифта для элемента шаблона"));
    connect(changeFontAction, SIGNAL(triggered()), this, SLOT(changeFont()));
    changeColorAction = new QAction(QObject::trUtf8("Изменить цвет"),0);
    changeColorAction->setStatusTip(QObject::trUtf8("Выбор нового цвета для элемента шаблона"));
    connect(changeColorAction, SIGNAL(triggered()), this, SLOT(changeColor()));
    rotateRightAction = new QAction (QObject::trUtf8("Вращать по часовой стрелке"),0);
    connect(rotateRightAction,SIGNAL(triggered()),this,SLOT(rotateRight()));
    rotateLeftAction = new QAction (QObject::trUtf8("Вращать против часовой стрелки"),0);
    connect(rotateLeftAction,SIGNAL(triggered()),this,SLOT(rotateLeft()));
    setTextAction = new QAction (QObject::trUtf8("Ввести произвольный текст"),0);
    connect (setTextAction,SIGNAL(triggered()),this,SLOT(setTextDlg()));
    delElemAction = new QAction (QObject::trUtf8("Удалить элемент"),0);
    connect (delElemAction,SIGNAL(triggered()),this,SLOT(delElement()));
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
    menu.addSeparator();
    menu.addAction(rotateRightAction);
    menu.addAction(rotateLeftAction);
    menu.addAction(setTextAction);
    menu.addSeparator();
    menu.addAction(delElemAction);
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

void SimpleItem::delElement()
{
    this->hide();
    this->deleteLater();
}

void SimpleItem::setTextDlg()
{
    QString source;
    bool ok = false;
    source = QInputDialog::getText(0, tr("Введите текст"), tr("Поле ввода текстовых данных:"), QLineEdit::Normal, source, &ok);

    if (ok && !source.isEmpty())
        this->setText(QStringList()<<source.split("\n"));
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


void SimpleItem::rotateRight()
{
    this->rotate(89.9);
}

void SimpleItem::rotateLeft()
{
    this->rotate(-89.9);
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

void SimpleItem::setColor(const QColor & col)
{
    this->currentColor = col;
}

void SimpleItem::setFont(const QFont &fnt)
{
    this->currentFont = fnt;

}

