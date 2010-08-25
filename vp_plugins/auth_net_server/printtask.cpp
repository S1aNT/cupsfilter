#include "printtask.h"

PrintTask::PrintTask(QObject *parent)
        :QObject(parent)
        ,m_docName( QString() )        
        ,m_printerQueue( QString() )
        ,m_pageCount(0)

{
    queueFiles2Print.clear();
}

void PrintTask::setPrinterQueue ( const QString &s)
{
    if (s.isEmpty()){
        return;
    }
    m_printerQueue = s;
}

void PrintTask::setDocName      ( const QString &s)
{
    if (s.isEmpty()){
        return;
    }
    m_docName = s;
}

void PrintTask::setPageCount    ( int cnt)
{
    m_pageCount = cnt;
}

void PrintTask::addFileToPrintQueue  ( const QString &s)
{
    if (s.isEmpty()){
        return;
    }
    queueFiles2Print.push(s);
}

