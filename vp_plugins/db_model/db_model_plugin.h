#ifndef DB_MODEL_PLUGIN_H
#define DB_MODEL_PLUGIN_H

#include "mytypes.h"
#include "idb_model_plugin.h"

#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>

QT_FORWARD_DECLARE_CLASS ( QSqlQuery )
        QT_FORWARD_DECLARE_CLASS ( QSqlQueryModel )
        QT_FORWARD_DECLARE_CLASS ( QSqlError )
        QT_FORWARD_DECLARE_CLASS ( QSqlTableModel )
        QT_FORWARD_DECLARE_CLASS ( QAbstractItemModel )

#include <QtCore/QObject>

        using namespace VPrn;

class DB_model_plugin : public QObject , Idb_model_plugin
{
    Q_OBJECT
    Q_INTERFACES(Idb_model_plugin)
 public:
    explicit  DB_model_plugin(QObject *parent = 0);
    ~DB_model_plugin();
    /**
      * @fn init(const QString & spool)
      * Первоначальная настройка плагина
      */
    void init(const QString &spool);

    void readTemplate(const QString &cuid, const QString &fileName,
                      QStandardItemModel *tInfo,QStandardItemModel *pages,
                      QStandardItemModel *elements);
signals:
    void error(VPrn::AppErrorType errCode,const QString &error_message);
    /**
      * @fn void readyToWork();
      * @brief Испускается когда плагин, выполнил все шаги инициализации:
      * @li Каталог временных файлов существует и доступен для записи
      * @li Драйвер SQLITE существует и доступен к загрузке
      */
    void readyToWork();
private:
    QString spoolDir;

    bool InitDB(QSqlDatabase DB_);
    /**
      * @fn bool fillTemplatesInfo(QSqlDatabase DB_,QStandardtemModel *model,
      *                            const QString &fileName);
      * @brief Заполняет модель ИНФОРМАЦИЯ_О_ШАБЛОНЕ
      */
    void fillTemplatesInfo(QSqlDatabase DB_,QStandardItemModel *model,
                           const QString &fileName);
    /**
      * @fn void fillPagesInfo(QSqlDatabase DB_,QStandardItemModel *model);
      * @brief Заполняет модель СТРАНИЦЫ_ШАБЛОНА
      */
    void fillPagesInfo(QSqlDatabase DB_,QStandardItemModel *model);
    /**
      * @fn void fillElementsInfo(QSqlDatabase DB_,QStandardItemModel *model);
      * @brief Заполняет модель ЭЛЕМЕНТЫ_НА_СТРАНИЦАХ
      */
    void fillElementsInfo(QSqlDatabase DB_,QStandardItemModel *model);
    void DumpError (const QSqlError & lastError);
};

#endif // DBWORKER_H
