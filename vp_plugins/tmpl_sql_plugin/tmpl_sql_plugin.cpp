#include "tmpl_sql_plugin.h"
#include "mytextitem.h"
#include "picitem.h"

#include <QtCore/QDebug>
#include <QtCore/QHashIterator>
#include <QtCore/QTemporaryFile>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QMetaType>
#include <QtCore/QModelIndex>
#include <QtCore/QDateTime>
#include <QtCore/QDataStream>
#include <QtCore/QBuffer>

#include <QtGui/QStandardItemModel>
#include <QtGui/QPrinter>
#include <QtGui/QPainter>
#include <QtGui/QPixmap>

#include <QtSql/QSqlError>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlTableModel>



using namespace VPrn;

Tmpl_sql_plugin::Tmpl_sql_plugin(QObject *parent)
        : QObject(parent)
{
    m_dbOpened      = false;
    m_dbConnect     = false;
    view_code_state = false;
    m_connectionName = QLatin1String("TCONN");

    qRegisterMetaType<VPrn::AppErrorType>("AppErrorType");
    qRegisterMetaType<pSizeColumnOrder>("pSizeColumnOrder");

    //Создаем группу стеков Undo
    undoGrp = new QUndoGroup (this);
    //Создаем сцены, связываем их со стеком Undo
    for (int i=1; i<9;i++){
        QUndoStack * stack = new QUndoStack(this);
        undoGrp->addStack(stack);
        scenesGrp.insert(i,new myScene(i,stack,this));
    }

    m_dbConnect = createConnection();
    m_dbOpened = false;
    if (m_dbConnect){
        /// Создаем модели
        elemInPageModel = new QSqlTableModel(this);
        pSizeModel = new QSqlQueryModel(this);
        pagesModel = new EditPagesModel(this);
        tInfoModel = new TemplateInfoEditModel(this);
    }
    // Заполним список базовых элементов шаблона

    baseElemList.insert(VPrn::cards_DOC_NAME,    QObject::trUtf8("Название док-та"));
    baseElemList.insert(VPrn::cards_STAMP,       QObject::trUtf8("Гриф"));
    baseElemList.insert(VPrn::cards_MB_NUMBER,   QObject::trUtf8("МБ"));
    baseElemList.insert(VPrn::cards_PUNKT, 	 QObject::trUtf8("Пункт перечня"));
    baseElemList.insert(VPrn::cards_PAGE_COUNT,  QObject::trUtf8("Кол-во листов"));
    baseElemList.insert(VPrn::cards_COPY_COUNT,  QObject::trUtf8("Кол-во экз."));
    baseElemList.insert(VPrn::cards_CURRENT_COPY,QObject::trUtf8("Номер экз."));
    baseElemList.insert(VPrn::cards_EXECUTOR,    QObject::trUtf8("Исполнитель"));
    baseElemList.insert(VPrn::cards_PRINTMAN,    QObject::trUtf8("Отпечатал"));
    baseElemList.insert(VPrn::cards_PHONE,       QObject::trUtf8("Телефон"));
    baseElemList.insert(VPrn::cards_INV_NUMBER,  QObject::trUtf8("Инв. N"));
    baseElemList.insert(VPrn::cards_PRINT_DATE,  QObject::trUtf8("Дата распечатки"));
    baseElemList.insert(VPrn::cards_RECIVER_1,   QObject::trUtf8("Получатель N1"));
    baseElemList.insert(VPrn::cards_RECIVER_2,   QObject::trUtf8("Получатель N2"));
    baseElemList.insert(VPrn::cards_RECIVER_3,   QObject::trUtf8("Получатель N3"));
    baseElemList.insert(VPrn::cards_RECIVER_4,   QObject::trUtf8("Получатель N4"));
    baseElemList.insert(VPrn::cards_RECIVER_5,   QObject::trUtf8("Получатель N5"));

}

Tmpl_sql_plugin::~Tmpl_sql_plugin()
{
    if (DB_.isOpen()){
        DB_.close();
    }
    //DB_.removeDatabase();//m_connectionName);
}

//****************************** public functions ******************************

void Tmpl_sql_plugin::init(const QString & spool)
{
    QDir dir;
    if (dir.cd(spool) ) {
        spoolDir = spool;
    }else{
        emit error(VPrn::InternalAppError,
                   QObject::trUtf8("Каталог %1 не существует или нет прав доступа\n%2")
                                    .arg(spool)
                                    .arg(QString(Q_FUNC_INFO))
                                    );
    }
}

QStringList Tmpl_sql_plugin::getBaseElemNameList() const
{
    return QStringList()<< baseElemList.values()
            << QObject::trUtf8("Произвольный текст")
            << QObject::trUtf8("Идентификатор Выч.Системы");
}

