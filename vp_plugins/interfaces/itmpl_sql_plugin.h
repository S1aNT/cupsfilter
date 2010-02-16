#ifndef ITMPL_SQL_PLUGIN_H
#define ITMPL_SQL_PLUGIN_H

class QString;
class QStandardItemModel;
class QGraphicsScene;
class QSize;
class QSqlQueryModel;
class QSqlTableModel;
class QStringList;

class TemplateInfoEditModel;
#include <QObject>


class Itmpl_sql_plugin{

public:
    virtual ~Itmpl_sql_plugin() {}

    virtual void init(const QString & spool,const QString & sid) = 0;

    /**
     * @brief Триггер переключающий по кругу  режим отображения элементов
     *	      шаблона код / значения поля и обновляющий сцену
     */
    /*
    virtual void viewCode() =0 ;

    virtual void convertTemplatesToPdf(const QString & templates_in_file) = 0;
    virtual void printFormatingPageToFile(int pageNum) = 0;

    virtual QGraphicsScene *getFirstPage()  = 0;
    virtual QGraphicsScene *getSecondPage() = 0;
    virtual QGraphicsScene *getThirdPage()  = 0;
    virtual QGraphicsScene *getFourthPage() = 0;

    virtual QStringList getPageSizeList() = 0;
    */
    virtual QStringList getBaseElemNameList() const = 0;

    virtual void openTemplates(const QString & t_fileName) = 0;
    virtual void createEmptyTemplate() = 0;
    virtual TemplateInfoEditModel  * getInfoModel() = 0;
    virtual QSqlQueryModel  * getPSizeModel() = 0;
    /**
      * @fn Получение списка страниц хранящихся в шаблоне
      * №п/п Название страницы тип страницы
      virtual QSqlQueryModel  * getPageDescModel() = 0;
      */

    virtual void saveTemplatesAs(const QString & fileName) = 0;
    virtual bool isDBOpened() = 0;
    virtual bool isDBConnected() =0 ;
    
    virtual void doAddBaseElementToPage(int page,const QString &text) = 0;
    virtual void setUserName (const QString &user) = 0;
    //virtual QSqlTableModel  * getInfoModel2() = 0;
    virtual void closeTemplates() = 0;	

};

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(Itmpl_sql_plugin,"com.technoserv.Plugin.Itmpl_sql_plugin/1.0");
QT_END_NAMESPACE

//! [0]
#endif
