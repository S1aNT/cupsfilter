#include "tmpl_sql_plugin.h"
#include "simpleitem.h"

#include <QtCore/QDebug>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlRelation>
#include <QtSql/QSqlRelationalTableModel>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QtCore/QFile>
#include <QDir>
#include <QtCore/QRegExp>
#include <QTemporaryFile>
#include <QPrinter>
#include <QMetaType>
#include <QModelIndex>
#include <QDateTime>
#include <QtGui/QGraphicsItem>
#include <QtGui/QGraphicsScene>
#include <QtCore/QHashIterator>


using namespace VPrn;

Tmpl_sql_plugin::Tmpl_sql_plugin(QObject *parent)
    : QObject(parent)
    , m_dbOpened(false)
    , m_dbConnect(false)
    , view_code_state(false)
    , m_connectionName(QString())
{

    qRegisterMetaType<pluginsError>("pluginsError");

    m_connectionName = QLatin1String("TCONN");
    m_dbConnect = createConnection();
    m_dbOpened = false;
    if (m_dbConnect){
        /// Создаем модели
        elemInPageModel = new QSqlTableModel(this);
        pSizeModel = new QSqlQueryModel(this);
        pagesModel = new QSqlQueryModel(this);
        tInfoModel = new TemplateInfoEditModel(this);
    }
}

Tmpl_sql_plugin::~Tmpl_sql_plugin()
{
    if (DB_.isOpen()){
        DB_.close();
    }
    //DB_.removeDatabase();//m_connectionName);
}
//------------------------------------------------------------------------------

//****************************** public functions ******************************

void Tmpl_sql_plugin::init(const QString & spool,const QString & sid)
{
    QString e_msg;
    QDir dir;

    if (!sid.isEmpty()) {
        current_sid = sid;// Запомним уникальный номер
        if (dir.cd(spool) && !spool.isEmpty()) {
            // Проверим факт существования временного каталога
            spool_dir = spool;
            // Формируем пути для файлов
            firstPage_tmpl_fn  = QObject::trUtf8("%1/%2_first_tmpl.pdf").arg(spool, sid);
            firstPage_tmpl_fn2 = QObject::trUtf8("%1/%2_first2_tmpl.pdf").arg(spool, sid);
            firstPage_tmpl_fn3 = QObject::trUtf8("%1/%2_first3_tmpl.pdf").arg(spool, sid);
            firstPage_tmpl_fn4 = QObject::trUtf8("%1/%2_first4_tmpl.pdf").arg(spool, sid);
            firstPage_tmpl_fn5 = QObject::trUtf8("%1/%2_first5_tmpl.pdf").arg(spool, sid);

            secondPage_tmpl_fn = QObject::trUtf8("%1/%2_second_tmpl.pdf").arg(spool, sid);
            thirdPage_tmpl_fn = QObject::trUtf8("%1/%2_third_tmpl.pdf").arg(spool, sid);
            fourthPage_tmpl_fn = QObject::trUtf8("%1/%2_fourth_tmpl.pdf").arg(spool, sid);
            // создаем сцены
            firstPage_scene  = new QGraphicsScene(this);
            firstPage_sceneN2  = new QGraphicsScene(this);
            firstPage_sceneN3  = new QGraphicsScene(this);
            firstPage_sceneN4  = new QGraphicsScene(this);
            firstPage_sceneN5  = new QGraphicsScene(this);

            secondPage_scene = new QGraphicsScene(this);
            thirdPage_scene  = new QGraphicsScene(this);
            fourthPage_scene = new QGraphicsScene(this);
            // Заполним список базовых элементов шаблона

            baseElemList << QObject::tr("МБ")
                    << QObject::tr("Название док-та")
                    << QObject::tr("Гриф")
                    << QObject::tr("Пункт перечня")
                    << QObject::tr("Номер копии")
                    << QObject::tr("Кол-во листов")
                    << QObject::tr("Исполнитель")
                    << QObject::tr("Отпечатал")
                    << QObject::tr("Телефон")
                    << QObject::tr("Инв. N")
                    << QObject::tr("Дата распечатки")
                    << QObject::tr("Получатель N1")
                    << QObject::tr("Получатель N2")
                    << QObject::tr("Получатель N3")
                    << QObject::tr("Получатель N4")
                    << QObject::tr("Получатель N5");

        }else{
            e_msg = QObject::trUtf8("ERROR: каталог %1 не существует\n").arg(spool);
        }

    }else{
        e_msg = QObject::trUtf8("ERROR: Неверный SID для документа\n").arg(sid);
    }
    if (!e_msg.isEmpty()) {
        emit error(InternalPluginError,e_msg);
    }

}

QStringList Tmpl_sql_plugin::getBaseElemNameList() const
{
    return baseElemList;
}

//------------------------------------------------------------------------------
bool Tmpl_sql_plugin::createConnection()
{
    bool openingOk = true;
    {
        DB_ = QSqlDatabase::addDatabase(QLatin1String("QSQLITE"));
        if (DB_.driver()
            && DB_.driver()->lastError().type() == QSqlError::ConnectionError) {
            emit error(DriverNotLoad,tr("Не могу загрузить драйвер sqlite!"));
            openingOk = false;
        }

    }
    return  openingOk;
}

