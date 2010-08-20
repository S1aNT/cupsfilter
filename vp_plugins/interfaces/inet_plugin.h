#ifndef INET_PLUGIN_H
#define INET_PLUGIN_H

#include "mytypes.h"
//#include  "message.h"

using namespace VPrn;

class QString;
class Message;

//! [0]
class Inet_plugin{

public:
    virtual ~Inet_plugin() {}
    virtual void init(const QString &host,int port) = 0 ;
    virtual void sendMessage( const Message &m_msg) = 0;
    //virtual MyCheckPoints state() const = 0;

};


QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(Inet_plugin,"com.technoserv.Plugin.Inet_plugin/1.0");
QT_END_NAMESPACE

//! [0]
#endif
