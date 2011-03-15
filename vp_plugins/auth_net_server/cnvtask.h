#ifndef CNVTASK_H
#define CNVTASK_H

#include <QObject>

class CnvTask : public QObject
{
    Q_OBJECT
public:
    CnvTask(QObject *parent = 0);
};

#endif // CNVTASK_H