bool Tmpl_sql_plugin::openDataBase(const QString & t_fileName)
{
    bool Ok = true;
    {
        DB_.setDatabaseName(t_fileName);
        Ok &= DB_.open();
        if (Ok){
            Ok &= InitDB();   
        }
        if (!Ok){
            emit error(DBOpeningError,
                       tr("Не могу открыть файл шаблона: %1\nОшибка %2")
                       .arg(t_fileName).arg(DB_.lastError().text()));
            DumpError(DB_.lastError());
            DB_.close();
            //DB_.removeDatabase();//m_connectionName);
        }
    }
    return  Ok;
}

//************************* Public slots *************************************

void Tmpl_sql_plugin::setTagValue(const QHash <QString, QString> &tagValue)
{
    QSqlQuery query(DB_);
    bool Ok = true;
    {
        /**
          * @brief Проверка что соединение с БД установленно
          * (драйвер был загружен), БД Открыта
          */
        Ok &= isDBConnected() && isDBOpened();

        if (Ok){
            Ok &= query.prepare("UPDATE elem SET text = ? WHERE tag =?");
            if (Ok){

                QHashIterator<QString, QString> tV(tagValue);

                while (tV.hasNext()) {
                    tV.next();
                    query.addBindValue(tV.value());
                    query.addBindValue(tV.key());
                    query.exec();
                    //cout << tV.key() << ": " << tV.value() << endl;
                }
            }
        }else{
            emit error (VPrn::SQLCommonError,
                        tr("Соединение с БД шаблона не было установленно"));
        }
    }
    if (Ok){
        /// Обновим все страницы шаблона
        update_scenes(tagValue);
    }else{
        emit error (VPrn::SQLQueryError,
                    tr("Ошибка [%1] при записи значений в БД шаблона")
                    .arg(query.lastError().text()));
    }
}

void Tmpl_sql_plugin::openTemplates(const QString & t_fileName)
{
    bool Ok = true;
    {
        /// Проверка что соединение с БД установленно (драйвер был загружен)
        Ok &= isDBConnected()
              && isValidFileName(t_fileName)
              && QFile::exists(t_fileName);
        if (Ok){
            Ok &=openDataBase(t_fileName);
            if (Ok){
                Ok &= fillModels();
                // Рабор данных полученных из шаблона и запись их в сцены
                Ok &= fillScenes4Data();

                if (Ok){                  
                    emit allTemplatesPagesParsed(firstPage_scene,firstPage_sceneN2,
                                                 firstPage_sceneN3,firstPage_sceneN4,
                                                 firstPage_sceneN5,secondPage_scene,
                                                 thirdPage_scene, fourthPage_scene);
                }
            }
        }
    }
    m_dbOpened = Ok;
}

void Tmpl_sql_plugin::createEmptyTemplate()
{
    bool Ok = true;
    {
        {
            QTemporaryFile file;
            if (file.open()) {
                currentDBFileName = file.fileName(); // returns the unique file name
            }
            file.close();
        }
        if (!currentDBFileName.isNull()){
            /// Пытаемся открыть данный файл как БД
            Ok &=openDataBase(currentDBFileName);
            if (Ok){
                /// Создадим пустую база с нужными полями
                Ok &= create_emptyDB(currentDBFileName);
                if (Ok){
                    Ok &= fillModels();
                }else{
                    emit error(SQLCommonError,
                               tr("Ошибка создания пустого шаблона в файле %1")
                               .arg(currentDBFileName));
                }
            }
        }else{
            Ok = false;
            emit error(FileNotFound,tr("Ошибка создания временного файла %1")
                       .arg(currentDBFileName));
        }
    }
    m_dbOpened = Ok;
}

void Tmpl_sql_plugin::saveTemplatesAs(const QString & fileName)
{
    bool Ok = true;
    {
        Ok &= !fileName.isEmpty() && !currentDBFileName.isNull();
        if (Ok){
            DB_.close();
            //DB_.removeDatabase(m_connectionName);
            Ok &=isValidFileName(fileName) && isCreateFile(fileName);
            if (Ok){
                Ok &= QFile::copy(currentDBFileName,fileName);
                if (Ok){
                    currentDBFileName = fileName;
                    this->openTemplates(currentDBFileName);
                }
            }else{
                emit error(FileIOError,
                           tr("Ошибка сохранения шаблона по заданному пути %1").arg(fileName));
            }
        }
    }
}

