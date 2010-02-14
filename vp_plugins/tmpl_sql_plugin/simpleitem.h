#ifndef SIMPLEITEM_H
#define SIMPLEITEM_H
#include <QObject>
#include <QGraphicsItem>


QT_BEGIN_NAMESPACE
class QFocusEvent;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneContextMenuEvent;
class QSize;
class QStringList;
class QColor;
class QFont;
class QAction;
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

        QString getTag() const {return tag;};
        void setTag(const QString &t){tag = t;};

        QFont getFont();
        QColor getColor();
        void setColor(const QColor & col);
        void setFont(const QFont &fnt);

public  slots:
        void setText(QStringList &pList);
        void changeFont();
        void changeColor();
        void setPrintFrame(bool frm);
        void rotateLeft();
        void rotateRight();
        void setTextDlg();
        void delElement();
protected:
        void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
        enum{nPenWidth=1};
        QStringList textList; // Список строк рисуемый в объекте
        QString tag;          // Тег который присвоен элементу
        
        bool printFrame; // Печатать рамку или нет
        QFont currentFont;
        QColor currentColor;
        QAction *changeFontAction;
        QAction *changeColorAction;
        QAction *rotateRightAction;
        QAction *rotateLeftAction;
        QAction *setTextAction;
        QAction *delElemAction;
};
#endif
