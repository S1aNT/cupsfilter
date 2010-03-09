#include "mytextitem.h"
#include <QtGui/QMatrix>


myTextItem::myTextItem (QGraphicsItem * parent)
    : QGraphicsTextItem (parent)
{

}
void myTextItem::setAngle(qreal Angle)
{
    m_Angle = Angle;
    QMatrix matrix;
    matrix.rotate(m_Angle);
    this->setMatrix(matrix);
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
    v_mode = tagTextMode;
    if (v_mode){
        // Показваем Tag default
        this->setPlainText(m_tag);
    }else{
        this->setPlainText(m_text);
    }
}