void Tmpl_sql_plugin::doAddBaseElementToPage(int page,const QString &text)
{
    /**
      * @todo ИЗМЕНИТЬ
      */
    QString e_msg;
    QString l_msg = QString(" [%1] ").arg(QString::fromAscii(Q_FUNC_INFO));
    QGraphicsScene *scene;
    QGraphicsItem *item;

    switch(page){
    case VPrn::FirstPage:
        scene = firstPage_scene;
        break;       
    case VPrn::FirstPageN2:
        scene = firstPage_sceneN2;
        break;
    case VPrn::FirstPageN3:
        scene = firstPage_sceneN3;
        break;
    case VPrn::FirstPageN4:
        scene = firstPage_sceneN4;
        break;
    case VPrn::FirstPageN5:
        scene = firstPage_sceneN5;
        break;
    case VPrn::SecondPage:
        scene = secondPage_scene;
        break;
    case VPrn::ThirdPage:
        scene = thirdPage_scene;
        break;
    case VPrn::FourthPage:
        scene = fourthPage_scene;
        break;
    default:
        e_msg = QObject::trUtf8("Ошибка: Такой страницы %2 в шаблоне не существует")
                .arg(page,0,10);
        /// @todo Исправить
        emit error(InternalPluginError,e_msg);
        //emit error(e_msg);
        //emit toLog(l_msg+e_msg);
        break;
    }
    if (scene){
        item = findPaperElem(scene);

        SimpleItem * pItem = new SimpleItem();
        pItem->setZValue(100);
        pItem->setPos(100.0,100.0);
        //pItem->setText(QStringList()<<QObject::trUtf8("Элемент"));
        pItem->setTag(text);
        pItem->setFlag(QGraphicsItem::ItemIsMovable);
        pItem->setData(ObjectName, "tElem");
        pItem->setParentItem(item);
        scene->update();
    }
}

//------------------------------------------------------------------------------

//***************** private functions **************************************

QGraphicsScene * Tmpl_sql_plugin::selectScene(int page)const
{
    QGraphicsScene *scene(0);
    switch(page){
    case VPrn::FirstPage:
        scene = firstPage_scene;
        break;
    case VPrn::FirstPageN2:
        scene = firstPage_sceneN2;
        break;
    case VPrn::FirstPageN3:
        scene = firstPage_sceneN3;
        break;
    case VPrn::FirstPageN4:
        scene = firstPage_sceneN4;
        break;
    case VPrn::FirstPageN5:
        scene = firstPage_sceneN5;
        break;
    case VPrn::SecondPage:
        scene = secondPage_scene;
        break;
    case VPrn::ThirdPage:
        scene = thirdPage_scene;
        break;
    case VPrn::FourthPage:
        scene = fourthPage_scene;
        break;
    }
    return scene;
}
// Рабор данных полученных из шаблона и запись их в сцены
bool Tmpl_sql_plugin::fillScenes4Data()
{
    QSqlQuery query (DB_);
    QSqlQuery query_detail (DB_);

    QGraphicsScene *scene(0);
    QGraphicsItem *item(0);
    bool Ok =true;
    {
        //Модель tInfoModel заполнена возьмем оттуда данные
        //int templ_id    = tInfoModel->data(tInfoModel->index(0,tInfo_id)).toInt();
        int    angle    = tInfoModel->data(tInfoModel->index(0,tInfo_angle)).toInt();
        double m_top    = MM_TO_POINT(tInfoModel->data(tInfoModel->index(0,tInfo_mtop)).toInt());
        double m_bottom = MM_TO_POINT(tInfoModel->data(tInfoModel->index(0,tInfo_mbottom)).toInt());
        double m_left   = MM_TO_POINT(tInfoModel->data(tInfoModel->index(0,tInfo_mleft)).toInt());
        double m_right  = MM_TO_POINT(tInfoModel->data(tInfoModel->index(0,tInfo_mright)).toInt());
        double p_width  = MM_TO_POINT(tInfoModel->data(tInfoModel->index(0,tInfo_p_width)).toInt());
        double p_height = MM_TO_POINT(tInfoModel->data(tInfoModel->index(0,tInfo_p_height)).toInt());

        //Модель pagesModel заполнена возьмем оттуда данные о страницах шаблона
        for (int i=0;i<pagesModel->rowCount();i++){
            int pType = pagesModel->data(pagesModel->index(i,VPrn::PD_p_type)).toInt();
            scene = selectScene(pType);
            if (scene){
                create_page(scene,p_width,p_height,m_top,m_bottom,m_right,m_left);
            }
        }
        if (Ok){
            // Теперь запишем в созданную страницу элементы которые к ней принадлежат
            // Данные возьмем в модели ЭЛЕМЕНТЫ_СТРАНИЦЫ
            for (int i=0;i<elemInPageModel->rowCount();i++){
                int elem_p_type = elemInPageModel->data(elemInPageModel->index(i,VPrn::elem_p_type)).toInt();
                scene = selectScene(elem_p_type);
                if (scene){
                    item = this->findPaperElem(scene);
                    //Получим данные конкретного элемента
                    int e_type   = elemInPageModel->data(elemInPageModel->index(i,VPrn::elem_text_img)).toInt();
                    qreal pos_x = elemInPageModel->data(elemInPageModel->index(i,VPrn::elem_pos_x)).toDouble();
                    qreal pos_y = elemInPageModel->data(elemInPageModel->index(i,VPrn::elem_pos_y)).toDouble();
                    QPointF ps(pos_x,pos_y);
                    if (e_type==1){
                        QString text = elemInPageModel->data(elemInPageModel->index(i,VPrn::elem_text)).toString();
                        QString tag  = elemInPageModel->data(elemInPageModel->index(i,VPrn::elem_tag)).toString();
                        QVariant variant = elemInPageModel->data(elemInPageModel->index(i,VPrn::elem_color));
                        QColor color = variant.value<QColor>();
                        variant = elemInPageModel->data(elemInPageModel->index(i,VPrn::elem_font));
                        QFont font = variant.value<QFont>();
                        create_SimpleItem(item,ps,font,color,text,tag);
                    }else{
                        //Читаем картинку из базы
                    }
                    scene->update();
                }
            }
        }
    }
    return Ok;
}

