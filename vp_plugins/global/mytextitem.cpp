#include "mytextitem.h"
#include <QtGui/QMatrix>
#include <QtGui/QFont>
#include <QtGui/QColor>

#include <QtCore/QString>
#include <QtCore/QPointF>


myTextItem::myTextItem (QGraphicsItem * parent)
    : QGraphicsTextItem (parent)
{

}

myTextItem::myTextItem(const QString &tag, const QString &text,
                       const QPointF &ps,  const QFont &fnt,
                       const QColor &col,  bool v_mode)
                           : m_tag  ( QString("[XXX]") )
                           , m_text ( QString() )
                           , m_mode (false)

{
    v_mode = m_mode;

    if (text.isEmpty()){
        this->setEText(tag);
    }else{
        this->setEText(text);
    }

    if (v_mode){
        this->setPlainText(tag);
    }else{
        this->setPlainText(text);
    }

    if (ps.isNull()){
        this->setPos( QPointF(100.0,100.0) );
    }else{
        this->setPos(ps);
    }

    this->setFont(fnt);

    //    if (fnt){
    //        this->setFont(fnt);
    //    }else{
    //        this->setFont( QFont("Times", 14, QFont::Bold) ) ;
    //    }
    if( col.isValid() ){
        this->setDefaultTextColor( col );
    }else{
        this->setDefaultTextColor(  Qt::black );
    }




    this->setETag(tag);
    this->setZValue(100);
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemIsFocusable, true);
    this->setData(ObjectName, "tElem");
}

void myTextItem::setAngle(qreal Angle)
{
    m_Angle = Angle;
    this->rotate(m_Angle);

//    QMatrix matrix;
//    matrix.rotate(m_Angle);
//    this->setMatrix(matrix);
}

void myTextItem::setETag(const QString &t)
{
    m_tag = t;
}

void myTextItem::setEText (const QString &t)
{
    m_text = t;
}

void myTextItem::setMode  (bool tagTextMode)
{
    m_mode = tagTextMode;
    if (m_mode){
        // Показваем Tag default
        this->setPlainText(m_tag);
    }else{
        this->setPlainText(m_text);
    }
}

