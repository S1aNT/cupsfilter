#ifndef TMPL_SQL_PLUGIN_H
#define TMPL_SQL_PLUGIN_H

/**
  * @mainpage Плагин
  * предназначен для работы с шаблоном, который представляет БД SQLITE
  * При первоначальной загрузке плагина происходит инициализация рабочих,
  * наборов данных:
  * @var  QMap <int,myScene *> scenesGrp;
  * Список указателей на сцены которые отрисовывают страницы шаблона
  * @var QUndoGroup * undoGrp;
  * Группа UndoStack  каждый их которых связан со своей сценой
  * В памяти :memory: создается пустая БД заполненая необходимыми
  * таблицами и связями, с данной БД связваются  необходимые модели.
  * Если пользователь выбирает СОЗДАТЬ новый пустой шаблон, то создается
  * новая БД по указанному пользователем пути,которая присоедияется к БД
  * в памяти ATTACH DB и в нее копируются данный из временной БД, после чего
  * модели настраиваютсяя на получение данных из новой БД, если возникает
  * необходимость нового временного шаблона, то данный цикл повторяется
*/
#include "itmpl_sql_plugin.h"
#include "tinfoeditmodel.h"
#include "editpagesmodel.h"
#include "myscene.h"
#include "mytypes.h"


#include <QtGui/QUndoStack>
#include <QtGui/QUndoGroup>

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QMap>
#include <QtCore/QByteArray>
#include <QtGui/QPrinter>
#include <QtSql/QSqlDatabase>

class QSqlQuery;
class QSqlQueryModel;
class QSqlRelationalTableModel;
class QSqlError;
class QSqlTableModel;

class myScene;

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


    inline bool isDBOpened()   {return m_dbOpened;}
    inline bool isDBConnected(){return m_dbConnect;}

    /**
      * @brief Геттеры возвращают константные указатели на
      * @fn getInfoModel();        модель ИНФО_ШАБЛОНА
      * @fn getPSizeModel();       модель РАЗМЕРЫ_ЛИСТА
      * @fn getPagesModel();       модель СТРАНИЦЫ_ШАБЛОНА
      * @fn getBaseElemNameList(); набор базовых элементов шаблона
      * @fn getUndoGrp();          группу стеков
      * @fn getScenesGroup();      список сцен, со связанными стеками Undo
      * @fn getFilesGroup();       список путей к pdf файлам myScene -> pdf
      */
    TemplateInfoEditModel * getInfoModel() const {return tInfoModel;}
    QSqlQueryModel        * getPSizeModel()const {return pSizeModel;}
    EditPagesModel        * getPagesModel()const {return pagesModel;}
    QStringList             getBaseElemNameList() const;
    QUndoGroup            * getUndoGrp() const   {return undoGrp;}
    QMap <int,myScene *>    getScenesGroup()     {return scenesGrp;}
    QMap <int,QString>      getFilesGroup()      {return filesGrp;}

    //-------------------------------------------------------------------

    /**
      * @brief Переключает режим отображения Код / Значение на всех сценах
      */
    void setViewMode();

    /**
      * @fn void setUserName (const QString &user){userName = user;};
      * @brief Устанавливает имя пользователя полученное от плагина Auth
      */
    void setUserName (const QString &user){userName = user;}
    /**
      * @fn void printAllPage2Pdf();
      * @brief Если шаблон успешно загружен и вышстоящее приложение потребовало
      * сформировать страницы в pdf, то в результате функция возвращает
      * QMap<int,QString> содержащий полные пути к сформированным страницам
      * и их номера
      *
      */
    //void printAllPage2Pdf();

    /**
      * @fn QStringList fillTemplatesCreatePages(const QString &c_uuid,
      *                                   const QByteArray client_data);
      * @brief Загружает требуемый шаблон, заполняет его требумыми данными и
      * формирует pdf страницы шаблона,данные получаются путем чтения client_data
      * как форматированно потока данных.Особенность, что QGraphicsScene
      * создается одна и страницы в ней формируются последовательно.
      * @param const QString &c_uuid        Уникальный номер клиента
      * @param const QByteArray client_data Массив данных переданных клиентом
      * @return Возращает true  в случае успешного разбора, иначе false
      */
      QStringList loadAndFillTemplateCreatePages(const QString &c_uuid,
                                        QByteArray client_data);

