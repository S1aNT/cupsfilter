#include "tmpl_sql_plugin.h"

#include <QtCore/QDebug>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlRelation>
#include <QtSql/QSqlRelationalTableModel>
#include <QtSql/QSqlQuery>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QTemporaryFile>
#include <QPrinter>
#include <QMetaType>

using namespace VirtualPrninterPlugins;

Tmpl_sql_plugin::Tmpl_sql_plugin(QObject *parent)
    : QObject(parent)
    , m_dbOpened(false)
    , m_dbConnect(false)
    , m_connectionName(QString())
{

    //qRegisterMetaType<pluginsError>("pluginsError");
    /// Создаем модели
    tInfoModel = new QSqlQueryModel(this);

    m_connectionName = QLatin1String("TCONN");
    m_dbConnect = createConnection();
    m_dbOpened = false;
}



Tmpl_sql_plugin::~Tmpl_sql_plugin()
{
    if (DB_.isOpen()){
        DB_.close();
    }
    DB_.removeDatabase(m_connectionName);
}

bool Tmpl_sql_plugin::createConnection()
{
    bool openingOk = true;
    {
        DB_ = QSqlDatabase::addDatabase(QLatin1String("QSQLITE"),
                                        m_connectionName);
        if (DB_.driver()
            && DB_.driver()->lastError().type() == QSqlError::ConnectionError) {
            emit error(DriverNotLoad,tr("Не могу загрузить драйвер sqlite!"));
            openingOk = false;
        }

    }
    return  openingOk;
}


void Tmpl_sql_plugin::setupModel()
{
    /// Чтение данных в модель
    tInfoModel->setQuery("SELECT t_name,t_desc,page_size.page_human,"
                         "page_size.print_paper_size,"
                         "page_size.p_witdh,page_size.p_height,angle,c_time,"
                         "m_time,author,"
                         "margin_top,margin_bottom,margin_left,margin_right"
                         "FROM template INNER JOIN page_size ON"
                         "template.psize_id=page_size.id");
    // TODO fixme

}

bool Tmpl_sql_plugin::openTemplates(const QString & t_fileName)
{
    bool Ok = true;
    {
        /** Проверка что соединение с БД  было предварительно установленно
           (драйвер был загружен)
         */
        if (isDBConnected()){
            Ok &= IsValidFileName(t_fileName);
            if (Ok){
                DB_.setDatabaseName(t_fileName);
                Ok &= DB_.open();
                if (Ok) {
                    Ok &= InitDB();
                }else{
                    emit error(DBOpeningError,
                               tr("Не могу открыть файл шаблона: %1\nОшибка %2")
                               .arg(t_fileName).arg(DB_.lastError().text()));
                    DumpError(DB_.lastError());
                    DB_.removeDatabase(m_connectionName);
                }
            }
        }
    }
    m_dbOpened = Ok;
    return  m_dbOpened;
}


//************************* Public slots *************************************
/**
  @brief Пустой шаблон создается в следующем порядке:
  1. проверка что файл заданный как БД шаблона не существует,
     но мождет быть создан
  2. Проверка того что драйвер БД загружен
  3. Установка соединения с БД и создание структуры базы
  */

bool Tmpl_sql_plugin::createEmptyTemplate(const QString & t_fileName)
{
    bool Ok = true;
    {
        /// Пытаемся открыть данный файл как БД
        Ok &= openTemplates(t_fileName);
        if (Ok){
            /// Создадим пустую база с нужными полями
            Ok &= create_emptyDB(t_fileName);
            if (!Ok){
                emit error(SQLCommonError,
                           tr("Ошибка создания пустого шаблона в файле %1")
                           .arg(t_fileName));
            }
        }


    }
    return Ok;
}
//****************************************************************************

//***************** protected functions **************************************

bool Tmpl_sql_plugin::InitDB()
{
    QSqlQuery pragma (DB_);
    bool Ok = true;
    {
        Ok &= pragma.exec ("PRAGMA journal_mode = TRUNCATE;");
        if (!Ok){
            DumpError(pragma.lastError());
        }else{
            Ok &= pragma.exec ("PRAGMA synchronous = OFF;");
            if (!Ok){
                DumpError(pragma.lastError());
            }else{
                Ok &=pragma.exec ("PRAGMA temp_store = MEMORY;");
                if (!Ok){
                    DumpError(pragma.lastError());
                }
            }
        }
    }
    return Ok;
}

