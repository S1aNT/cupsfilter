#ifndef PRINTTASK_H
#define PRINTTASK_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QQueue>

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
    explicit PrintTask   ( QObject *parent = 0 );

    void setPrinterQueue ( const QString &s) ;    
    void setDocName      ( const QString &s) ;   
    void setPageCount    ( int cnt)          ;
    void addFileToPrintQueue  ( const QString &s,int pCopies);
    void setDocCopies    ( QList <int> &copies );

    QString getDocName()      const { return m_docName; }   
    QString getPrinterQueue() const { return m_printerQueue; }
    int     getPagesInDocCount(){return m_pageCount;}
    int     getPageCount() ;
    QString getFileToPrint();
    QList <int> getDocCopies()      { return m_doc_copyes;}
    void clearQueue();
    bool isNextFileToPrint();
private:
    QString m_docName;      // Название документа   
    QString m_printerQueue; // Имя принтера (имя очереди для CUPS сервера)    
    int     m_pageCount;    // Кол-во стр в документе
    QQueue <int>     m_CalcPageCount;// Кол-во печатемых копий для каждого файла входящего в состав документа
    QQueue <QString> m_queueFiles2Print; // Очередь файлов для печати
    QList <int>      m_doc_copyes; // Копии документа (1-5 получатели)
};


#endif // PRINTTASK_H
