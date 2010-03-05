#ifndef MYTEXTITEM_H
#define MYTEXTITEM_H

#include <QGraphicsTextItem>
#include <QtGlobal>

class myTextItem : public QGraphicsTextItem
{
public:
    QString getTag()  const {return tag;};
    qreal   getAngle()const {return m_Angle;};

    void    setTag   (const QString &t);
    void    setAngle (qreal Angle);

private:
    QString tag;
    qreal m_Angle;
};

#endif // MYTEXTITEM_H
