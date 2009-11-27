#ifndef GS_PLUGIN_H
#define GS_PLUGIN_H

#include <QObject>
#include <QDebug>
#include <QPixmap>
#include <QtPlugin>
#include <QStringList>
#include <QPainter>
#include <QFile>
#include <QDir>
#include <QRegExp>
#include <QTemporaryFile>
#include <QTextStream>
#include <QDateTime>

#include "proc_thread.h"
#include "igs_plugin.h"


class gs_plugin :public QObject, Igs_plugin
{
    Q_OBJECT
    Q_INTERFACES(Igs_plugin)
    Q_ENUMS(TaskState)
   // Q_ENUMS(ErrorCode)
public:
    enum TaskState {converted,merged,splitted_first,splitted_other,printed,previewedPage,pdfMarkAdded };
    //enum ErrorCode {forkProccessDie};
    gs_plugin(QObject *parent=0){}
    bool init(const QString &gs_bin,const QString &pdftk_bin,const QString &temp_folder,const QString &gs_rcp_file,const QString &sid);
    QString getFirstPages(){return firstPage_fn;};
    QString getOtherPages(){return otherPages_fn;};
    QPixmap getSnapShot(){return currentPageSnapShot;};
    void convertPs2Pdf(const QString &input_fn);
    void getPageCount(const QString &input_fn);
    void convertPdf2Png(const QString &fn, int convertedPage);
    void merge2Pdf(const QString &input_fn,const QString &background_fn,const QString &output_fn);
    void printPdf(const QString &print_fn,const QString &printer_name);
    void addPdfMark(const QString &input_fn,const QString &user_name, const QString &host_name,quint16 host_ip);
    void clearAll();
signals:
    void error(QString error_message);
    void StateChanged(TaskState state);

private slots:
    //TODO добавить для всех этих слотов сигнал маппер и объединить в одну функцию
    //TODO порождаемый поток держать в спящем состоянии и пробуждать при приходе новой команды
    // а не создовать каждый раз заново.
    void parseCnvThread(int Code,QString output);
    void parsePageCountThread(int Code,QString output);
    void parseFirstPageThread(int Code,QString output);
    void parseOtherPageThread(int Code,QString output);
    void parseMergeThread(int Code,QString output);
    void parseAddPdfMarkThread(int Code,QString output);
    void parseCnv2PngThread(int Code,QString output);
private:
    QString gsBin;
    QString tempPath;
    QString pdftkBin;
    QStringList args;
    QString Sid;
    QString firstPage_fn;
    QString otherPages_fn;
    QString mainPDF;
    QString pdf2png_page;
    QString gs_rcp;

    QPixmap currentPageSnapShot;
    int pagesCount; // Число страниц в конвертируемом документе
protected:
    void splitPdf(const QString &source_fn,const QString &firts_page_fn,const QString &other_page_fn);
};


#endif
