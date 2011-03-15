#include "printtask.h"
#include <QtCore/QDebug>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <QVariant>

PrintTask::PrintTask(QObject *parent)
        : QObject(parent)
        , m_docName( QString() )
        , m_MB( QString() )
        , m_printerQueue( QString() )
        , m_pageCount(0)

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

void PrintTask::setMB( const QString &s)
{
    if (s.isEmpty()){
        return;
    }
    m_MB = s;
}

void PrintTask::setPageCount    ( int cnt)
{

    m_pageCount = cnt;

}

void PrintTask::addFileToPrintQueue ( const QString &s,int pCopies)
{
    if (s.isEmpty()){
        return;
    }
    m_queueFiles2Print.enqueue(s);
    m_PrintDocCopies.enqueue(pCopies);
    // Вычислим размер файла
    m_FileSizes.enqueue( getFileRealSize(s) );
}

void PrintTask::setDocCopies( QList <int> &copies )
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

int PrintTask::getPagesInDocCount()
{
    int p(1);
    if (!m_PrintDocCopies.isEmpty()){
        p= m_PrintDocCopies.dequeue();
    }
    return p;
}

qint32 PrintTask::getCurrentFileSizes()
{
    qint32 p(0);
    if (!m_FileSizes.isEmpty()){
        p= m_FileSizes.dequeue();
    }
    return p;
}

bool PrintTask::isNextFileToPrint()
{
    return !m_queueFiles2Print.isEmpty();
}

void PrintTask::clearQueue()
{
    m_queueFiles2Print.clear();
}

qint32 PrintTask::getFileRealSize(const QString &fileName)
{
    QVariant max_fsize(0);

    QFile file_in(fileName);
    if (!file_in.open(QIODevice::ReadOnly) ){
        return 0;
    }
    file_in.close();
    max_fsize = file_in.size();
    bool ok;
    qint32 fsize = max_fsize.toInt(&ok);
    if (ok){
        return fsize;
    }
    return 0;
}