void Tmpl_sql_plugin::create_SimpleItem(QGraphicsItem *parent,
                                    QPointF &ps, QFont &fnt,
                                    QColor &col,QString &text,QString &tag)
{
    QStringList filledList;

    SimpleItem * pItem = new SimpleItem;

    pItem->setPos(ps);
    pItem->setFont(fnt);
    pItem->setColor(col);
    pItem->setText(text);
    pItem->setTag(tag);
    pItem->setZValue(100);
    pItem->setFlag(QGraphicsItem::ItemIsMovable);
    pItem->setData(ObjectName, "tElem");
    pItem->setParentItem(parent);
}

void Tmpl_sql_plugin::create_page(QGraphicsScene * scene,
                                  qreal width,qreal height,
                                  qreal m_top,qreal m_bottom,
                                  qreal m_right,qreal m_left)
{
    if (scene){
        scene->clear();
        scene->setSceneRect(0, 0, width,height);
        scene->setBackgroundBrush(Qt::white);
        /// рисуем границы (@todo при печати надо их убирать)

        QGraphicsRectItem *paper_rect =
                new QGraphicsRectItem (QRectF(0,0, width,height));
        paper_rect->setPen(QPen(Qt::black));
        paper_rect->setBrush(QBrush(Qt::red));
        paper_rect->setZValue(-1000.0);
        paper_rect->setData(ObjectName, "Paper");
        scene->addItem(paper_rect);

        QGraphicsRectItem *border_rect =
                new QGraphicsRectItem (
                        QRectF(m_left, m_top,width-m_left-m_right,height-m_top-m_bottom)
                        );

        border_rect->setPen(QPen(Qt::black,2,Qt::DotLine));
        border_rect->setBrush(QBrush(Qt::white));
        border_rect->setOpacity(1);
        border_rect->setZValue(-900);
        border_rect->setData(ObjectName, "Border");
        border_rect->setParentItem(paper_rect);
    }
}

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
                Ok &=pragma.exec("PRAGMA foreign_keys = ON");
                if (!Ok){
                    DumpError(pragma.lastError());
                }
            }
        }
    }
    return Ok;
}

bool Tmpl_sql_plugin::isValidFileName(const QString & fileName)
{
    bool Ok = true;
    {
        if (fileName.isEmpty()){
            emit error(FileIOError, tr("Имя файла шаблона не заданно!"));
            Ok = false;
        }else{
            QRegExp rx("(.+)tmpl");
            if(rx.indexIn(fileName.toLower()) == -1){
                emit error(FileIOError,
                           tr("файла шаблона должен иметь расширение tmpl!"));
                Ok = false;
            }
        }
    }
    return Ok;
}

void Tmpl_sql_plugin::DumpError (const QSqlError & lastError)
{
    /*
    emit error(SQLCommonError,
               tr("Ошибка работы с БД шаблона. %1").arg(lastError.text()));
*/
    qDebug()<< "lastError.text() "        << lastError.text() << "\n"
            << "lastError.databaseText() "<< lastError.databaseText() << "\n"
            << "lastError.driverText()"   << lastError.driverText() << "\n"
            << "lastError.type() "        << lastError.type() << "\n"
            << "lastError.number() "      << lastError.number() << "\n";
}

