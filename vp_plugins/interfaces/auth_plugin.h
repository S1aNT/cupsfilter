#ifndef AUTHINTERFACE_H
#define AUTHINTERFACE_H

class QString;

//! [0]
class Auth_plugin{
public:
    virtual ~Auth_plugin() {}
    virtual void init (const QString &mandat_filename)=0;
    virtual void init ()=0;
    virtual QString getUserName() = 0;
    virtual QString getUserMandat() = 0;
};


QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(Auth_plugin,"com.technoserv.Plugin.Auth_plugin/1.0");
QT_END_NAMESPACE

//! [0]
#endif