void Tmpl_sql_plugin::setViewMode()
{
    myScene * scene(0);
    int p_visible;
    int p_number;

    for (int i=0;i<pagesModel->rowCount();i++){
        p_visible = pagesModel->data(pagesModel->index(i,VPrn::PD_p_visible)).toInt();
        if ( p_visible == 1 ){
            p_number = pagesModel->data(pagesModel->index(i,VPrn::PD_p_number)).toInt();
            /// Поиск myScene соответсвующего странице с номером
            scene = scenesGrp.value( p_number );
            if (scene){
                scene->setViewMode();
            }
        }
    }
}




bool Tmpl_sql_plugin::prepare_template(const QString &c_uuid,
                                       const QString &t_fileName,
                                       QMap <int,QString> elemMap,
                                       int copy_number)
{

    myScene *m_scene(0);
    QPrinter pdfprinter;    
    QString path = QString("%1/%2/%3-copy/").arg(spoolDir,c_uuid).arg(copy_number,0,10);
    m_scene = new myScene();    

    bool Ok =true;
    {
        {
            QSqlDatabase db  = QSqlDatabase::addDatabase(QLatin1String("QSQLITE"),c_uuid);            
            if (db.driver()->lastError().type() != QSqlError::ConnectionError) {                
                // Загружаем шаблон
                Ok = isValidFileName(t_fileName) &&  QFile::exists(t_fileName);
                if (Ok){
                    db.setDatabaseName( t_fileName );                    
                    if (db.open() ){
                        // Создаем модели
                        QSqlQueryModel *elemInPageModel_client  = new QSqlQueryModel(this);
                        QSqlQueryModel *pagesModel_client           = new QSqlQueryModel(this);
                        QSqlQueryModel *pagesDetail                      = new QSqlQueryModel(this);
                        Ok &= saveDataToBase(elemMap,db);// Обновим данные элементов
                        if (Ok){
                            // Заполнение моделей
                            Ok &= selectIntoModel(db,pagesModel_client,
                                                  QString ("SELECT angle, page_size.p_width,page_size.p_height"
                                                           " FROM template "
                                                           " INNER JOIN page_size ON template.page_size_id=page_size.id")
                                                  );
                            Ok &=  selectIntoModel(db,elemInPageModel_client,
                                                   QString ("SELECT elem.id,e_text,e_tag,pos_x,pos_y,color,font,"
                                                            "angle,border,img_data,img_scaled, always_view,page_detail.p_number,img"
                                                            " FROM elem "
                                                            " INNER JOIN page_detail ON page_detail_id = page_detail.id "
                                                            " WHERE page_detail.p_visible = 1 "
                                                            " ORDER BY page_detail.p_number ")
                                                   );
                            Ok &=  selectIntoModel(db,pagesDetail,
                                                   QString ("SELECT p_number FROM page_detail WHERE p_visible =1")
                                                   );
                            if (Ok){
                                // Получим размер страницы и ее ориентацию
                                qreal p_width;
                                qreal p_height;
                                int   p_angle;
                                for (int i=0;i<pagesModel_client->rowCount();i++){
                                    p_angle  = pagesModel_client->data(pagesModel_client->index(i,0)).toInt();
                                    p_width  = MM_TO_POINT(pagesModel_client->data(pagesModel_client->index(i,1)).toDouble());
                                    p_height = MM_TO_POINT(pagesModel_client->data(pagesModel_client->index(i,2)).toDouble());
                                }

                                pdfprinter.setOutputFormat(QPrinter::PdfFormat);
                                pdfprinter.setResolution( QPrinter::HighResolution );
                                // Формирование ориентации страниц шаблона
                                if (p_angle == 90){
                                    pdfprinter.setOrientation(QPrinter::Landscape);
                                }else{
                                    pdfprinter.setOrientation(QPrinter::Portrait);
                                }

                                // Цикл по всем страницам шаблона
                                int p_number;
                                QString  page_fn;
                                int current_page;
                                int t_page=0;
                                while ( t_page<pagesDetail->rowCount() ){
                                    p_number  = pagesDetail->data(
                                            pagesDetail->index(t_page,0)).toInt();
                                    page_fn.clear();
                                    switch (p_number){
                                    case VPrn::FirstPage:
                                    case VPrn::FirstPageN2:
                                    case VPrn::FirstPageN3:
                                    case VPrn::FirstPageN4:
                                    case VPrn::FirstPageN5:
                                        // Формируем лицевую сторону
                                        if (p_number == copy_number){
                                            page_fn = path;
                                            page_fn.append("t_firstpage.pdf");
                                        }
                                        break;
                                    case VPrn::SecondPage:
                                        page_fn = path;
                                        page_fn.append("t_otherpage.pdf");
                                        break;
                                    case VPrn::ThirdPage:
                                        page_fn = path;
                                        page_fn.append("t_oversidepage.pdf");
                                        break;
                                    case VPrn::FourthPage:
                                        page_fn = path;
                                        page_fn.append("t_lastpage.pdf");
                                        break;
                                    }
                                    if (!page_fn.isEmpty())    {
                                        // Начнем формировать новую страницу
                                        pdfprinter.setOutputFileName(page_fn);

                                        // Создаем сцену
                                        m_scene->createPageForPrint(p_width,p_height);

                                        // Цикл по всем элементам шаблона
                                        for (int i=0;i<elemInPageModel_client->rowCount();i++){
                                            current_page = elemInPageModel_client->data(elemInPageModel_client->index(i,VPrn::elem_p_number)).toInt();

                                            if (p_number == current_page){
                                                // Элемент принадлежит текущей странице, добавим его
                                                // Получим все данные элемента и сформируем его
                                                int e_type  = elemInPageModel_client->data(elemInPageModel_client->index(i,VPrn::elem_text_img)).toInt();
                                                qreal pos_x = elemInPageModel_client->data(elemInPageModel_client->index(i,VPrn::elem_pos_x)).toDouble();
                                                qreal pos_y = elemInPageModel_client->data(elemInPageModel_client->index(i,VPrn::elem_pos_y)).toDouble();
                                                QPointF ps(pos_x,pos_y);
                                                qreal angle      = elemInPageModel_client->data(elemInPageModel_client->index(i,VPrn::elem_angle)).toDouble();

                                                if (e_type == 0){
                                                    QString text     = elemInPageModel_client->data(elemInPageModel_client->index(i,VPrn::elem_text)).toString();
                                                    QString tag      = elemInPageModel_client->data(elemInPageModel_client->index(i,VPrn::elem_tag)).toString();
                                                    QVariant variant = elemInPageModel_client->data(elemInPageModel_client->index(i,VPrn::elem_color));
                                                    QColor color     = variant.value<QColor>();
                                                    variant          = elemInPageModel_client->data(elemInPageModel_client->index(i,VPrn::elem_font));
                                                    QFont font       = variant.value<QFont>();

                                                    m_scene->addBaseElem(tag,text,ps,font,color,angle);
                                                }else{
                                                    //Читаем картинку из базы
                                                    QPixmap img;
                                                    img.fill(Qt::transparent);
                                                    qreal scale_xy   = elemInPageModel_client->data(elemInPageModel_client->index(i,VPrn::elem_img_scaled)).toDouble();
                                                    QByteArray pixBuf = elemInPageModel_client->data(elemInPageModel_client->index(i,VPrn::elem_img_data)).toByteArray();
                                                    img.loadFromData(pixBuf);
                                                    m_scene->addImgElem(ps,angle,scale_xy,img);
                                                }
                                            }
                                        }
                                        //Печать в pdf, обнуление всего
                                        QPainter painter(&pdfprinter);
                                        painter.setOpacity(0.0);

                                        /// Если последний режим был показа тегов, то переключим на показ значений
                                        if (m_scene->getViewMode()){
                                            m_scene->setViewMode();
                                        }
                                        m_scene->render(&painter);
                                        //m_scene->clear();
                                    }
                                    t_page++;
                                }
                            }
                        }
                        db.close();
                    }else{
                        emit error(VPrn::FileIOError,
                                   QObject::trUtf8("Файл [%1] шаблона не существует!\n%2")
                                   .arg(t_fileName)
                                   .arg(QString(Q_FUNC_INFO))
                                   );
                        Ok &= false;
                    }
                }else{
                    emit error(VPrn::SqlDriverNotLoad,
                               QObject::trUtf8("Не могу загрузить драйвер sqlite!\n%1")
                               .arg(QString(Q_FUNC_INFO)));
                    Ok &=false;
                }
            }
        }
        QSqlDatabase::removeDatabase(c_uuid);

        return Ok;
    }
}

