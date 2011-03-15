#include "convertformatsthread.h"

ConvertFormatsThread::ConvertFormatsThread()
{
    start();
}

ConvertFormatsThread::~ConvertFormatsThread()
{
    {
        QMutexLocker locker(&mutex);
        while ( tasksQueue.isEmpty() ) {
            delete tasksQueue.dequeue();
        }
        tasksQueue.enqueue(EndTask);
        taskAdded.wakeOne();
    }
    wait();
}

void ConvertFormatsThread::addTask(CnvTask *m_task)
{
    QMutexLocker locker(&mutex);
    tasksQueue.enqueue(m_task);
    taskAdded.wakeOne();
}

CnvTask ConvertFormatsThread::getFinishedTask()
{
    QMutexLocker locker(&mutex);
    return currentTask;
}

//------------------------ Protected ---------------------------------------------------

void ConvertFormatsThread::run()
{
}
