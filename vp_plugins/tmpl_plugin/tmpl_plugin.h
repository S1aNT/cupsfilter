#ifndef TMPL_PLUGIN_H
#define TMPL_PLUGIN_H

#include <QObject>
#include <QDebug>
#include <QPixmap>
#include <QtPlugin>
#include <QPainter>
#include <QFile>
#include <QDir>
#include <QTemporaryFile>
#include <QDataStream>
#include <QPrinter>
#include <QPainter>
#include <QDate>
#include <QGraphicsRectItem>


#include "itmpl_plugin.h"
#include "tech_global.h"

#define MM_TO_POINT(mm) ((mm)*2.83465058)

using namespace SafeVirtualPrinter;

class Tmpl_plugin :public QObject, Itmpl_plugin
{
    Q_OBJECT
    Q_INTERFACES(Itmpl_plugin)
public:

    Tmpl_plugin(QObject *parent=0){};
    void init(const QString & spool,const QString & sid);
    void createEmptyTemplate(const QString & file_name,
                             const QString & t_author,
                             const QString & t_name,
                             const QString & t_desc,
                             const QString & p_size,

                             bool pages_orient,
                             const QString & c_date,
                             qreal m_top,
                             qreal m_bottom,
                             qreal m_right,
                             qreal m_left);


    void printFormatingPageToFile(int pageNum);
    QGraphicsScene *getFirstPage(){return firstPage_tmpl;};
    QGraphicsScene *getSecondPage(){return secondPage_tmpl;};
    QGraphicsScene *getThirdPage(){return thirdPage_tmpl;};
    QGraphicsScene *getFourthPage(){return fourthPage_tmpl;};
    QStringList getPageSizeList();

signals:
    void error(QString error_message);
    void allTemplatesPagesParsed();
    void emptyTemplateCreate(const QString & t_name);
public slots:
    void update_scene(int pageNum);
    void setTemplates(const QString & templates_in_file,QStandardItemModel * model);
protected:
    qreal findPageSize_H(int page_size_id);
    qreal findPageSize_V(int page_size_id);
    int getElemIdByName(const QString &elem_name);
    bool parse_templates(const QString & in_file);
    QString findFromModel(const QString &find_line);
    static const int version = 1;

    QStandardItemModel *work_model;

    QString spool_dir;
    QString current_sid;
    QString currentTemplates;
    QString emptyTemplate_file; //

    QString firstPage_tmpl_fn;
    QString secondPage_tmpl_fn;
    QString thirdPage_tmpl_fn;
    QString fourthPage_tmpl_fn;

    QGraphicsScene * firstPage_tmpl;
    QGraphicsScene * secondPage_tmpl;
    QGraphicsScene * thirdPage_tmpl;
    QGraphicsScene * fourthPage_tmpl;


    QString date_time;               //
    QString author;                  //   (author [date_time])   tooltipe
    QString templates_name;          //  ,
    QString description;             //  ,
    QString paper_size;
    QMap<QString, int> page_size;
    qreal page_height;               //
    qreal page_width;                //
    bool  page_orient;               //
    qreal margin_top;                //
    qreal margin_bottom;             //
    qreal margin_left;               //
    qreal margin_right;              //

    int firstPageElemCount;
    int secondPageElemCount;
    int thirdPageElemCount;
    int fourthPageElemCount;
};

#endif