//------------------------------------------------------------------------------
bool Tmpl_sql_plugin::createConnection()
{
    bool openingOk = true;
    {
        DB_ = QSqlDatabase::addDatabase(QLatin1String("QSQLITE"));
        if (DB_.driver()
            && DB_.driver()->lastError().type() == QSqlError::ConnectionError) {
            emit error(VPrn::SqlDriverNotLoad,
                       QObject::trUtf8("Не могу загрузить драйвер sqlite!\n%1")
                       .arg(QString(Q_FUNC_INFO)));
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
            emit error(VPrn::DBOpeningError,
                       QObject::trUtf8("Не могу открыть файл шаблона: %. Ошибка %2\n%3")
                       .arg(t_fileName).arg(DB_.lastError().text()).arg(QString(Q_FUNC_INFO)));
            DumpError(DB_.lastError());
            DB_.close();
            //DB_.removeDatabase();//m_connectionName);
        }
    }
    return  Ok;
}

//************************* Public slots *************************************

void Tmpl_sql_plugin::convert2Pdf()
{
    /*
    myScene * scene(0);
    int p_visible;
    int p_number;
    QString fileName;
    QPrinter pdfprinter;

    bool Ok = true;
    {
        Ok &= isDBConnected() && isDBOpened();
        if (Ok){
            for (int i=0; i < pagesModel->rowCount(); i++){
                p_visible = pagesModel->data(pagesModel->index(i,VPrn::PD_p_visible)).toInt();
                if ( p_visible == 1 ){
                    p_number = pagesModel->data(pagesModel->index(i,VPrn::PD_p_number)).toInt();
                    /// Поиск myScene соответсвующего странице с номером
                    scene = scenesGrp.value( p_number );
                    /// Поиск соответсвующего имени файла в который будем сохранять сцену
                    fileName = filesGrp.value( p_number );
                    if (scene && !fileName.isEmpty()){
                        if (QFile::exists(fileName)){
                            Ok = QFile::remove(fileName);
                        }
                        // Определим ориентацию страницы
                        if (scene->getAngle() == 0.0){
                            pdfprinter.setOrientation(QPrinter::Portrait);
                        }else{
                            pdfprinter.setOrientation(QPrinter::Landscape);
                        }
                        /// Если последний режим был показа тегов, то переключим на показ значений
                        if (scene->getViewMode()){
                            scene->setViewMode();
                        }

                        pdfprinter.setOutputFormat(QPrinter::PdfFormat);
                        pdfprinter.setOutputFileName(fileName);
                        QPainter painter(&pdfprinter);
                        scene->render(&painter);
                    }else{
                        Ok &=false;
                        emit error(VPrn::InternalAppError,
                                   QObject::trUtf8("Ошибка преобразования шаблона в pdf.\n"
                                                   "Нарушенна структура шаблона"));
                    }
                }
            }
        }else{
            emit error(VPrn::InternalAppError,
                       QObject::trUtf8("Ошибка преобразования шаблона в pdf.\n"
                                       "Необходимо вначале открыть существующий или создать новый шаблон"));
        }
    }
    if (Ok){
        emit allPagesConverted();
    }
    */
}

