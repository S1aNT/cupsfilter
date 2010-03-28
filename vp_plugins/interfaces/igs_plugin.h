#ifndef IGS_PLUGIN_H
#define IGS_PLUGIN_H
#include "mytypes.h"

using namespace VPrn;
class QString;


class Igs_plugin{

public:
    virtual ~Igs_plugin() {}
    virtual void init(const QString &gs_bin,const QString &pdftk_bin,const QString &temp_folder,const QString &sid)=0;
    virtual void convertPs2Pdf(const QString &client_uuid,const QString &input_fn) = 0;
};


QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(Igs_plugin,"com.technoserv.Plugin.Igs_plugin/2.0");
QT_END_NAMESPACE


#endif
