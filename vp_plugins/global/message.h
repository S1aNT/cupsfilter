#ifndef MESSAGE_H
#define MESSAGE_H
/**
  * @class Messeage ƒанный класс инкапсулирует в себе сообщение передаваемое от
  * клиента к серверу и обратно
  */
#include <QObject>

class Message : public QObject
{
Q_OBJECT
public:
    explicit Message(QObject *parent = 0);

signals:

public slots:

};

#endif // MESSAGE_H