bool Tmpl_sql_plugin::IsValidFileName(const QString & fileName)
{
    bool Ok = true;
    {
        if (fileName.isEmpty()){
            emit error(FileIOError, tr("Имя файла шаблона не заданно!"));
            return false;
        }else{
            QFileInfo fi(fileName);
            QString suf = fi.suffix();
            if (suf.compare("tmpl",Qt::CaseInsensitive)!=0){
                emit error(FileIOError,
                           tr("файла шаблона должен иметь расширение tmpl!"));
                return false;
            }
        }

        Ok &= QFile::exists(fileName);
        if (Ok){
            Ok &= QFile::remove(fileName);
            if (!Ok){
                emit error(FileIOError,
                           tr("Ошибка удаления временного файла %1")
                           .arg(fileName));
            }
        }else{
            QFile file(fileName);
            Ok &= file.open(QIODevice::WriteOnly);
            if (Ok){
                file.close();
                Ok &= file.remove();
            }else{
                emit error(FileIOError,tr("Ошибка создания файла шаблона %1.")
                           .arg(fileName));
            }
        }
    }
    return Ok;
}

void Tmpl_sql_plugin::DumpError (const QSqlError& lastError)
{
    qDebug()<< Q_FUNC_INFO
            << lastError.text() << "|"
            << lastError.databaseText() << "|"
            << lastError.driverText() << "|"
            << lastError.type() << "|"
            << lastError.number();
}