bool Tmpl_sql_plugin::create_tables(QSqlQuery &query)
{
    bool Ok =true;
    {
        // Создаем пустую БД с прописанными значениями


        //Таблица описание страницы
        /**
          * p_number порядковый номер страницы в шаблоне
          * p_type тип страницы в шаблоне
          * (Первая [может быть несколько], остальные , фонарик)
          * p_name название страницы для отображения в редакторе
          */
        Ok &= query.exec("create table page_detail (id INTEGER primary key autoincrement, "
                         "p_number INTEGER,p_type INTEGER,p_name TEXT,p_visible INTEGER);"
                         );
        if (!Ok){
            DumpError(query.lastError());
        }
        //Таблица эементов
        Ok &= query.exec("CREATE TABLE elem (id INTEGER primary key autoincrement,"
                         "page_detail_id INTEGER,text TEXT,tag TEXT,"
                         "pos_x REAL,pos_y REAL,color TEXT,font TEXT,angle REAL,"
                         "border boolean,img INTEGER,img_data blob, always_view boolean); "
                         ); //"FOREIGN KEY (page_detail_id) REFERENCES page_detail (id) );"
        if (!Ok){
            DumpError(query.lastError());
        }
        //Таблица размеров страниц
        Ok &= query.exec("create table page_size (id INTEGER primary key autoincrement, "
                         "page_human TEXT,print_paper_size INTEGER,p_witdh INTEGER,"
                         "p_height INTEGER);"
                         );
        if (!Ok){
            DumpError(query.lastError());
        }

        //Таблица шаблон (основная таблица)
        Ok &= query.exec("create table template (id INTEGER primary key autoincrement, "
                         "angle INTEGER,c_time INTEGER,m_time INTEGER,author TEXT,"
                         "t_name TEXT, t_desc TEXT,"
                         "margin_top INTEGER,margin_bottom INTEGER,margin_left INTEGER,"
                         "margin_right INTEGER,page_size_id INTEGER, "
                         "FOREIGN KEY (page_size_id) REFERENCES page_size (id) ON UPDATE CASCADE);"
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

        Ok &= query.exec("CREATE INDEX tag_ix ON elem(tag);");
        if (!Ok){
            DumpError(query.lastError());
        }
    }
    return Ok;
}

int Tmpl_sql_plugin::getId4pageSizeTable(QSqlQuery &query,const QString & findSize)
{
    int pageSizeID=-1;
    bool Ok = true;
    {
        if (!findSize.isEmpty()) {
            Ok &= query.exec(QString("SELECT count(*) as cnt,id FROM page_size "
                                     "WHERE page_human='%1'").arg(findSize));
            if (Ok){
                int field_cnt = query.record().indexOf("cnt");
                int field_id  = query.record().indexOf("id");
                query.next();
                if (query.value(field_cnt).toInt()==1){
                    pageSizeID = query.value(field_id).toInt();
                }else{
                    emit error(VPrn::SQLCommonError,
                               tr("Ошибка разбора шаблона.\n"
                                  "Не найдена запись  [%1] в таблице размеров страниц").arg(findSize));
                }
            }else{
                qDebug() << "last query = " << query.lastQuery();
                DumpError(query.lastError());                
                emit error(VPrn::SQLCommonError, tr("Ошибка разбора шаблона."));
            }
        }
    }
    return   pageSizeID;
}

/**
  @brief При вызове функции соединение с БД должно быть установленно
  файл БД существуетн.
  */
bool Tmpl_sql_plugin::create_emptyDB(QString const&)
{
    bool Ok =true;
    {
        // Создаем пустую БД с прописанными значениями
        QSqlQuery query (DB_);
        int templ_id;
        Ok &= create_tables(query);
        /// Заполним таблицы размеры страниц
        Ok &= fillPagesSizeTable(query);
        // Ищем для шаблона по умолчанию значение page_size.id для А4
        int ps_id = getId4pageSizeTable(query,"A4 (210 x 297 мм)");
        //int ps_id = getId4pageSizeTable(query,"A3 (297 x 420 мм)");
        if (ps_id!=-1){

            Ok &= query.prepare("INSERT INTO template (t_name,t_desc,c_time,"
                                "m_time,author,angle,"
                                "margin_top,margin_bottom,margin_left,"
                                "margin_right,page_size_id) "
                                "VALUES(?,?,?,?,?,?,?,?,?,?,?);");
            if (Ok){

                query.addBindValue(tr("Новый шаблон"));
                query.addBindValue(tr("Тут можно кратко описать шаблон..."));
                query.addBindValue(QDateTime::currentDateTime().toTime_t());
                query.addBindValue(QDateTime::currentDateTime().toTime_t());
                query.addBindValue(userName);            
                query.addBindValue(90);
                query.addBindValue(10);
                query.addBindValue(15);
                query.addBindValue(30);
                query.addBindValue(10);
                query.addBindValue(ps_id);

                Ok &= query.exec();

                /// Получим id последней вставленной записи
                if (Ok){
                    templ_id = query.lastInsertId().toInt();
                    /// Создаем 4 основных страницы в шаблоне
                    Ok &= query.prepare("insert into page_detail (p_number,p_type,"
                                        "p_name,p_visible) VALUES(?,?,?,?);");
                    if (Ok){
                        query.addBindValue(1);
                        query.addBindValue(VPrn::FirstPage);

                        query.addBindValue(tr("Лицевая сторона 1-го листа.Экземпляр №1"));
                        query.addBindValue(1);
                        Ok &= query.exec();

                        query.addBindValue(2);
                        query.addBindValue(VPrn::FirstPage);

                        query.addBindValue(tr("Лицевая сторона 1-го листа.Экземпляр №2"));
                        query.addBindValue(1);
                        Ok &= query.exec();

                        query.addBindValue(3);
                        query.addBindValue(VPrn::FirstPage);
                        query.addBindValue(tr("Лицевая сторона 1-го листа.Экземпляр №3"));
                        query.addBindValue(1);
                        Ok &= query.exec();

                        query.addBindValue(4);
                        query.addBindValue(VPrn::FirstPage);

                        query.addBindValue(tr("Лицевая сторона 1-го листа.Экземпляр №4"));
                        query.addBindValue(1);
                        Ok &= query.exec();

                        query.addBindValue(5);
                        query.addBindValue(VPrn::FirstPage);

                        query.addBindValue(tr("Лицевая сторона 1-го листа.Экземпляр №5"));
                        query.addBindValue(1);
                        Ok &= query.exec();

                        query.addBindValue(6);
                        query.addBindValue(VPrn::SecondPage);

                        query.addBindValue(tr("Лицевая сторона 2-го листа"));
                        query.addBindValue(1);
                        Ok &= query.exec();

                        query.addBindValue(7);
                        query.addBindValue(VPrn::ThirdPage);

                        query.addBindValue(tr("Обратная сторона каждого листа"));
                        query.addBindValue(1);
                        Ok &= query.exec();

                        query.addBindValue(8);
                        query.addBindValue(VPrn::FourthPage);

                        query.addBindValue(tr("Фонарик"));
                        query.addBindValue(1);
                        Ok &= query.exec();
                        /// Запишем крепкие связи
                        Ok &= query.exec(QString("INSERT into rel_templ_page "
                                                 "(templ_id,page_detail_id) "
                                                 "select %1,id from page_detail"
                                                 ).arg(templ_id));
                    }else{
                        DumpError(query.lastError());
                    }
                }else{
                    DumpError(query.lastError());
                }
            }else{
                DumpError(query.lastError());
            }
        }else{
            Ok &= false;
        }
    }
    return Ok;
}

bool Tmpl_sql_plugin::fillPagesSizeTable(QSqlQuery &query)
{
    bool Ok = true;
    {
        /// Создаем базовый набор размеров страниц
        Ok &= query.prepare("insert into page_size (page_human,print_paper_size,"
                            "p_witdh,p_height) VALUES(?,?,?,?);");
        if (Ok){
            query.addBindValue("A4 (210 x 297 мм)");
            query.addBindValue(QPrinter::A4);
            query.addBindValue(210);
            query.addBindValue(297);
            Ok &= query.exec();

            query.addBindValue("A3 (297 x 420 мм)");
            query.addBindValue(QPrinter::A3);
            query.addBindValue(297);
            query.addBindValue(420);

            Ok &= query.exec();
            query.addBindValue("A0 (841 x 1189 мм)");
            query.addBindValue(QPrinter::A0);
            query.addBindValue(841);
            query.addBindValue(1189);
            Ok &= query.exec();

            query.addBindValue("A1 (594 x 841 мм)");
            query.addBindValue(QPrinter::A1);
            query.addBindValue(594);
            query.addBindValue(841);
            Ok &= query.exec();

            query.addBindValue("A2 (420 x 594 мм)");
            query.addBindValue(QPrinter::A2);
            query.addBindValue(420);
            query.addBindValue(594);
            Ok &= query.exec();

            query.addBindValue("A5 (148 x 210 мм)");
            query.addBindValue(QPrinter::A5);
            query.addBindValue(148);
            query.addBindValue(210);
            Ok &= query.exec();

            query.addBindValue("A6 (105 x 148 мм)");
            query.addBindValue(QPrinter::A6);
            query.addBindValue(105);
            query.addBindValue(148);
            Ok &= query.exec();

            query.addBindValue("A7 (74 x 105 мм)");
            query.addBindValue(QPrinter::A7);
            query.addBindValue(74);
            query.addBindValue(105);
            Ok &= query.exec();

            query.addBindValue("A8 (52 x 74 мм)");
            query.addBindValue(QPrinter::A8);
            query.addBindValue(52);
            query.addBindValue(74);
            Ok &= query.exec();

            query.addBindValue("A9 (37 x 52 мм)");
            query.addBindValue(QPrinter::A9);
            query.addBindValue(37);
            query.addBindValue(52);
            Ok &= query.exec();

            query.addBindValue("B0 (1000 x 1414 мм)");
            query.addBindValue(QPrinter::B0);
            query.addBindValue(1000);
            query.addBindValue(1414);
            Ok &= query.exec();

            query.addBindValue("B1 (707 x 1000 мм)");
            query.addBindValue(QPrinter::B1);
            query.addBindValue(707);
            query.addBindValue(1000);
            Ok &= query.exec();

            query.addBindValue("B2 (500 x 707 мм)");
            query.addBindValue(QPrinter::B2);
            query.addBindValue(500);
            query.addBindValue(707);
            Ok &= query.exec();

            query.addBindValue("B3 (353 x 500 мм)");
            query.addBindValue(QPrinter::B3);
            query.addBindValue(353);
            query.addBindValue(500);
            Ok &= query.exec();

            query.addBindValue("B4 (250 x 353 мм)");
            query.addBindValue(QPrinter::B4);
            query.addBindValue(250);
            query.addBindValue(353);
            Ok &= query.exec();

            query.addBindValue("B5 (176 x 250 мм)");
            query.addBindValue(QPrinter::B5);
            query.addBindValue(176);
            query.addBindValue(250);
            Ok &= query.exec();

            query.addBindValue("B6 (125 x 176 мм)");
            query.addBindValue(QPrinter::B6);
            query.addBindValue(125);
            query.addBindValue(176);
            Ok &= query.exec();

            query.addBindValue("B7 (88 x 125 мм)");
            query.addBindValue(QPrinter::B7);
            query.addBindValue(88);
            query.addBindValue(125);
            Ok &= query.exec();

            query.addBindValue("B8 (62 x 88 мм)");
            query.addBindValue(QPrinter::B8);
            query.addBindValue(62);
            query.addBindValue(88);
            Ok &= query.exec();

            query.addBindValue("B9 (44 x 62 мм)");
            query.addBindValue(QPrinter::B9);
            query.addBindValue(44);
            query.addBindValue(62);
            Ok &= query.exec();

            query.addBindValue("B10 (31 x 44 мм)");
            query.addBindValue(QPrinter::B10);
            query.addBindValue(31);
            query.addBindValue(44);
            Ok &= query.exec();

            query.addBindValue("C5E (163 x 229 мм)");
            query.addBindValue(QPrinter::C5E);
            query.addBindValue(163);
            query.addBindValue(229);
            Ok &= query.exec();

            query.addBindValue("DLE (110 x 220 мм)");
            query.addBindValue(QPrinter::DLE);
            query.addBindValue(110);
            query.addBindValue(220);
            Ok &= query.exec();

            query.addBindValue("Executive (191 x 254 мм)");
            query.addBindValue(QPrinter::Executive);
            query.addBindValue(191);
            query.addBindValue(254);
            Ok &= query.exec();

            query.addBindValue("Folio (210 x 330 мм)");
            query.addBindValue(QPrinter::Folio);
            query.addBindValue(210);
            query.addBindValue(330);
            Ok &= query.exec();

            query.addBindValue("Ledger (432 x 279 мм)");
            query.addBindValue(QPrinter::Ledger);
            query.addBindValue(432);
            query.addBindValue(279);
            Ok &= query.exec();

            query.addBindValue("Legal (216 x 356 мм)");
            query.addBindValue(QPrinter::Legal);
            query.addBindValue(216);
            query.addBindValue(356);
            Ok &= query.exec();

            query.addBindValue("Letter (216 x 279 мм)");
            query.addBindValue(QPrinter::Letter);
            query.addBindValue(216);
            query.addBindValue(279);
            Ok &= query.exec();

            query.addBindValue("Tabloid (279 x 432 мм)");
            query.addBindValue(QPrinter::Tabloid);
            query.addBindValue(279);
            query.addBindValue(432);
            Ok &= query.exec();
        }else{
            DumpError(query.lastError());
        }
    }
    return Ok;
}

bool Tmpl_sql_plugin::isCreateFile(const QString & fileName)
{
    bool Ok = true;
    {
        if (QFile::exists(fileName)){
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
                emit error(FileIOError,tr("Ошибка создания файла шаблона %1."
                                          "\nError %2")
                           .arg(fileName).arg(file.errorString()));
            }
        }
    }
    return Ok;
}

