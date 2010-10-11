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
    void setMB      ( const QString &s) ;    
    void addFileToPrintQueue  ( const QString &s,int pCopies);
    void setDocCopies    ( QList <int> &copies );
    void setPageCount    ( int cnt);
    QString getDocName()      const { return m_docName; }
    QString getMB()      const { return m_MB; }
    QString getPrinterQueue() const { return m_printerQueue; }
    int     getPagesInDocCount();
    int     getPageCount() {return m_pageCount;}

    qint64  getCurrentFileSizes();
    QString getFileToPrint();
    QList <int> getDocCopies()      { return m_doc_copyes;}
    void clearQueue();
    bool isNextFileToPrint();
private:
    QString m_docName;      // Название документа   
    QString m_MB;      // MB документа
    QString m_printerQueue; // Имя принтера (имя очереди для CUPS сервера)    
    int     m_pageCount;    // Кол-во стр в документе
    QQueue <int>     m_PrintDocCopies;// Кол-во печатемых копий для каждого файла входящего в состав документа
    QQueue <qint64> m_FileSizes;    // Размеры в байтах для каждого файла входящего в состав документа
    QQueue <QString> m_queueFiles2Print; // Очередь файлов для печати
    QList <int>      m_doc_copyes; // Копии документа (1-5 получатели)
    //-------------------------------------------------------------------------
    qint64 getFileRealSize(const QString &fileName);
};


#endif // PRINTTASK_H
