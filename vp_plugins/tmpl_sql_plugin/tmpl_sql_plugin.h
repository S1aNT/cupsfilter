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

    /**
      * @brief Singleton DB connection
      */
    QSqlDatabase DB_;
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

    void DumpError (const QSqlError& lastError);

    /**
      * @brief Предварительная настройка БД
      */
    bool InitDB ();
    /**
      * @brief Проверяет возможность создания файла по указанному пути
      * 1.Проверка что данный файл не существует.
      * Если файл есть на диске, то удаляем его с диска.
      * (Так как пользователь уже согласился с выбором именно этого файла,
      *  выбрал переписать его)
      * 2.Создаем и удаляем файл по указанному пути
      */
    bool IsValidFileName(const QString & fileName);
};


#endif

