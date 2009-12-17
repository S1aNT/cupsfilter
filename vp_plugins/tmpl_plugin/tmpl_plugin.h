#ifndef TMPL_PLUGIN_H
#define TMPL_PLUGIN_H

#include <QObject>
#include <QMap>
#include <QMetaType>
#include "templ_info.h"

#include "itmpl_plugin.h"
#include "tech_global.h"


#define MM_TO_POINT(mm) ((mm)*2.83465058)

using namespace SafeVirtualPrinter;

struct tInfo
{
    int version; 	// версия шаблона
    QString date_time;  // дата и время создания
    QString t_author;   // автор шаблона
    QString t_name;     // имя шаблона
    QString t_desc;     // описание шаблона
    QString p_size;     // размер бумаги (Для человека)
    bool  page_orient;  // ориентация страницы Книжная/альбомная true/false
    qreal page_width;   // ширина листа в  [мм -> point] (для компьютера)
    qreal page_height;  // высота листа в  [мм -> point] (для компьютера)
    qreal m_top;        // отступ сверху в [мм -> point] (для компьютера)
    qreal m_bottom;     // отступ снизу в [мм -> point] (для компьютера)
    qreal m_left;       // отступ слева в [мм -> point] (для компьютера)
    qreal m_right;      // отступ справа в [мм -> point] (для компьютера)
    int firstPageElemCount;    // число элементов на первой странице шаблона
    int secondPageElemCount;   // число элементов на второй странице шаблона
    int thirdPageElemCount;    // число элементов на третьей странице шаблона
    int fourthPageElemCount;   // число элементов на четвертой странице шаблона
};
Q_DECLARE_METATYPE(tInfo);

inline QDataStream &operator<<( QDataStream &out, const tInfo& save )
{
    out << save.version;
    out << save.date_time;
    out << save.t_author;
    out << save.t_name;
    out << save.t_desc;
    out << save.p_size;
    out << save.page_orient;
    out << save.page_width;
    out << save.page_height;
    out << save.m_top;
    out << save.m_bottom;
    out << save.m_left;
    out << save.m_right;
    out << save.firstPageElemCount;
    out << save.secondPageElemCount;
    out << save.thirdPageElemCount;
    out << save.fourthPageElemCount;
    return out;
}

inline QDataStream &operator>>( QDataStream &in, tInfo& load)
{
    in >> load.version;
    in >> load.date_time;
    in >> load.t_author;
    in >> load.t_name;
    in >> load.t_desc;
    in >> load.p_size;
    in >> load.page_orient;
    in >> load.page_width;
    in >> load.page_height;
    in >> load.m_top;
    in >> load.m_bottom;
    in >> load.m_left;
    in >> load.m_right;
    in >> load.firstPageElemCount;
    in >> load.secondPageElemCount;
    in >> load.thirdPageElemCount;
    in >> load.fourthPageElemCount;
    return in;
}

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
