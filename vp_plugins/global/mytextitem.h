#ifndef MYTEXTITEM_H
#define MYTEXTITEM_H

#include <QGraphicsTextItem>
#include <QtGlobal>

#include "mytypes.h"
using namespace VPrn;

QT_FORWARD_DECLARE_CLASS(QString)
QT_FORWARD_DECLARE_CLASS(QPointF)
QT_FORWARD_DECLARE_CLASS(QFont)
QT_FORWARD_DECLARE_CLASS(QColor)


class myTextItem : public QGraphicsTextItem
{
public:
    myTextItem (QGraphicsItem * parent = 0 );

    myTextItem (const QString &tag, const QString &text,
                const QPointF &ps,  const QFont &fnt,
                const QColor &col,  bool v_mode);


    QString getETag()   const {return m_tag;}
    QString getEText()  const {return m_text;}
    qreal   getAngle()  const {return m_Angle;}

    void    setMode  (bool tagTextMode);
    void    setETag  (const QString &t);
    void    setEText (const QString &t);
    void    setAngle (qreal Angle);

private:
    QString m_tag;
    QString m_text;
    bool    m_mode; //Режим отображения Tag/Text
    qreal   m_Angle;
};




#endif // MYTEXTITEM_H
