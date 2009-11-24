#ifndef IGS_PLUGIN_H
#define IGS_PLUGIN_H

#include <QString>
#include <QPixmap>
#include <QStringList>


class Igs_plugin{

public:
    virtual ~Igs_plugin() {}
    virtual bool init(const QString &gs_bin,const QString &pdftk_bin,const QString &temp_folder,const QString &gs_rcp_file,const QString &sid)=0;
    virtual QString getFirstPages()=0;
    virtual QString getOtherPages()=0;
    virtual void convertPs2Pdf(const QString &input_fn)=0;
    virtual void getPageCount(const QString &input_fn)=0;
    virtual QPixmap convertPdf2Png(const QString &fn, int beginPage, int Width )=0;
    virtual void merge2Pdf(const QString &input_fn,const QString &background_fn,const QString &output_fn)=0;
    virtual void printPdf(const QString &print_fn,const QString &printer_name)=0;
    virtual void addPdfMark(const QString &input_fn, const QString &user_name, const QString &host_name,quint16 host_ip)=0;
};


QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(Igs_plugin,"com.technoserv.Plugin.Igs_plugin/1.0");
QT_END_NAMESPACE


#endif
