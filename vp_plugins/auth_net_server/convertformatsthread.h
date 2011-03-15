#ifndef CONVERTFORMATSTHREAD_H
#define CONVERTFORMATSTHREAD_H

#include <QThread>
#include <QMutex>
#include <Qqueue>
#include <QWaitCondition>
#include "cnvtask.h"

class ConvertFormatsThread : public QThread
{
    Q_OBJECT

public:
    ConvertFormatsThread();
    ~ConvertFormatsThread();
    void addTask(CnvTask *m_task);

    CnvTask getFinishedTask();

protected:
    void run();
private:
    CnvTask currentTask;
    QWaitCondition taskAdded;
    QMutex mutex;
    QQueue <CnvTask *> tasksQueue;

};

#endif // CONVERTFORMATSTHREAD_H
