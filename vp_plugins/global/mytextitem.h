#ifndef MYTEXTITEM_H
#define MYTEXTITEM_H

#include <QGraphicsTextItem>
#include <QtGlobal>

class myTextItem : public QGraphicsTextItem
{
public:
    QString getTag()const {return tag;};
    void setTag(const QString &t);
    void setAngle(qreal Angle);
    qreal getAngle(){return m_Angle;};
private:
    QString tag;
    qreal m_Angle;
};

#endif // MYTEXTITEM_H