void Tmpl_sql_plugin::openTemplates(const QString & t_fileName)
{
    bool Ok = true;
    {
        /// Проверка что соединение с БД установленно (драйвер был загружен)
        Ok &= isDBConnected() &&
              isValidFileName(t_fileName) &&
              QFile::exists(t_fileName);
        if (Ok){
            Ok &=openDataBase(t_fileName);
            if (Ok){
                Ok &= fillModels();
                // Рабор данных полученных из шаблона и запись их в сцены
                fillScenes4Data();

                if (Ok){
                    emit allTemplatesPagesParsed();
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
                    emit error(VPrn::SQLCommonError,
                               QObject::trUtf8("Ошибка создания пустого шаблона в файле %1\n%2")
                               .arg(currentDBFileName).arg(QString(Q_FUNC_INFO)));
                }
            }
        }else{
            Ok = false;
            emit error(VPrn::FileNotFound,QObject::trUtf8("Ошибка создания временного файла %1\n%2")
                       .arg(currentDBFileName).arg(QString(Q_FUNC_INFO)));
        }
    }
    m_dbOpened = Ok;
}

void Tmpl_sql_plugin::saveTemplates()
{
    bool Ok = true;
    {
        QSqlQuery query(DB_);
        /// Проверка что соединение с БД установленно (драйвер был загружен)
        Ok &= isDBConnected() && isDBOpened();
        if (Ok){
            /// Формируем SQL запрос на очистку таблицы от всех элементов для всех страниц
            Ok &= query.exec("DELETE FROM elem ");
            if (Ok){
                int p_visible;
                int p_number;
                int p_id;
                myScene *scene(0);
                myTextItem *tElem(0);
                PicItem *tImg(0);

                QGraphicsItem *item;

                QString elem_type;
                QVariant t_var;
                for (int i=0;i<pagesModel->rowCount();i++){
                    p_visible = pagesModel->data(pagesModel->index(i,VPrn::PD_p_visible)).toInt();
                    if ( p_visible == 1 ){
                        p_number = pagesModel->data(pagesModel->index(i,VPrn::PD_p_number)).toInt();
                        /// Поиск myScene соответсвующего странице с номером
                        scene = scenesGrp.value( p_number );
                        if (scene){
                            p_id = pagesModel->data(pagesModel->index(i,VPrn::PD_id)).toInt();
                            /// Формируем SQL запрос на INSERT таблицы elem
                            Ok &= query.prepare("INSERT INTO elem (page_detail_id,e_text,e_tag,"
                                                "pos_x,pos_y,color,font,angle,border,"
                                                "img_data,img_scaled,img,always_view ) "
                                                "VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?)"
                                                );
                            if (Ok){
                                ///Цикл по все элементам сцены
                                for (int j = 0; j < scene->items().size(); j++){
                                    item = scene->items().at(j);
                                    elem_type = item->data(ObjectName).toString();

                                    if (elem_type == "tElem"){
                                        tElem =(myTextItem* )scene->items().at(j);

                                        query.addBindValue( p_id );
                                        query.addBindValue( tElem->toPlainText() );
                                        query.addBindValue( tElem->getETag()   );
                                        query.addBindValue( tElem->pos().x()  );
                                        query.addBindValue( tElem->pos().y()  );
                                        t_var = tElem->defaultTextColor();
                                        query.addBindValue( t_var.toString()  );
                                        t_var = tElem->font();
                                        query.addBindValue( t_var.toString()  );
                                        query.addBindValue( tElem->getAngle() );
                                        query.addBindValue( 0 );
                                        query.addBindValue( 0 );
                                        query.addBindValue( 0 );
                                        query.addBindValue( 0 );
                                        query.addBindValue( 1 );
                                        query.exec();
                                    }
                                    if (elem_type =="tImg"){
                                        tImg =(PicItem* )scene->items().at(j);

                                        query.addBindValue( p_id );
                                        query.addBindValue( "" );
                                        query.addBindValue( "" );
                                        query.addBindValue( tImg->pos().x()  );
                                        query.addBindValue( tImg->pos().y()  );
                                        query.addBindValue( ""  );
                                        query.addBindValue( ""  );
                                        query.addBindValue( tImg->getAngle() );
                                        query.addBindValue( 0 );

                                        // Преобразует картинку в QByteArray для записи в базу
                                        QByteArray img_bytes;
                                        QBuffer buffer(&img_bytes);
                                        buffer.open(QIODevice::WriteOnly);
                                        tImg->pixmap().save(&buffer, "PNG");
                                        query.addBindValue( img_bytes );

                                        query.addBindValue( tImg->getScaledSize() );
                                        query.addBindValue( 1 );
                                        query.addBindValue( 1 );
                                        query.exec();

                                    }
                                }
                            }else{
                                emit error (VPrn::SQLQueryError,
                                            QObject::trUtf8("Ошибка [%1] при заполнении шаблона\n%2")
                                            .arg( query.lastError().text())
                                            .arg( QString( Q_FUNC_INFO ) )
                                            );
                            }
                        }else{
                            emit error (VPrn::SQLQueryError,
                                        QObject::trUtf8("Ошибка [%1] при очистке шаблона\n%2")
                                        .arg( query.lastError().text() )
                                        .arg(QString(Q_FUNC_INFO))
                                        );
                        }
                    }
                }
            }
        }else{
            emit error(VPrn::InternalAppError,
                       QObject::trUtf8("Ошибка сохранения шаблона.\n"
                                       "Необходимо вначале открыть существующий или создать новый шаблон\n%1")
                       .arg(QString(Q_FUNC_INFO))
                       );
        }
    }
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
                emit error(VPrn::FileIOError,
                           QObject::trUtf8("Ошибка сохранения шаблона по заданному пути %1\n%2")
                           .arg(fileName)
                           .arg(QString(Q_FUNC_INFO))
                           );
            }
        }
    }
}

