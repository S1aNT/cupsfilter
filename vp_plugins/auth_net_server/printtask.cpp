#include "printtask.h"
#include <QDebug>
#include <QRegExp>
#include <QStringList>

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

    QStringList list = s.split(";:;");
    if (list.size() !=0 ){
        // пришло в формате dns_name.printer_name
        m_printerQueue = list.last();
        qDebug() << "m_printerQueue: " << m_printerQueue <<"\n";
    }
}

void PrintTask::setDocName( const QString &s)
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
    m_queueFiles2Print.enqueue(s);
}
void PrintTask::setDocCopies    ( QList <int> &copies )
{
    m_doc_copyes =copies;
}

QString PrintTask::getFileToPrint()
{
    QString str;
    if (!m_queueFiles2Print.isEmpty()){
        str= m_queueFiles2Print.dequeue();
    }
    return str;
}
