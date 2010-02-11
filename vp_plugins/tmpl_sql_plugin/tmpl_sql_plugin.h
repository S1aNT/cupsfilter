#ifndef TMPL_SQL_PLUGIN_H
#define TMPL_SQL_PLUGIN_H

#include <QObject>
#include <QSqlDatabase>


class QSqlQuery;
class QSqlQueryModel;
class QSqlRelationalTableModel;
class QSqlError;

#include "itmpl_sql_plugin.h"
#include "tinfoeditmodel.h"

#include "tech_global.h"

using namespace SafeVirtualPrinter;

class Tmpl_sql_plugin : public QObject , Itmpl_sql_plugin
{
    Q_OBJECT
    Q_INTERFACES(Itmpl_sql_plugin)
public:

    Tmpl_sql_plugin(QObject *parent = 0);
    ~Tmpl_sql_plugin();

    inline bool isDBOpened(){return m_dbOpened;};
    inline bool isDBConnected(){return m_dbConnect;};

    void closeTemplates();	
    bool isLoad();
    bool hasError();
    //QSqlQueryModel  * getInfoModel(){return tInfoModel;};

    TemplateInfoEditModel * getInfoModel(){return tInfoModel;};

    QSqlQueryModel  * getPSizeModel(){return pSizeModel;};


signals:
    void error(pluginsError errCode,QString error_message);

public slots:
    /**
      *@brief Открытие шаблона работает так:
      * если драйвер БД загружен и установленно соединение,
      * то открыть существующий файл как БД
      */
    bool openTemplates(const QString & t_fileName);
    /**
      *@brief Создание пустого шаблона (БД)
      * если драйвер БД загружен и установленно соединение,
      * и имя файла проходит проверку на валидность
      * и такой файл в заданном месте можно создать
      * то создадим БД в заданном файле и наполним его начальным содержимым
      */
    bool createEmptyTemplate(const QString & t_fileName);
private:    
    bool createTables(QSqlQuery *query);

    bool m_dbOpened;
    bool m_dbConnect;

    QString m_connectionName;

    //QSqlRelationalTableModel * tInfoModel;
    /**
      * @brief Модель ИНФО_ШАБЛОНА
      */
    //QSqlQueryModel  * tInfoModel;
    TemplateInfoEditModel  * tInfoModel;

    /**
      * @brief Модель РАЗМЕРЫ_ЛИСТА
      */
    QSqlQueryModel  * pSizeModel;

    /**
      * @brief Singleton DB connection
      */
    QSqlDatabase DB_;
protected:
    /**
      * @brief Открываем существующую БД
      */
    bool openDataBase(const QString & t_fileName);
    /**
      * @brief Создаем пустую БД с параметрами по умолчанию
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
    bool fillModels();
    void DumpError (const QSqlError & lastError);
    /**
      * @brief Предварительная настройка БД
      */
    bool InitDB ();
    /**
      * @brief Проверяет корректность имени файла [имя файла].tmpl
      */
    bool isValidFileName(const QString & fileName);
    /**
      * @brief Проверяет возможность создания файла по указанному пути
      * 1.Проверка что данный файл не существует.
      * Если файл есть на диске, то удаляем его с диска.
      * (Так как пользователь уже согласился с выбором именно этого файла,
      *  выбрал переписать его)
      * 2.Создаем и удаляем файл по указанному пути
      */
    bool isCreateFile(const QString & fileName);
};


#endif