bool Tmpl_sql_plugin::fillModels()
{
    bool Ok =true;
    {
        pSizeModel->clear();
        tInfoModel->clear();
        pagesModel->clear();
        elemInPageModel->clear();

        /// Заполним модель (только для чтения) список размеров страниц
        pSizeModel->setQuery("select id, page_human,print_paper_size, "
                             "p_witdh,p_height  from page_size",DB_);
        pSizeModel->setHeaderData(pSize_id, Qt::Horizontal, tr("Id"));
        pSizeModel->setHeaderData(pSize_page, Qt::Horizontal, tr("Размер листа"));
        pSizeModel->setHeaderData(pSize_code, Qt::Horizontal, tr("Код"));
        pSizeModel->setHeaderData(pSize_width, Qt::Horizontal, tr("Ширина (мм)"));
        pSizeModel->setHeaderData(pSize_height, Qt::Horizontal, tr("Высота (мм)"));
        if (pSizeModel->lastError().isValid()){
            emit error(SQLQueryError,tr("Ошибка получения свойств шаблона. %1")
                       .arg(pSizeModel->lastError().text()));
            Ok &= false;
        }

        /// Заполним модель (только для чтения)  СТРАНИЦЫ_ШАБЛОНА
        pagesModel->setQuery("SELECT page_detail.id,page_detail.p_number,page_detail.p_type,"
                             " page_detail.p_name,page_detail.p_visible FROM template "
                             " INNER JOIN rel_templ_page ON templ_id= template.id"
                             " INNER JOIN page_detail ON page_detail_id =  page_detail.id"
                             " ORDER BY p_number",DB_);

        pagesModel->setHeaderData(VPrn::PD_id,       Qt::Horizontal, tr("Id"));
        pagesModel->setHeaderData(VPrn::PD_p_number, Qt::Horizontal, tr("Порядковый номер"));
        pagesModel->setHeaderData(VPrn::PD_p_type,   Qt::Horizontal, tr("Тип страницы"));
        pagesModel->setHeaderData(VPrn::PD_p_name,   Qt::Horizontal, tr("Название страницы"));

        if (pagesModel->lastError().isValid()){
            emit error(SQLQueryError,tr("Ошибка получения свойств шаблона. %1")
                       .arg(pagesModel->lastError().text()));
            Ok &= false;
        }
        /// Заполним модель (только для чтения) ЭЛЕМЕНТЫ_СТРАНИЦЫ
        elemInPageModel->setQuery("SELECT elem.id,text,tag,pos_x,pos_y,color,font,"
                                  "angle,border,img_data,always_view,page_detail.p_type "
                                  "img FROM elem "
                                  "INNER JOIN page_detail ON page_detail_id = page_detail.id "
                                  "WHERE page_detail.p_visible = 1 "
                                  "ORDER BY page_detail.p_type"
                                  ,DB_);
        elemInPageModel->setHeaderData(VPrn::elem_id,
                                       Qt::Horizontal, tr("Id"));
        elemInPageModel->setHeaderData(VPrn::elem_text,
                                       Qt::Horizontal, tr("Текст"));
        elemInPageModel->setHeaderData(VPrn::elem_tag,
                                       Qt::Horizontal, tr("Тэг"));
        elemInPageModel->setHeaderData(VPrn::elem_pos_x,
                                       Qt::Horizontal, tr("Позиция х"));
        elemInPageModel->setHeaderData(VPrn::elem_pos_y,
                                       Qt::Horizontal, tr("Позиция"));
        elemInPageModel->setHeaderData(VPrn::elem_color,
                                       Qt::Horizontal, tr("Цвет шрифта"));
        elemInPageModel->setHeaderData(VPrn::elem_font,
                                       Qt::Horizontal, tr("Шрифт"));
        elemInPageModel->setHeaderData(VPrn::elem_angle,
                                       Qt::Horizontal, tr("Угол поворота"));
        elemInPageModel->setHeaderData(VPrn::elem_border,
                                       Qt::Horizontal, tr("Граница элемента (1/0)"));
        elemInPageModel->setHeaderData(VPrn::elem_img_data,
                                       Qt::Horizontal, tr("Изображение"));
        elemInPageModel->setHeaderData(VPrn::elem_always_view,
                                       Qt::Horizontal, tr("Всегда видим (1/0)"));
        elemInPageModel->setHeaderData(VPrn::elem_p_type,
                                       Qt::Horizontal, tr("Страница шаблона"));
        elemInPageModel->setHeaderData(VPrn::elem_text_img,
                                       Qt::Horizontal, tr("Тип элемента текст/картинка (1/0)"));
        if (elemInPageModel->lastError().isValid()){
            emit error(SQLQueryError,tr("Ошибка получения свойств шаблона. %1")
                       .arg(elemInPageModel->lastError().text()));
            Ok &= false;
        }
        /// Чтение данных в редактируемую модель
        tInfoModel->refresh();

        if (tInfoModel->lastError().isValid()){
            emit error(SQLQueryError,tr("Ошибка получения свойств шаблона. %1")
                       .arg(tInfoModel->lastError().text()));
            Ok &= false;
        }
    }
    return Ok;
}

