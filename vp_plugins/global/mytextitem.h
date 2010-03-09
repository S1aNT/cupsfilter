#ifndef MYTEXTITEM_H
#define MYTEXTITEM_H

#include <QGraphicsTextItem>
#include <QtGlobal>

class myTextItem : public QGraphicsTextItem
{
public:
    myTextItem (QGraphicsItem * parent = 0 );
    QString getETag()   const {return m_tag;};
    QString getEText()  const {return m_text;};
    qreal   getAngle()  const {return m_Angle;};

    void    setMode  (bool tagTextMode);
    void    setETag  (const QString &t);
    void    setEText (const QString &t);
    void    setAngle (qreal Angle);

private:
    QString m_tag;
    QString m_text;
    bool    v_mode; //Режим отображения Tag/Text
    qreal m_Angle;
};

#endif // MYTEXTITEM_H