void Tmpl_sql_plugin::doAddBaseElementToPage(int page,const QString &tag)
{
    myScene *scene = scenesGrp.value(page);
    if (scene){
        scene->addBaseElem(tag);
    }else{
        emit error(VPrn::InternalAppError,
                   QObject::trUtf8("Ошибка добавления элемента %1 в шаблон\n%2")
                   .arg(tag).arg(QString(Q_FUNC_INFO)));
    }
}

void Tmpl_sql_plugin::doAddImgElementToPage(int page,const QString &file_name)
{
    myScene *scene = scenesGrp.value(page);
    if (scene){
        if (QFile::exists(file_name)){
            scene->AddImgElement(file_name);
        }else{
            emit error(VPrn::InternalAppError,
                       QObject::trUtf8("Графический элемент %1 не найден!\n%2")
                       .arg(file_name).arg(QString(Q_FUNC_INFO)));

        }
    }else{
        emit error(VPrn::InternalAppError,
                   QObject::trUtf8("Ошибка добавления графического элемента в шаблон из файла %1\n%2")
                   .arg(file_name).arg(QString(Q_FUNC_INFO)));
    }
}

//***************** private functions **************************************

/// @short При вызове функций соединение с БД должно быть установленно

// Рабор данных полученных из шаблона и запись их в сцены
void Tmpl_sql_plugin::fillScenes4Data()
{
    // QSqlQuery query (DB_);
    // QSqlQuery query_detail (DB_);

    myScene *scene(0);

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
        int pNumber = pagesModel->data(pagesModel->index(i,VPrn::PD_p_number)).toInt();
        //scene = selectScene(pNumber);
        scene = scenesGrp.value(pNumber);
        if (scene){
            //Чистим стек Undo
            scene->undoStack()->clear();
            scene->createPage(p_width,p_height,m_top,m_bottom,m_right,m_left);
            scene->setAngle(angle);
        }
    }

    // Теперь запишем в созданную страницу элементы которые к ней принадлежат
    // Данные возьмем в модели ЭЛЕМЕНТЫ_СТРАНИЦЫ
    for (int i=0;i<elemInPageModel->rowCount();i++){
        int elem_p_number = elemInPageModel->data(elemInPageModel->index(i,VPrn::elem_p_number)).toInt();
        scene = scenesGrp.value(elem_p_number);
        if (scene ){
            //Получим данные конкретного элемента
            int e_type  = elemInPageModel->data(elemInPageModel->index(i,VPrn::elem_text_img)).toInt();
            qreal pos_x = elemInPageModel->data(elemInPageModel->index(i,VPrn::elem_pos_x)).toDouble();
            qreal pos_y = elemInPageModel->data(elemInPageModel->index(i,VPrn::elem_pos_y)).toDouble();
            QPointF ps(pos_x,pos_y);
            if (e_type == 0){
                QString text     = elemInPageModel->data(elemInPageModel->index(i,VPrn::elem_text)).toString();
                QString tag      = elemInPageModel->data(elemInPageModel->index(i,VPrn::elem_tag)).toString();
                QVariant variant = elemInPageModel->data(elemInPageModel->index(i,VPrn::elem_color));
                QColor color     = variant.value<QColor>();
                variant          = elemInPageModel->data(elemInPageModel->index(i,VPrn::elem_font));
                QFont font       = variant.value<QFont>();
                qreal angle      = elemInPageModel->data(elemInPageModel->index(i,VPrn::elem_angle)).toDouble();
                scene->addBaseElem(tag,text,ps,font,color,angle);
            }else{
                //Читаем картинку из базы
                QPixmap img;
                img.fill(Qt::transparent);

                qreal angle      = elemInPageModel->data(elemInPageModel->index(i,VPrn::elem_angle)).toDouble();
                qreal scale_xy   = elemInPageModel->data(elemInPageModel->index(i,VPrn::elem_img_scaled)).toDouble();
                QByteArray pixBuf = elemInPageModel->data(elemInPageModel->index(i,VPrn::elem_img_data)).toByteArray();
                img.loadFromData(pixBuf);
                scene->addImgElem(ps,angle,scale_xy,img);
            }
            scene->update();
        }
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
        QRegExp rx("(.+)tmpl");
        if(rx.indexIn(fileName.toLower()) == -1){
            emit error(VPrn::FileIOError,
                       QObject::trUtf8("Файла шаблона должен иметь расширение tmpl!\n%1")
                       .arg(QString(Q_FUNC_INFO)));
            Ok = false;
        }
    }
    return Ok;
}

