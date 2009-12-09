#ifndef TMPL_PLUGIN_H
#define TMPL_PLUGIN_H

#include <QObject>
#include <QMap>
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
    QGraphicsScene *getFirstPage(){return firstPage_scene;};
    QGraphicsScene *getSecondPage(){return secondPage_scene;};
    QGraphicsScene *getThirdPage(){return thirdPage_scene;};
    QGraphicsScene *getFourthPage(){return fourthPage_scene;};
    QStringList getPageSizeList();


signals:
    void error(QString error_message);
    void toLog(QString log_message);
    void allTemplatesPagesParsed(QGraphicsScene *scene_1,QGraphicsScene *scene_2,
                                 QGraphicsScene *scene_3,QGraphicsScene *scene_4);
    void emptyTemplateCreate(const QString & t_name);
public slots:
    void update_scene(int pageNum);
    void setTemplates(const QString & templates_in_file,QStandardItemModel * model);
    // Добавим базовый элемент на страницу page
    void doAddBaseElementToPage(int page);
    // сохраним текущий набор сцен в файл шаблона
    void doSaveTemplates();
protected:
    qreal findPageSize_H(int page_size_id);
    qreal findPageSize_V(int page_size_id);
    int getElemIdByName(const QString &elem_name);
    bool parse_templates(const QString & in_file);
    QString findFromModel(const QString &find_line);
private:
    templates_info_t t_info; // Информация о шаблоне
    static const int version = 1;

    QStandardItemModel *work_model;

    QString spool_dir;
    QString current_sid;

    QString firstPage_tmpl_fn;
    QString secondPage_tmpl_fn;
    QString thirdPage_tmpl_fn;
    QString fourthPage_tmpl_fn;

    QGraphicsScene * firstPage_scene;
    QGraphicsScene * secondPage_scene;
    QGraphicsScene * thirdPage_scene;
    QGraphicsScene * fourthPage_scene;
    QMap<QString, int> page_size;

};

#endif
