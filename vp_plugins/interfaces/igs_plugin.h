#ifndef IGS_PLUGIN_H
#define IGS_PLUGIN_H

#include <QString>
#include <QPixmap>

typedef enum{
    gsPluginLoaded,
    converted,
    merged,
    printed
}State;

class Igs_plugin{

public:
    virtual ~Igs_plugin() {}
    virtual void init(const QString &gs_bin,const QString &grep_bin,const QString &temp_folder,const QString &sid)=0;
    virtual QString getFirstPages()=0;
    virtual QString getOtherPages()=0;
    virtual void convertPs2Pdf(QString &input_fn)=0;
    virtual int pageCount(QString &input_fn)=0;
    virtual QPixmap convertPdf2Png(QString &fn, int beginPage,int lastPage, int Width )=0;
    virtual void merge2Pdf(QString &input_fn,QString &background_fn)=0;
    virtual void printPdf(QString &print_fn,QString &printer_name)=0;
};


QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(Igs_plugin,"com.technoserv.Plugin.Igs_plugin/1.0");
QT_END_NAMESPACE


#endif
