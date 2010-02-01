#ifndef TMPL_SQL_PLUGIN_H
#define TMPL_SQL_PLUGIN_H

#include <QObject>
#include <QSqlDatabase>


class QSqlQuery;
class QSqlQueryModel;
class QSqlRelationalTableModel;
#include "itmpl_sql_plugin.h"

//#include "tech_global.h"

using namespace VirtualPrninterPlugins;

class Tmpl_sql_plugin : public QObject , Itmpl_sql_plugin
{
    Q_OBJECT
    Q_INTERFACES(Itmpl_sql_plugin)

    QSqlDatabase DB_;
public:

    Tmpl_sql_plugin(QObject *parent = 0);
    ~Tmpl_sql_plugin();

    /**
      *@brief Открытие шаблона работает так:
      * если драйвер БД загружен и установленно соединение,
      * то открыть заданный файл как БД
      */

    bool openTemplates(const QString & t_fileName);
    //QStringList customFilters() const;

    void closeTemplates();	
    bool isLoad();
    bool hasError();

    /// Предварительная настройка БД
    void Prepare ();

signals:
   void error(pluginsError errCode,QString error_message);
public slots:
   bool createEmptyTemplate(const QString & t_fileName);
private:
   inline bool isDBOpened(){return m_dbOpened;};
   inline bool isDBConnected(){return m_dbConnect;};

    bool createTables(QSqlQuery *query);

    bool m_dbOpened;
    bool m_dbConnect;

    QString m_connectionName;
    //QSqlQuery m_query;

    /// Модель общие сведения о шаблоне
    //QSqlRelationalTableModel * tInfoModel;
    QSqlQueryModel  * tInfoModel;



protected:
    /**
      @brief Создаем пустую БД с параметрами по умолчанию
      */
    bool create_emptyDB(const QString &emptyDB_fileName);
    /**
      @brief Загружает драйвер и устанавливает соединение с БД sqlite
      */
    bool createConnection();
    /**
      * @brief Настройка модели, для установленного соединения
      * и файла открытого как БД
      * прочитать данные и записать их в соответсвующие модели
      */
    void setupModel();
    /**
      * @brief служебная функция открывает БД с шаблоном
      */
    bool openDB (const QString & db_fileName);
    void DumpError (const QSqlError& lastError);
};


#endif