QGraphicsItem *Tmpl_sql_plugin::findPaperElem(QGraphicsScene *scene)
{
    QGraphicsItem *item;

    // Поиск  указателя на бумагу
    for (int i = 0; i < scene->items().size(); ++i){
        item = scene->items().at(i);
        if ( item->data(ObjectName).toString()=="Paper"){
            break;
        }
    }
    return item;
}

void Tmpl_sql_plugin::update_scenes(const QHash<QString, QString> &hash)
{
    QString t_str;
    QGraphicsScene *scene;
    QSqlQuery query (DB_);

    ///выбор страниц из базы
    bool Ok = true;
    {
        Ok &= query.exec("SELECT page_detail.p_number,page_detail.p_type,"
                         " page_detail.p_name FROM template "
                         " INNER JOIN rel_templ_page ON templ_id= template.id "
                         " INNER JOIN page_detail ON page_detail_id =  page_detail.id"
                         " ORDER BY p_number");
        if (Ok){
            //int field_id       = query.record().indexOf("id");
            //int field_p_number = query.record().indexOf("p_number");
            int field_pType    = query.record().indexOf("field_p_type");

            while (query.next()) {
                //page_detail_id = query.value(field_id).toInt();

                switch (query.value(field_pType).toInt()){
                case FirstPage:
                    scene = firstPage_scene;
                    break;
                case SecondPage:
                    scene = secondPage_scene;
                    break;
                case ThirdPage:
                    scene = thirdPage_scene;
                    break;
                case FourthPage:
                    scene = fourthPage_scene;
                    break;
                }
                if (scene){
                    //Обновим элементы сцены
                    for (int i = 0; i < scene->items().size(); i++){
                        QGraphicsItem *item = scene->items().at(i);
                        t_str=item->data(ObjectName).toString();
                        if (t_str==QString("tElem")){
                            SimpleItem* item =(SimpleItem* )scene->items().at(i);
                            t_str = item->getTag();
                            QHash<QString, QString>::const_iterator i = hash.find(t_str);
                            while (i != hash.end() && i.key() == t_str) {
                                item->setText(i.value());
                                ++i;
                            }

                        }
                    }
                }
            }
            //
        }else{
            DumpError(query.lastError());
        }
    }
}

//****************************************************************************
void Tmpl_sql_plugin::closeTemplates()
{

}


Q_EXPORT_PLUGIN2(Itmpl_sql_plugin, Tmpl_sql_plugin)
;




