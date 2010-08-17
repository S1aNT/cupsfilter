#ifndef PRINTTASK_H
#define PRINTTASK_H

#include <QObject>
#include <QMap>
#include <QList>

/**
  * @class PrintTask
  * @brief Предназначен для записи информации о конкретном задании печати
  * которое характеризуется следующими параметрами:
  * @underline
  * @li QString docName;      // Название документа
  * @li QString mb;           // Номер МБ  
  * @li QString printerQueue; // Имя принтера (имя очереди для CUPS сервера)
  * @li int     overside_pageCount; // Кол-во печатаемых листов
*/

class PrintTask : public QObject
{
    Q_OBJECT
public:
    explicit PrintTask(QObject *parent = 0);    
    void setPrinterQueue ( const QString &s) { m_printerQueue = s;}
    void setJobId4Copy   ( const QString &s,int copy){m_jobId4copy[copy] = s;}
    void setCopyes       ( QList <int> cnt)  { m_copyes = cnt;}
    void setDocName      ( const QString &s) { m_docName = s;}
    void setMB           ( const QString &s) { m_mb = s;}
    void setPageCount    ( int cnt)          { m_pageCount = cnt; }

    QString getDocName()      const { return m_docName; }
    QString getMB()           const { return m_mb; }
    QString getPrinterQueue() const { return m_printerQueue; }
    int     getPageCount()          { return m_pageCount; }
    QList <int>  getCopyes()        { return m_copyes; }
    QString getJobId4Copy(int copy) { return m_jobId4copy.value(copy); }
private:
    QString m_docName;      // Название документа
    QString m_mb;           // Номер МБ    
    QString m_printerQueue; // Имя принтера (имя очереди для CUPS сервера)    
    int     m_pageCount;    // Кол-во стр в документе    
    QList  <int>         m_copyes;    // Число экз. документа
    QMap   <int,QString> m_jobId4copy;
};


#endif // PRINTTASK_H
