#ifndef TMPL_SQL_PLUGIN_H
#define TMPL_SQL_PLUGIN_H

/**
  * @todo В настоящий момент в шаблоне жестко создается 4 сцены,
  * указатели на которые возвращаются внешнему приложению
  * необходимо сделать наборот, шаблон возвращает QSqlQuery модель
  * описывающую набор страниц в виде таблицы
  * (№п/п, Название страницы, тип страницы [Лицевая сторона 1листа, <- {1,5}
  *                                         Лицевая сторона 2листа, <- {0,1}
  *                                         Обратная сторона листа, <- {0,1}
  *                                         Фонарик])               <- {0,1}
  * А приложение генерирует необходимое число QGraphicsScene и передает
  * в шаблон их через указатель для заполнения сцены элементами присутвующими
  * на данной странице в шаблоне
  * При создании  шаблона необходимо указывать требуемое число страниц
  */

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


class Tmpl_sql_plugin : public QObject , Itmpl_sql_plugin
{
    Q_OBJECT
    Q_INTERFACES(Itmpl_sql_plugin)
public:

    Tmpl_sql_plugin(QObject *parent = 0);
    ~Tmpl_sql_plugin();
    /**
      * @fn init(const QString & spool,const QString & sid)
      * Первоначальная настройка плагина
      */
    void init(const QString & spool,const QString & sid);

    inline bool isDBOpened(){return m_dbOpened;};
    inline bool isDBConnected(){return m_dbConnect;};

    TemplateInfoEditModel * getInfoModel(){return tInfoModel;};

    QSqlQueryModel  * getPSizeModel(){return pSizeModel;};
    /**
      * @fn Возвращает набор базовых элементов шаблона
      */
    QStringList getBaseElemNameList() const;

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
    void closeTemplates();
private:    

    bool m_dbOpened;
    bool m_dbConnect;
    /**
      * @var spool_dir    каталог для временных файлов
      * @var current_sid  уникальный сеансовый номер
      * Пути к генерируемым страницам шаблона 1-4
      * @todo должны формироваться по запросу приложения
      * @var firstPage_tmpl_fn;
      * @var secondPage_tmpl_fn;
      * @var thirdPage_tmpl_fn;
      * @var fourthPage_tmpl_fn;
      * Набор из 4-х сцен
      * @var firstPage_scene;
      * @var secondPage_scene;
      * @var thirdPage_scene;
      * @var fourthPage_scene;
      */
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

    bool view_code_state;

    QString m_connectionName;
    /**
      * @var baseElemList;
      * список базовых элементов
      */
    QStringList baseElemList;

    /**
      * @var currentDBFileName
      * Имя файла текущей открытой БД
      */
    QString currentDBFileName;
    /**
      * @fn Модель ИНФО_ШАБЛОНА
      */
    TemplateInfoEditModel  * tInfoModel;

    /**
      * @var Модель РАЗМЕРЫ_ЛИСТА
      */
    QSqlQueryModel  * pSizeModel;

    /**
      * @var Singleton DB connection
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

