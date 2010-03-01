#include "mytextitem.h"
#include <QtGui/QMatrix>

void myTextItem::setAngle(qreal Angle)
{
    m_Angle = Angle;
    QMatrix matrix;
    matrix.rotate(m_Angle);
    this->setMatrix(matrix);
}

void myTextItem::setTag(const QString &t)
{
    tag = t;
    this->setPlainText(t);
}
