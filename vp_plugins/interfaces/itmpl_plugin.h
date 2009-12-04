#ifndef ITMPL_PLUGIN_H
#define ITMPL_PLUGIN_H

#include <QString>
#include <QStandardItemModel>
#include <QGraphicsScene>

//! [0]
class Itmpl_plugin{

public:
    virtual ~Itmpl_plugin() {}
    virtual void init(const QString & spool,const QString & sid,const QStandardItemModel * model)=0;
    virtual void setTemplates(const QString & templates_in_file)=0;
    virtual void printFormatingPageToFile(int pageNum)=0;
    virtual QGraphicsScene *getFirstPage()=0;
    virtual QGraphicsScene *getSecondPage()=0;
    virtual QGraphicsScene *getThirdPage()=0;
    virtual QGraphicsScene *getFourthPage()=0;
};


QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(Itmpl_plugin,"com.technoserv.Plugin.Itmpl_plugin/1.0");
QT_END_NAMESPACE

//! [0]
#endif
