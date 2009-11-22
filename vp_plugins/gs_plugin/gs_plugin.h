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
#include <QProcess>
#include <QRegExp>

#include "igs_plugin.h"


class gs_plugin :public QObject, Igs_plugin
{
    Q_OBJECT
    Q_INTERFACES(Igs_plugin)
    Q_ENUMS(TaskState)

public:
    enum TaskState {converted,merged,splitted,printed };

    gs_plugin(QObject *parent=0){}
    bool init(const QString &gs_bin,const QString &pdftk_bin,const QString &temp_folder,const QString &sid);
    QString getFirstPages(){return firstPage_fn;};
    QString getOtherPages(){return otherPages_fn;};
    void convertPs2Pdf(const QString &input_fn);
    int pageCount(const QString &input_fn);
    QPixmap convertPdf2Png(const QString &fn, int beginPage,int Width);
    void merge2Pdf(const QString &input_fn,const QString &background_fn,const QString &output_fn);
    void printPdf(const QString &print_fn,const QString &printer_name);
    void addPdfMark(const QString &input_fn,const QStringList mark);

signals:
    void error(const QString &error_message);
    void StateChanged(TaskState state);

private:

    QString gsBin;
    QString tempPath;
    QString pdftkBin;
    QStringList args;
    QString Sid;
    QString firstPage_fn;
    QString otherPages_fn;
    QString mainPDF;

    QPixmap pageSnapShot;
    int pagesCount; // Число страниц в конвертируемом документе

    void splitPdf(const QString &source_fn,const QString &firts_page_fn,const QString &other_page_fn);
    QString startProc(const QString &bin,const QStringList &proc_args);

};


#endif
