#ifndef MYTEXTITEM_H
#define MYTEXTITEM_H

#include <QGraphicsTextItem>

class QFont;
class myTextItem : public QGraphicsTextItem
{
public:
    myTextItem (QGraphicsTextItem * parent = 0);

    QString getTag()const {return tag;};
    void setTag(const QString &t){tag=t;};
private:
    QString tag;

};

#endif // MYTEXTITEM_H
