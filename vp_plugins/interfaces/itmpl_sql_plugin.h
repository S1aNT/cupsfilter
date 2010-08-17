#ifndef ITMPL_SQL_PLUGIN_H
#define ITMPL_SQL_PLUGIN_H
#include "mytypes.h"

using namespace VPrn;

QT_FORWARD_DECLARE_CLASS( QString )
QT_FORWARD_DECLARE_CLASS( QStandardItemModel )
QT_FORWARD_DECLARE_CLASS( QSqlQueryModel )
QT_FORWARD_DECLARE_CLASS( QSqlTableModel )
QT_FORWARD_DECLARE_CLASS( QStringList )
QT_FORWARD_DECLARE_CLASS( QUndoGroup )
QT_FORWARD_DECLARE_CLASS( QByteArray )

class myScene;
class TemplateInfoEditModel;
class EditPagesModel;
#include <QObject>
#include <QHash>
#include <QMap>
#include <QPair>


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
    */
    virtual QStringList getBaseElemNameList() const = 0;

    virtual void openTemplates(const QString & t_fileName) = 0;
    virtual void createEmptyTemplate() = 0;
    virtual TemplateInfoEditModel  * getInfoModel()  const = 0;
    virtual QSqlQueryModel         * getPSizeModel() const = 0;
    virtual EditPagesModel         * getPagesModel() const = 0;
    virtual QUndoGroup             * getUndoGrp()    const = 0;
    virtual QMap <int,myScene *>     getScenesGroup()      = 0;
    virtual QMap <int,QString >      getFilesGroup()       = 0;
    virtual void saveTemplatesAs(const QString & fileName) = 0;
    virtual void saveTemplates () = 0 ;
    virtual bool isDBOpened    () = 0;
    virtual bool isDBConnected () =0 ;

    virtual void doAddBaseElementToPage(int page,const QString &text) = 0;
    virtual void doAddImgElementToPage(int page,const QString &file_name) =0;
    virtual void convert2Pdf() = 0;
    virtual void setUserName (const QString &user) = 0;
    virtual void setViewMode () =0 ;
    virtual bool prepare_template(const QString &c_uuid,const QString &t_fileName,
                                  const QMap <int,QString> elemMap,
                                  int copy_number) =0 ;
};

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(Itmpl_sql_plugin,"com.technoserv.Plugin.Itmpl_sql_plugin/1.0");
QT_END_NAMESPACE

//! [0]
#endif