bool Tmpl_sql_plugin::create_emptyDB(QString const&)
{
    /**
      @brief При вызове функции соединение с БД должно быть установленно
      файл БД существуетн.
      */
    bool Ok =true;
    {
        // Создаем пустую БД с прописанными значениями
        QSqlQuery query (DB_);

        /**
           Создаем набор таблиц
        */
        //Таблица эементов

        Ok &= query.exec("CREATE TABLE elem (id INTEGER primary key autoincrement,"
                         "page_detail_id INTEGER UNIQUE,text TEXT,tag TEXT,"
                         "pos_x real,pos_y real,color blob,font blob,angle real,"
                         "border boolean,img_data blob, always_view boolean, "
                         "FOREIGN KEY(page_detail_id) REFERENCES page_detail(id))"
                         );
        if (!Ok){
            DumpError(query.lastError());
        }
        //Таблица описание страницы
        /**
          * p_number порядковый номер страницы в шаблоне
          * p_type тип страницы в шаблоне 1,2,3
          * (Первая [может быть несколько], остальные , фонарик)
          * p_name название страницы для отображения в редакторе
          */
        Ok &= query.exec("create table page_detail (id INTEGER primary key autoincrement, "
                         "p_number INTEGER,p_type INTEGER,p_name TEXT);"
                         );
        if (!Ok){
            DumpError(query.lastError());
        }

        //Таблица размеров страниц
        Ok &= query.exec("create table page_size (id INTEGER primary key autoincrement, "
                         "page_human TEXT,print_paper_size INTEGER,p_witdh real,"
                         "p_height real);"
                         );
        if (!Ok){
            DumpError(query.lastError());
        }

        //Таблица шаблон (основная таблица)
        Ok &= query.exec("create table template (id INTEGER primary key autoincrement, "
                         "psize_id INTEGER UNIQUE,angle real,c_time INTEGER,"
                         "m_time INTEGER,author TEXT,"
                         "t_name TEXT, t_desc TEXT,"
                         "margin_top real,margin_bottom real,margin_left real,"
                         "margin_right real,"
                         "FOREIGN KEY(psize_id) REFERENCES page_size(id));"
                         );
        if (!Ok){
            DumpError(query.lastError());
        }

        // Таблицы отношений
        Ok &= query.exec("create table rel_templ_page"
                         "(templ_id INTEGER REFERENCES page_size(id),"
                         "page_detail_id INTEGER REFERENCES page_detail(id));"
                         );
        if (!Ok){
            DumpError(query.lastError());
        }

        Ok &= query.exec("CREATE UNIQUE INDEX tag_ix ON elem(tag);");
        if (!Ok){
            DumpError(query.lastError());
        }

        // Создаем базовый набо страниц
        Ok &= query.prepare("insert into page_size (page_human,print_paper_size,"
                            "p_witdh,p_height) VALUES(?,?,?,?);");
        if (!Ok){
            DumpError(query.lastError());
        }

        query.addBindValue("(A4 (210 x 297 мм)");
        query.addBindValue(QPrinter::A4);
        query.addBindValue(210);
        query.addBindValue(297);
        Ok &= query.exec();

        query.addBindValue("(A3 (297 x 420 мм)");
        query.addBindValue(QPrinter::A3);
        query.addBindValue(297);
        query.addBindValue(420);

        Ok &= query.exec();
        query.addBindValue("(A0 (841 x 1189 мм)");
        query.addBindValue(QPrinter::A0);
        query.addBindValue(841);
        query.addBindValue(1189);
        Ok &= query.exec();

        query.addBindValue("(A1 (594 x 841 мм)");
        query.addBindValue(QPrinter::A1);
        query.addBindValue(594);
        query.addBindValue(841);
        Ok &= query.exec();

        query.addBindValue("(A2 (420 x 594 мм)");
        query.addBindValue(QPrinter::A2);
        query.addBindValue(420);
        query.addBindValue(594);
        Ok &= query.exec();

        query.addBindValue("(A5 (148 x 210 мм)");
        query.addBindValue(QPrinter::A5);
        query.addBindValue(148);
        query.addBindValue(210);
        Ok &= query.exec();

        query.addBindValue("(A6 (105 x 148 мм)");
        query.addBindValue(QPrinter::A6);
        query.addBindValue(105);
        query.addBindValue(148);
        Ok &= query.exec();

        query.addBindValue("(A7 (74 x 105 мм)");
        query.addBindValue(QPrinter::A7);
        query.addBindValue(74);
        query.addBindValue(105);
        Ok &= query.exec();

        query.addBindValue("(A8 (52 x 74 мм)");
        query.addBindValue(QPrinter::A8);
        query.addBindValue(52);
        query.addBindValue(74);
        Ok &= query.exec();

        query.addBindValue("(A9 (37 x 52 мм)");
        query.addBindValue(QPrinter::A9);
        query.addBindValue(37);
        query.addBindValue(52);
        Ok &= query.exec();

        query.addBindValue("(B0 (1000 x 1414 мм)");
        query.addBindValue(QPrinter::B0);
        query.addBindValue(1000);
        query.addBindValue(1414);
        Ok &= query.exec();

        query.addBindValue("(B1 (707 x 1000 мм)");
        query.addBindValue(QPrinter::B1);
        query.addBindValue(707);
        query.addBindValue(1000);
        Ok &= query.exec();

        query.addBindValue("(B2 (500 x 707 мм)");
        query.addBindValue(QPrinter::B2);
        query.addBindValue(500);
        query.addBindValue(707);
        Ok &= query.exec();

        query.addBindValue("(B3 (353 x 500 мм)");
        query.addBindValue(QPrinter::B3);
        query.addBindValue(353);
        query.addBindValue(500);
        Ok &= query.exec();

        query.addBindValue("(B4 (250 x 353 мм)");
        query.addBindValue(QPrinter::B4);
        query.addBindValue(250);
        query.addBindValue(353);
        Ok &= query.exec();

        query.addBindValue("(B5 (176 x 250 мм)");
        query.addBindValue(QPrinter::B5);
        query.addBindValue(176);
        query.addBindValue(250);
        Ok &= query.exec();

        query.addBindValue("(B6 (125 x 176 мм)");
        query.addBindValue(QPrinter::B6);
        query.addBindValue(125);
        query.addBindValue(176);
        Ok &= query.exec();

        query.addBindValue("(B7 (88 x 125 мм)");
        query.addBindValue(QPrinter::B7);
        query.addBindValue(88);
        query.addBindValue(125);
        Ok &= query.exec();

        query.addBindValue("(B8 (62 x 88 мм)");
        query.addBindValue(QPrinter::B8);
        query.addBindValue(62);
        query.addBindValue(88);
        Ok &= query.exec();

        query.addBindValue("(B9 (44 x 62 мм)");
        query.addBindValue(QPrinter::B9);
        query.addBindValue(44);
        query.addBindValue(62);
        Ok &= query.exec();

        query.addBindValue("(B10 (31 x 44 мм)");
        query.addBindValue(QPrinter::B10);
        query.addBindValue(31);
        query.addBindValue(44);
        Ok &= query.exec();

        query.addBindValue("(C5E (163 x 229 мм)");
        query.addBindValue(QPrinter::C5E);
        query.addBindValue(163);
        query.addBindValue(229);
        Ok &= query.exec();

        query.addBindValue("(DLE (110 x 220 мм)");
        query.addBindValue(QPrinter::DLE);
        query.addBindValue(110);
        query.addBindValue(220);
        Ok &= query.exec();

        query.addBindValue("(Executive (191 x 254 мм)");
        query.addBindValue(QPrinter::Executive);
        query.addBindValue(191);
        query.addBindValue(254);
        Ok &= query.exec();

        query.addBindValue("(Folio (210 x 330 мм)");
        query.addBindValue(QPrinter::Folio);
        query.addBindValue(210);
        query.addBindValue(330);
        Ok &= query.exec();

        query.addBindValue("(Ledger (432 x 279 мм)");
        query.addBindValue(QPrinter::Ledger);
        query.addBindValue(432);
        query.addBindValue(279);
        Ok &= query.exec();

        query.addBindValue("(Legal (216 x 356 мм)");
        query.addBindValue(QPrinter::Legal);
        query.addBindValue(216);
        query.addBindValue(356);
        Ok &= query.exec();

        query.addBindValue("(Letter (216 x 279 мм)");
        query.addBindValue(QPrinter::Letter);
        query.addBindValue(216);
        query.addBindValue(279);
        Ok &= query.exec();

        query.addBindValue("(Tabloid (279 x 432 мм)");
        query.addBindValue(QPrinter::Tabloid);
        query.addBindValue(279);
        query.addBindValue(432);
        Ok &= query.exec();

        // Создаем базовый набор обязательных полей
        Ok &= query.prepare("insert into elem (tag,pos_x,pos_y,border) VALUES(?,?,?,?);");
        if (!Ok){
            DumpError(query.lastError());
        }

        query.addBindValue("МБ");
        query.addBindValue(100);
        query.addBindValue(100);
        query.addBindValue(1);
        Ok &= query.exec();

        query.addBindValue("Название док-та");
        query.addBindValue(100);
        query.addBindValue(100);
        query.addBindValue(1);
        Ok &= query.exec();

        query.addBindValue("Гриф");
        query.addBindValue(100);
        query.addBindValue(100);
        query.addBindValue(1);
        Ok &= query.exec();

        query.addBindValue("Пункт перечня");
        query.addBindValue(100);
        query.addBindValue(100);
        query.addBindValue(1);
        Ok &= query.exec();

        query.addBindValue("Номер копии");
        query.addBindValue(100);
        query.addBindValue(100);
        query.addBindValue(1);
        Ok &= query.exec();

        query.addBindValue("Кол-во листов");
        query.addBindValue(100);
        query.addBindValue(100);
        query.addBindValue(1);
        Ok &= query.exec();

        query.addBindValue("Исполнитель");
        query.addBindValue(100);
        query.addBindValue(100);
        query.addBindValue(1);
        Ok &= query.exec();

        query.addBindValue("Отпечатал");
        query.addBindValue(100);
        query.addBindValue(100);
        query.addBindValue(1);
        Ok &= query.exec();


        query.addBindValue("Телефон");
        query.addBindValue(100);
        query.addBindValue(100);
        query.addBindValue(1);
        Ok &= query.exec();

        query.addBindValue("Инв. N");
        query.addBindValue(100);
        query.addBindValue(100);
        query.addBindValue(1);
        Ok &= query.exec();

        query.addBindValue("Дата распечатки");
        query.addBindValue(100);
        query.addBindValue(100);
        query.addBindValue(1);
        Ok &= query.exec();

        query.addBindValue("Получатель N1");
        query.addBindValue(100);
        query.addBindValue(100);
        query.addBindValue(1);
        Ok &= query.exec();


        query.addBindValue("Получатель N2");
        query.addBindValue(100);
        query.addBindValue(100);
        query.addBindValue(1);
        Ok &= query.exec();

        query.addBindValue("Получатель N3");
        query.addBindValue(100);
        query.addBindValue(100);
        query.addBindValue(1);
        Ok &= query.exec();

        query.addBindValue("Получатель N4");
        query.addBindValue(100);
        query.addBindValue(100);
        query.addBindValue(1);
        Ok &= query.exec();

        query.addBindValue("Получатель N5");
        query.addBindValue(100);
        query.addBindValue(100);
        query.addBindValue(1);
        Ok &= query.exec();

        query.addBindValue("Изображение");
        query.addBindValue(100);
        query.addBindValue(100);
        query.addBindValue(1);
        Ok &= query.exec();
    }
    return Ok;
}
//****************************************************************************
void Tmpl_sql_plugin::closeTemplates()
{

}
bool Tmpl_sql_plugin::isLoad()
{
    return false;
}
bool Tmpl_sql_plugin::hasError()
{
    return false;
}

Q_EXPORT_PLUGIN2(Itmpl_sql_plugin, Tmpl_sql_plugin)
;
