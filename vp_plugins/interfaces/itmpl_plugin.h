#ifndef ITMPL_PLUGIN_H
#define ITMPL_PLUGIN_H

class QString;
class QStandardItemModel;
class QGraphicsScene;
class Templ_info;

#include <QObject>
//! [0]
class Itmpl_plugin{

public:
    virtual ~Itmpl_plugin() {}
    virtual void init(const QString & spool,const QString & sid)=0;
    virtual void loadTemplates(const QString & templates_in_file)=0;
    virtual void createEmptyTemplate(const QString & file_name,
                             const QString & t_author,
                             const QString & t_name,
                             const QString & t_desc,
                             const QString & p_size,

                             bool pages_orient,
                             const QString & c_date,
                             qreal m_top,
                             qreal m_bottom,
                             qreal m_right,
                             qreal m_left)=0;

    virtual void setTemplates(const QString & templates_in_file,QStandardItemModel * model)=0;
    virtual void convertTemplatesToPdf(const QString & templates_in_file,QStandardItemModel * model)=0;
    virtual void printFormatingPageToFile(int pageNum)=0;
    virtual QGraphicsScene *getFirstPage()=0;
    virtual QGraphicsScene *getSecondPage()=0;
    virtual QGraphicsScene *getThirdPage()=0;
    virtual QGraphicsScene *getFourthPage()=0;
    virtual QStringList    getPageSizeList()=0;
    virtual QStringList getElemNameList()=0;
    virtual QSize getPageSizeFromString(QString & page_str)=0;
    virtual bool getPageOrientation()=0;
    virtual void setPageOrientation(bool p_orient)=0;
    virtual Templ_info getTemplInfo() =0;
};


QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(Itmpl_plugin,"com.technoserv.Plugin.Itmpl_plugin/1.0");
QT_END_NAMESPACE

//! [0]
#endif
