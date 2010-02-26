#include "mytextitem.h"
#include <QFont>

myTextItem::myTextItem(QGraphicsTextItem * parent)
        :QGraphicsTextItem(parent)
{
    // Первоначальная настройка
    this->setFont(QFont("Times", 10, QFont::Bold));    
}

