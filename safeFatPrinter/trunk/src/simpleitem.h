#ifndef SIMPLEITEM_H
#define SIMPLEITEM_H
#include <QDebug>
#include <QApplication>
#include <QGraphicsRectItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QPen>
#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <QFontDialog>
#include <QColorDialog>
#include <QStringList>
#include <QMenu>
#include <QAction>
#include <QPoint>
#include <QDrag>
#include <QMimeData>

QT_BEGIN_NAMESPACE
class QFocusEvent;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneContextMenuEvent;
QT_END_NAMESPACE


class SimpleItem: public QObject,public QGraphicsItem
{
    Q_OBJECT
public:
	SimpleItem( QGraphicsItem * parent = 0);

	QSize calcSize() const;

	virtual QRectF boundingRect() const;
	virtual void paint (QPainter *ppainter,const QStyleOptionGraphicsItem *,QWidget *);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *pe);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *pe);
	QStringList getText();
	QFont getFont();
	QColor getColor();


public  slots:
	void setText(QStringList &pList);
	void changeFont();
	void changeColor();
	void setPrintFrame(bool frm);

protected:
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
	enum{nPenWidth=1};
	QStringList textList; // Список строк рисуемый в объекте
	bool printFrame; // Печатать рамку или нет
	QFont currentFont;
	QColor currentColor;
	QAction *changeFontAction;
	QAction *changeColorAction;
};
#endif