signals:
    void error(pluginsError errCode,QString error_message);
    void allTemplatesPagesParsed();
    void allPagesConverted();

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
      * @li 1. Проверка что файл заданный как БД шаблона не существует, но мождет быть создан
      * @li имя файла проходит проверку на валидность (имя_файла.tmpl)
      * @li такой файл в заданном месте можно создать
      * @li 2. Проверка того что драйвер БД загружен, соединение по умолчанию установленно
      * @li 3. Открытие БД и создание структуры базы
      */
    void createEmptyTemplate();
    /**
      * @fn saveTemplatesAs(const QString & fileName)
      * @brief сохраним шаблон с заданным именем происходит просто копирование
      * файла по новому пути, с проверкой возможности данной операции
      */
    void saveTemplatesAs(const QString & fileName);
    /**
      * @fn saveTemplates(const QString & fileName)
      * @brief Запись в существующий файл шаблона, являющейся корректной БД
      * при этом эта БД уже открыта
      * данных (элементы размещенные на странице) которые ввел пользователь,
      * в нужные таблицы с проверкой возможности данной операции
      */
    void saveTemplates();

    /** @fn doAddBaseElementToPage(int page,QString &tag)
      * добавляет базовый элемент на страницу с номером page
      */
    void doAddBaseElementToPage(int page,const QString &tag);
    /**
      * @fn void convert2Pdf();
      * @brief Преобразует текущий загруженный шаблон в набор pdf страниц
      */
    void convert2Pdf();
private:
    /**
      * @var userName;    имя текущего пользователя полученное от подсистемы авторизации
      * @var spool_dir    каталог для временных файлов
      * @var current_sid  уникальный сеансовый номер
      */
    QString userName;
    QString spool_dir;
    QString current_sid;

    bool view_code_state;

    /**
      * @var Singleton DB connection
      */
    QSqlDatabase DB_;
    bool m_dbOpened;
    bool m_dbConnect;
    QString m_connectionName;

    /**
      * @var QHash<QString, QString> m_tagValue;
      * @var QMap <int,QString> filesGrp;       Список файлов pdf
      * @var QMap <int,myScene *> scenesGrp;    Список сцен
      * @var QMap <int,QGraphicsItem *> pageItems;  Список элементов, привязанных к странице
      * @var QUndoGroup * undoGrp;              Группа стеков Undo
      * @var baseElemList;                      Список базовых элементов
      * @var currentDBFileName;                 Имя файла текущей открытой БД
      * @var TemplateInfoEditModel *tInfoModel; Модель ИНФО_ШАБЛОНА
      * @var QSqlQueryModel * pSizeModel;       Модель РАЗМЕРЫ_ЛИСТА
      */

    QMap <int,QString> filesGrp;
    QMap <int,myScene *> scenesGrp;

    QUndoGroup * undoGrp;
    QStringList baseElemList;
    QString currentDBFileName;
    TemplateInfoEditModel  * tInfoModel;
    QSqlQueryModel * pSizeModel;

    /**
      * @var редактируемая Модель СТРАНИЦЫ_ШАБЛОНА
      * @brief В данной модели храняться все страницы шаблона
      * №п/п|Тип страницы| Отображаемое имя страницы
      */
    EditPagesModel * pagesModel;
    /**
      * @todo сделать данную модель редкатируемой, при добавлении или удалении элемента
      * со страницы, соответственно добавлять или удалять элемент модели, при записи шаблона,
      * писать из модели а не из страницы
      * @var Модель ЭЛЕМЕНТЫ_СТРАНИЦЫ
      * @brief В данной модели храняться все элементы
      * размещенные на i-странице шаблона
      * text|tag|pos_x|pos_y|color|font|angle|border|img_data|always_view
      */
    QSqlQueryModel * elemInPageModel;

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


    /** @fn void fillScenes4Data()
      * Рабор данных полученных из шаблона и запись их в сцены
      */
    void fillScenes4Data();

    /**
      * @fn bool fillPagesSizeTable(QSqlQuery &query)
      * @brief Заполняет таблицу размеров страниц
      */
    bool fillPagesSizeTable(QSqlQuery &query);

    /**
      * @fn int getId4pageSizeTable(QSqlQuery &query,const QString & findSize)
      * @retval int
      * @brief Возвращает id Записи в таблице размеров страниц
      */
    int getId4pageSizeTable(QSqlQuery &query,const QString & findSize);



    /**
      * @fn bool saveDataToBase(const QHash<QString, QString> &hash);
      * @brief проходит по всем элементам каждой страницы и проверяет
      * есть ли там элемент с тегом = tag если есть, то записывает ему новое
      * значение, координаты элемента не меняются
      */
    bool saveDataToBase(const QHash<QString, QString> &hash);

};


#endif

