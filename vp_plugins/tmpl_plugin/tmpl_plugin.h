#ifndef TMPL_PLUGIN_H
#define TMPL_PLUGIN_H

#include <QObject>
#include <QMap>
#include <QMetaType>
#include "templ_info.h"

#include "itmpl_plugin.h"
#include "tech_global.h"



using namespace SafeVirtualPrinter;


class QGraphicsItem;
class QPointF;
class QFont;
class QColor;
class QStringList;

class SimpleItem;

class Templ_Info;

class Tmpl_plugin :public QObject, Itmpl_plugin
{
    Q_OBJECT
    Q_INTERFACES(Itmpl_plugin)
public:
    Tmpl_plugin(QObject *parent=0);
    void init(const QString & spool,const QString & sid);
    // Просто загрузим шаблон и преобразуем его в набор сцен
    // так как модель не передали то и преобразование не производим
    // нужна отдельная функция при установке модели произвести обновление
    // каждой сцены в соответствии с моделью
    void loadTemplates(const QString & templates_in_file);

    void createEmptyTemplate(const QString & file_name);
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
    QStringList getElemNameList();
    QSize getPageSizeFromString(QString & page_str);
    // Возвращает текущую ориентацию шаблона
    bool getPageOrientation();
    // Изменяет текущую ориентацию
    void setPageOrientation(bool p_orient);
    Templ_info getTemplInfo(){return templ_info;};
    void setTemplInfo(Templ_info &tInfo){ templ_info = tInfo;};
signals:
    void error(QString error_message);
    void toLog(QString log_message);
    void allTemplatesPagesParsed(QGraphicsScene *scene_1,QGraphicsScene *scene_2,
                                 QGraphicsScene *scene_3,QGraphicsScene *scene_4);
    void emptyTemplateCreate(const QString & t_name);
    void allPagesConverted(QString &first,QString &second,
                          QString &third,QString &fourth);
public slots:


    void convertTemplatesToPdf(const QString & templates_in_file,QStandardItemModel * model);
    void update_scene(int pageNum);
    void setTemplates(const QString & templates_in_file,QStandardItemModel * model);

    // Добавим базовый элемент на страницу page
    void doAddBaseElementToPage(int page,QStringList &text_list);
    // Добавим img элемент на страницу page
    void doAddImgElementToPage(int page,QString &file_img);

    // сохраним текущий набор сцен в файл шаблона
    void doSaveTemplates();
protected:
    qreal findPageSize_H(int page_size_id);// Высота
    qreal findPageSize_W(int page_size_id);// Ширина
    int getElemIdByName(const QString &elem_name);
    bool parse_templates(const QString & in_file);
    QString findFromModel(const QString &find_line);

    // Возвращает число элементов в сцене
    int getElemCount(QGraphicsScene *scene);
    // Возвращает указатель на элемент paper  в сцене
    QGraphicsItem *findPaperElem(QGraphicsScene *scene);
    // Создает Qmap размеров страниц
    void createHW_Map();
private:
    Templ_info templ_info;
    //tInfo t_info; // Информация о шаблоне
    static const int t_version = 2; // версия шаблона
    QString page_marker; // маркер страницы
    QString templates_file_name; //Имя шаблона с которым работаем

    //  Заполняет список базовых элементов шаблона
    void fillElemMap();
    // Заполняет список размеров страниц
    void fillPageMap();
    // Создает основу страницы
    void create_page(QGraphicsScene * scene,qreal width,qreal height,
                     qreal m_top,qreal m_bottom,
                     qreal m_right,qreal m_left);
    // Создает новый базовый элемент
    void create_SimpleItem(QGraphicsItem *parent,
                           QPointF &ps, QFont &fnt,
                           QColor &col,QStringList &pList);

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
    QMap<QString, int> page_size_map;
    QStringList page_name_QSL;
    QStringList elem_name_QSL;
    QMap<QString,QSize> page_WH_map;
    // Список базовых элементов шаблона
    QMap <QString, int> elem_name_map;

};

#endif