void Tmpl_sql_plugin::DumpError (const QSqlError & lastError)
{
    /*
    emit error(SQLCommonError,
               QObject::trUtf8("Ошибка работы с БД шаблона. %1").arg(lastError.text()));
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
                         "p_number INTEGER,p_name TEXT,p_visible INTEGER);"
                         );
        if (!Ok){
            DumpError(query.lastError());
        }
        //Таблица эементов
        Ok &= query.exec("CREATE TABLE elem (id INTEGER primary key autoincrement,"
                         "page_detail_id INTEGER,e_text TEXT,e_tag TEXT,"
                         "pos_x REAL,pos_y REAL,color TEXT,font TEXT,angle REAL,"
                         "border boolean,img_data blob,img_scaled REAL, always_view boolean,img INTEGER); "
                         ); //"FOREIGN KEY (page_detail_id) REFERENCES page_detail (id) );"
        if (!Ok){
            DumpError(query.lastError());
        }
        //Таблица размеров страниц
        Ok &= query.exec("create table page_size (id INTEGER primary key autoincrement, "
                         "page_human TEXT,print_paper_size INTEGER,p_width INTEGER,"
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

        Ok &= query.exec("CREATE INDEX tag_ix ON elem(e_tag);");
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
                               QObject::trUtf8("Ошибка разбора шаблона.\n"
                                               "Не найдена запись [%1] в таблице размеров страниц\n%2")
                               .arg(findSize).arg(QString(Q_FUNC_INFO)));
                }
            }else{
                qDebug() << "last query = " << query.lastQuery();
                DumpError(query.lastError());
                emit error(VPrn::SQLCommonError, QObject::trUtf8("Ошибка разбора шаблона.\n%1").arg(QString(Q_FUNC_INFO)));
            }
        }
    }
    return   pageSizeID;
}

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

                query.addBindValue(QObject::trUtf8("Новый шаблон"));
                query.addBindValue(QObject::trUtf8("Тут можно кратко описать шаблон..."));
                query.addBindValue(QDateTime::currentDateTime().toTime_t());
                query.addBindValue(QDateTime::currentDateTime().toTime_t());
                query.addBindValue(userName);
                query.addBindValue(0);
                query.addBindValue(10);
                query.addBindValue(15);
                query.addBindValue(30);
                query.addBindValue(10);
                query.addBindValue(ps_id);

                Ok &= query.exec();

                /// Получим id последней вставленной записи
                if (Ok){
                    templ_id = query.lastInsertId().toInt();
                    /// Создаем 8 основных страницы в шаблоне
                    Ok &= query.prepare("insert into page_detail (p_number,"
                                        "p_name,p_visible) VALUES(?,?,?);");
                    if (Ok){

                        query.addBindValue(VPrn::FirstPage);
                        query.addBindValue(QObject::trUtf8("Лицевая сторона 1-го листа.Экземпляр №1"));
                        query.addBindValue(1);
                        Ok &= query.exec();

                        query.addBindValue(VPrn::FirstPageN2);
                        query.addBindValue(QObject::trUtf8("Лицевая сторона 1-го листа.Экземпляр №2"));
                        query.addBindValue(1);
                        Ok &= query.exec();

                        query.addBindValue(VPrn::FirstPageN3);
                        query.addBindValue(QObject::trUtf8("Лицевая сторона 1-го листа.Экземпляр №3"));
                        query.addBindValue(1);
                        Ok &= query.exec();

                        query.addBindValue(VPrn::FirstPageN4);
                        query.addBindValue(QObject::trUtf8("Лицевая сторона 1-го листа.Экземпляр №4"));
                        query.addBindValue(1);
                        Ok &= query.exec();

                        query.addBindValue(VPrn::FirstPageN5);
                        query.addBindValue(QObject::trUtf8("Лицевая сторона 1-го листа.Экземпляр №5"));
                        query.addBindValue(1);
                        Ok &= query.exec();

                        query.addBindValue(VPrn::SecondPage);
                        query.addBindValue(QObject::trUtf8("Лицевая сторона 2-го листа"));
                        query.addBindValue(1);
                        Ok &= query.exec();

                        query.addBindValue(VPrn::ThirdPage);
                        query.addBindValue(QObject::trUtf8("Обратная сторона каждого листа"));
                        query.addBindValue(1);
                        Ok &= query.exec();

                        query.addBindValue(VPrn::FourthPage);
                        query.addBindValue(QObject::trUtf8("Фонарик"));
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
                            "p_width,p_height) VALUES(?,?,?,?);");
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
                emit error(VPrn::FileIOError,
                           QObject::trUtf8("Ошибка удаления временного файла %1\n%2")
                           .arg(fileName).arg(QString(Q_FUNC_INFO)));
            }
        }else{
            QFile file(fileName);
            Ok &= file.open(QIODevice::WriteOnly);
            if (Ok){
                file.close();
                Ok &= file.remove();
            }else{
                emit error(VPrn::FileIOError,QObject::trUtf8("Ошибка создания файла шаблона %1. %2\n%3")
                           .arg(fileName).arg(file.errorString()).arg(QString(Q_FUNC_INFO))
                           );
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
                             "p_width,p_height  from page_size",DB_);
        pSizeModel->setHeaderData(pSize_id, Qt::Horizontal, QObject::trUtf8("Id"));
        pSizeModel->setHeaderData(pSize_page, Qt::Horizontal, QObject::trUtf8("Размер листа"));
        pSizeModel->setHeaderData(pSize_code, Qt::Horizontal, QObject::trUtf8("Код"));
        pSizeModel->setHeaderData(pSize_width, Qt::Horizontal, QObject::trUtf8("Ширина (мм)"));
        pSizeModel->setHeaderData(pSize_height, Qt::Horizontal, QObject::trUtf8("Высота (мм)"));
        if (pSizeModel->lastError().isValid()){
            emit error(VPrn::SQLQueryError,QObject::trUtf8("Ошибка получения свойств шаблона. %1\n%2")
                       .arg(pSizeModel->lastError().text()).arg(QString(Q_FUNC_INFO)));
            Ok &= false;
        }

        /// Заполним модель (только для чтения)  СТРАНИЦЫ_ШАБЛОНА
        pagesModel->refresh();

        if (pagesModel->lastError().isValid()){
            emit error(VPrn::SQLQueryError,QObject::trUtf8("Ошибка получения свойств шаблона. %1\n%2")
                       .arg(pagesModel->lastError().text()).arg(QString(Q_FUNC_INFO)));
            Ok &= false;
        }
        /// @todo МОДЕЛЬ переделать на чтение запись
        /// Заполним модель (только для чтения) ЭЛЕМЕНТЫ_СТРАНИЦЫ
        elemInPageModel->setQuery("SELECT elem.id,e_text,e_tag,pos_x,pos_y,color,font,"
                                  "angle,border,img_data,img_scaled, always_view,page_detail.p_number,img"
                                  " FROM elem "
                                  " INNER JOIN page_detail ON page_detail_id = page_detail.id "
                                  " WHERE page_detail.p_visible = 1 "
                                  ,DB_);
        elemInPageModel->setHeaderData(VPrn::elem_id,
                                       Qt::Horizontal, QObject::trUtf8("Id"));
        elemInPageModel->setHeaderData(VPrn::elem_text,
                                       Qt::Horizontal, QObject::trUtf8("Текст"));
        elemInPageModel->setHeaderData(VPrn::elem_tag,
                                       Qt::Horizontal, QObject::trUtf8("Тэг"));
        elemInPageModel->setHeaderData(VPrn::elem_pos_x,
                                       Qt::Horizontal, QObject::trUtf8("Позиция х"));
        elemInPageModel->setHeaderData(VPrn::elem_pos_y,
                                       Qt::Horizontal, QObject::trUtf8("Позиция"));
        elemInPageModel->setHeaderData(VPrn::elem_color,
                                       Qt::Horizontal, QObject::trUtf8("Цвет шрифта"));
        elemInPageModel->setHeaderData(VPrn::elem_font,
                                       Qt::Horizontal, QObject::trUtf8("Шрифт"));
        elemInPageModel->setHeaderData(VPrn::elem_angle,
                                       Qt::Horizontal, QObject::trUtf8("Угол поворота"));
        elemInPageModel->setHeaderData(VPrn::elem_border,
                                       Qt::Horizontal, QObject::trUtf8("Граница элемента (1/0)"));
        elemInPageModel->setHeaderData(VPrn::elem_img_data,
                                       Qt::Horizontal, QObject::trUtf8("Изображение"));
        elemInPageModel->setHeaderData(VPrn::elem_img_scaled,
                                       Qt::Horizontal, QObject::trUtf8("Масштаб"));
        elemInPageModel->setHeaderData(VPrn::elem_always_view,
                                       Qt::Horizontal, QObject::trUtf8("Всегда видим (1/0)"));
        elemInPageModel->setHeaderData(VPrn::elem_p_number,
                                       Qt::Horizontal, QObject::trUtf8("Страница шаблона"));
        elemInPageModel->setHeaderData(VPrn::elem_text_img,
                                       Qt::Horizontal, QObject::trUtf8("Тип элемента текст/картинка (1/0)"));
        if (elemInPageModel->lastError().isValid()){
            emit error(VPrn::SQLQueryError,QObject::trUtf8("Ошибка получения свойств шаблона. %1\n%2")
                       .arg(elemInPageModel->lastError().text()).arg(QString(Q_FUNC_INFO)));
            Ok &= false;
        }
        /// Чтение данных в редактируемую модель
        tInfoModel->refresh();

        if (tInfoModel->lastError().isValid()){
            emit error(VPrn::SQLQueryError,QObject::trUtf8("Ошибка получения свойств шаблона. %1\n%2")
                       .arg(tInfoModel->lastError().text()).arg(QString(Q_FUNC_INFO)));
            Ok &= false;
        }
    }
    return Ok;
}

bool Tmpl_sql_plugin::saveDataToBase(const QMap <int,QString> elemMap,
                                     QSqlDatabase db)
{
    QSqlQuery query (db);
    bool Ok = true;
    {
        QMapIterator<int, QString> i(elemMap);
        while (i.hasNext()) {
            i.next();
            QString str = baseElemList.value(i.key());
            Ok &= query.exec(
                    QString("UPDATE elem SET e_text='%1' "
                            "WHERE img = 0 AND e_tag ='%2' ").arg( i.value(),str )
                    );
        }

    }
    if (!Ok){
        qDebug() << query.lastError().text();
        emit error(VPrn::SQLQueryError,QObject::trUtf8("Ошибка обновления элементов шаблона. %1\n%2")
                   .arg( query.lastError().text() ).arg(QString(Q_FUNC_INFO))
                   );
    }
    return Ok;
}


//****************************************************************************

bool Tmpl_sql_plugin::selectIntoModel(QSqlDatabase db, QSqlQueryModel *model,
                                      const QString &sql)
{
    bool Ok =true;
    {
        model->setQuery(sql,db);

        if (model->lastError().isValid()){
            Ok &=false;
            emit error(VPrn::SQLQueryError,
                       QObject::trUtf8("Ошибка получения элементов шаблона. %1\n%2")
                       .arg( model->lastError().text() ).arg(QString(Q_FUNC_INFO))
                       );
        }
    }
    return Ok;
}
Q_EXPORT_PLUGIN2(Itmpl_sql_plugin, Tmpl_sql_plugin)
        ;




