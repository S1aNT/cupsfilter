#ifndef TMPL_SQL_PLUGIN_H
#define TMPL_SQL_PLUGIN_H

#include <QObject>
#include <QSqlDatabase>

class QSqlQuery;
class QSqlQueryModel;
class QSqlRelationalTableModel;
class QSqlError;
class QSqlTableModel;

#include "itmpl_sql_plugin.h"
#include "tinfoeditmodel.h"
#include "mytypes.h"

using namespace VPrn;

//#include "tech_global.h"
//using namespace SafeVirtualPrinter;

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
    //QSqlTableModel * getInfoModel2(){return tInfoModel2;};

    QSqlQueryModel  * getPSizeModel(){return pSizeModel;};


signals:
    void error(pluginsError errCode,QString error_message);

public slots:
    /**
      * @fn Открытие шаблона
      * @brief работает так если драйвер БД загружен и установленно соединение,
      * то открыть существующий файл как БД
      */
    void openTemplates(const QString & t_fileName);
    /**
      * @fn Создание пустого шаблона (БД) во временном файле
      * @brief если драйвер БД загружен и установленно соединение,
      * то создадим БД в заданном файле и наполним его начальным содержимым
      * Пустой шаблон создается в следующем порядке:
      * 1. проверка что файл заданный как БД шаблона не существует, но мождет быть создан
      *    - имя файла проходит проверку на валидность (имя_файла.tmpl)
      *    - такой файл в заданном месте можно создать
      * 2. Проверка того что драйвер БД загружен, соединение по умолчанию установленно
      * 3. Открытие БД и создание структуры базы
      */
    void createEmptyTemplate();
    /**
      * @fn сохраним шаблон с заданным именем
      */
    void saveTemplatesAs(const QString & fileName);
private:    

    bool m_dbOpened;
    bool m_dbConnect;

    QString m_connectionName;
    /**
      * @var currentDBFileName
      * Имя файла текущей открытой БД
      */
    QString currentDBFileName;

    //QSqlRelationalTableModel * tInfoModel;
    /**
      * @fn Модель ИНФО_ШАБЛОНА
      */
    TemplateInfoEditModel  * tInfoModel;
    //QSqlTableModel         * tInfoModel2;
    //QSqlQueryModel  * tInfoModel;

    /**
      * @fn Модель РАЗМЕРЫ_ЛИСТА
      */
    QSqlQueryModel  * pSizeModel;

    /**
      * @fn Singleton DB connection
      */
    QSqlDatabase DB_;

    /**
      * @fn Открываем существующую БД
      */
    bool openDataBase(const QString & t_fileName);
    /**
      * @fn Создаем пустую БД с параметрами по умолчанию
      */
    bool create_emptyDB(const QString &emptyDB_fileName);
    /**
      * @fn Создает набор необходимых таблиц
      * QSqlQuery &query связан с нужным соединением с БД
      */
    bool create_tables(QSqlQuery &query);
    /**
      @fn Загружает драйвер и устанавливает соединение с БД sqlite
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

