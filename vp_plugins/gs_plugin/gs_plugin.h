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

#include "igs_plugin.h"


class gs_plugin :public QObject, Igs_plugin
{
    Q_OBJECT
    Q_INTERFACES(Igs_plugin)
public:
    gs_plugin(QObject *parent=0){}
    void init(const QString &gs_bin,const QString &grep_bin,const QString &temp_folder,const QString &sid);
    QString getFirstPages(){return firstPage_fn;};
    QString getOtherPages(){return otherPages_fn;};
    void convertPs2Pdf(QString &input_fn);
    int pageCount(QString &input_fn);
    QPixmap convertPdf2Png(QString &fn, int beginPage,int lastPage, int Width);
    void merge2Pdf(QString &input_fn,QString &background_fn);
    void printPdf(QString &print_fn,QString &printer_name);

signals:
    void error(const QString &error_message);
    void StateChanged(State state);

private:

    QString gsBin;
    QString tempPath;
    QString grepBin;
    QStringList args;
    QString Sid;
    QString firstPage_fn;
    QString otherPages_fn;
    QPixmap pageSnapShot;
    int PageCountInDoc; // Число страниц в конвертируемом документе
    int callGS(const QStringList &gs_args);
};


#endif
