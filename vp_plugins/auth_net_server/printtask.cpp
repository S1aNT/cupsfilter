#include "printtask.h"

PrintTask::PrintTask(QObject *parent)
        :QObject(parent)
        ,m_docName( QString() )        
        ,m_printerQueue( QString() )
        ,m_pageCount(0)

{
    m_queueFiles2Print.clear();
    m_doc_copyes.clear();
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
    m_queueFiles2Print.push(s);
}
void PrintTask::setDocCopies    ( QList <int> &copies )
{
    m_doc_copyes =copies;
}
