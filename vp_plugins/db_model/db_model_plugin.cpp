#include "db_model_plugin.h"

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QTemporaryFile>

#include <QtCore/QStringList>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QList>
#include <QStandardItem>

DB_model_plugin::DB_model_plugin(QObject *parent)
        : QObject(parent)
        , spoolDir (QString())
{
}

DB_model_plugin::~DB_model_plugin()
{
}

//----------------------------- PUBLIC -----------------------------------------
void DB_model_plugin::init(const QString &spool)
{
    QDir dir;
    if (dir.cd(spool) ) {
        spoolDir = spool;
        // Создадим временный файл в каталоге spool
        QTemporaryFile file;
        file.setFileTemplate(QString("%1/XXXXXXXX").arg(spoolDir));
        if (file.open()) {
            // Инициализация БД, проверка наличия требуемого драйвера
            {
                QSqlDatabase db  = QSqlDatabase::addDatabase(QLatin1String("QSQLITE"),
                                                             QLatin1String("TEST_CON"));
                if (db.driver()->lastError().type() != QSqlError::ConnectionError) {
                    qDebug() << "SQLITE driver exist and loaded!";
                    // Создаем необходимые модели
                    emit readyToWork();
                }else{
                    DumpError( db.driver()->lastError() );
                    emit error(VPrn::SqlDriverNotLoad,
                               QObject::trUtf8("Не могу загрузить драйвер sqlite!\n%1")
                               .arg(QString(Q_FUNC_INFO)));
                }
            }
            QSqlDatabase::removeDatabase(QLatin1String("TEST_CON"));
        }
    }else{
        emit error(VPrn::InternalAppError,
                   QObject::trUtf8("Каталог %1 не существует или нет прав доступа\n%2")
                   .arg(spool)
                   .arg(QString(Q_FUNC_INFO))
                   );
    }     
}

void DB_model_plugin::readTemplate(const QString &cuid, const QString &fileName,
                                   QStandardItemModel *tInfo,
                                   QStandardItemModel *pages,
                                   QStandardItemModel *elements)
{
    // Все неприятности проверим в начале
    if (cuid.isEmpty()){
        emit error(VPrn::InternalAppError,
                   QObject::trUtf8("Не задан индентификатор клиента!\n%1")
                   .arg(QString(Q_FUNC_INFO))
                   );
        return;
    }
    if (!QFile::exists(fileName)){
        emit error(VPrn::FileNotFound,
                   QObject::trUtf8("Файл шаблона не найден %1!\n%2")
                   .arg(fileName)
                   .arg(QString(Q_FUNC_INFO))
                   );
        return;
    }
    if (!(tInfo && pages && elements)){
        emit error(VPrn::InternalAppError,
                   QObject::trUtf8("Модели данных шаблона не определены!\n%1")
                   .arg(QString(Q_FUNC_INFO))
                   );
        return;
    }

    // Инициализация БД, проверка наличия требуемого драйвера
    {
        QSqlDatabase db  = QSqlDatabase::addDatabase(QLatin1String("QSQLITE"),
                                                     cuid);
        if (db.driver()->lastError().type() != QSqlError::ConnectionError) {
            qDebug() << "SQLITE driver exist and loaded!";
            // Началась работа
            db.setDatabaseName( fileName );
            if (db.open() ){
                if ( InitDB(db) ){
                    // Заполним модели данными из БД шаблона
                    fillTemplatesInfo(db,tInfo,fileName); // ИНФОРМАЦИЯ_О_ШАБЛОНЕ
                    fillPagesInfo(db,pages);              // СТРАНИЦЫ_ШАБЛОНА
                    fillElementsInfo(db,elements);       // ЭЛЕМЕНТЫ_НА_СТРАНИЦАХ
                }else{
                    emit error(VPrn::SQLCommonError,
                               QObject::trUtf8("Ошибка выполнения SQL запроса!\n%1")
                               .arg(QString(Q_FUNC_INFO)));
                }
            }else{
                emit error(VPrn::SQLCommonError,
                           QObject::trUtf8("Ошибка открыбия БД шаблона!\n%1")
                           .arg(QString(Q_FUNC_INFO)));
            }
            db.close();
        }else{
            DumpError( db.driver()->lastError() );
            emit error(VPrn::SqlDriverNotLoad,
                       QObject::trUtf8("Не могу загрузить драйвер sqlite!\n%1")
                       .arg(QString(Q_FUNC_INFO)));
        }
    }
    QSqlDatabase::removeDatabase(cuid);
}
//--------------------------------- PRIVATE ------------------------------------
bool DB_model_plugin::InitDB(QSqlDatabase DB_)
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

void DB_model_plugin::DumpError (const QSqlError & lastError)
{
    qDebug()<< "lastError.text() "        << lastError.text() << "\n"
            << "lastError.databaseText() "<< lastError.databaseText() << "\n"
            << "lastError.driverText()"   << lastError.driverText() << "\n"
            << "lastError.type() "        << lastError.type() << "\n"
            << "lastError.number() "      << lastError.number() << "\n";
}

void DB_model_plugin::fillTemplatesInfo(QSqlDatabase DB_,
                                        QStandardItemModel *model,
                                        const QString &fileName)
{
    QSqlQuery query(DB_);
    QList<QStandardItem *> itemList;

    QString sql_txt = QObject::trUtf8("SELECT t_name,t_desc,"
                                      "angle,c_time,m_time,author,margin_top,margin_bottom,"
                                      "margin_left,margin_right,page_size.p_width,page_size.p_height "
                                      "FROM template "
                                      " INNER JOIN page_size ON template.page_size_id=page_size.id");
    if (query.exec(sql_txt) ){
        int field_t_name   = query.record().indexOf("t_name");
        int field_t_desc   = query.record().indexOf("t_desc");
        int field_angle    = query.record().indexOf("angle");
        int field_c_time   = query.record().indexOf("c_time");
        int field_m_time   = query.record().indexOf("m_time");
        int field_author   = query.record().indexOf("author");
        int field_m_top    = query.record().indexOf("margin_top");
        int field_m_bottom = query.record().indexOf("margin_bottom");
        int field_m_left   = query.record().indexOf("margin_left");
        int field_m_right  = query.record().indexOf("margin_right");
        int field_p_width  = query.record().indexOf("p_width");
        int field_p_height = query.record().indexOf("p_height");
        while (query.next()) {

            itemList.append(new QStandardItem (
                    query.value(field_t_name).toString() )
                            );
            itemList.append(new QStandardItem( fileName )
                            );
            itemList.append(new QStandardItem (
                    query.value(field_t_desc).toString() )
                            );
            itemList.append(new QStandardItem (
                    query.value(field_angle).toString() )
                            );
            itemList.append(new QStandardItem (
                    query.value(field_c_time).toString() )
                            );
            itemList.append(new QStandardItem (
                    query.value(field_m_time).toString() )
                            );
            itemList.append(new QStandardItem (
                    query.value(field_author).toString() )
                            );
            itemList.append(new QStandardItem (
                    query.value(field_m_top).toString() )
                            );
            itemList.append(new QStandardItem (
                    query.value(field_m_bottom).toString() )
                            );
            itemList.append(new QStandardItem (
                    query.value(field_m_left).toString() )
                            );
            itemList.append(new QStandardItem (
                    query.value(field_m_right).toString() )
                            );
            itemList.append(new QStandardItem (
                    query.value(field_p_width).toString() )
                            );
            itemList.append(new QStandardItem (
                    query.value(field_p_height).toString() )
                            );
        }

        model->appendRow(itemList);
    }else{
        DumpError(query.lastError());
        emit error(VPrn::SQLCommonError,
                   QObject::trUtf8("Ошибка выполнения SQL запроса [%1]!\n%2")
                   .arg( query.lastError().text() )
                   .arg(QString(Q_FUNC_INFO))
                   );
    }
}

void DB_model_plugin::fillPagesInfo(QSqlDatabase DB_,QStandardItemModel *model)
{
    QSqlQuery query(DB_);
    QList<QStandardItem *> itemList;
    QString sql_txt = QObject::trUtf8("SELECT page_detail.id,page_detail.p_number,"
                                      " page_detail.p_name,page_detail.p_visible FROM template "
                                      " INNER JOIN rel_templ_page ON templ_id= template.id"
                                      " INNER JOIN page_detail ON page_detail_id =  page_detail.id"
                                      " ORDER BY p_number");
    if (query.exec(sql_txt) ){
        int field_p_number   = query.record().indexOf("p_number");
        int field_p_name  = query.record().indexOf("p_name");
        int field_p_visible    = query.record().indexOf("p_visible");

        while (query.next()) {
            itemList.append(new QStandardItem (
                    query.value(field_p_number).toString() )
                            );
            itemList.append(new QStandardItem (
                    query.value(field_p_name).toString() )
                            );
            itemList.append(new QStandardItem (
                    query.value(field_p_visible).toString() )
                            );
        }
        model->appendRow(itemList);
    }else{
        DumpError(query.lastError());
        emit error(VPrn::SQLCommonError,
                   QObject::trUtf8("Ошибка выполнения SQL запроса [%1]!\n%2")
                   .arg( query.lastError().text() )
                   .arg(QString(Q_FUNC_INFO))
                   );
    }
}

void DB_model_plugin::fillElementsInfo(QSqlDatabase DB_,QStandardItemModel *model)
{
    QSqlQuery query(DB_);
    QList<QStandardItem *> itemList;
    QString sql_txt = QObject::trUtf8("SELECT elem.id,e_text,e_tag,pos_x,pos_y,color,font,"
                                      "angle,border,img_data,img_scaled, always_view,page_detail.p_number,img"
                                      " FROM elem "
                                      " INNER JOIN page_detail ON page_detail_id = page_detail.id "
                                      " WHERE page_detail.p_visible = 1 "
                                      " ORDER BY page_detail.p_number ");
    if (query.exec(sql_txt) ){
        int field_e_text      = query.record().indexOf("e_text");
        int field_e_tag       = query.record().indexOf("e_tag");
        int field_pos_x       = query.record().indexOf("pos_x");
        int field_pos_y       = query.record().indexOf("pos_y");
        int field_color       = query.record().indexOf("color");
        int field_font        = query.record().indexOf("font");
        int field_angle       = query.record().indexOf("angle");
        int field_border      = query.record().indexOf("border");
        int field_img_data    = query.record().indexOf("img_data");
        int field_img_scaled  = query.record().indexOf("img_scaled");
        int field_always_view = query.record().indexOf("always_view");
        int field_p_number    = query.record().indexOf("p_number");
        int field_img         = query.record().indexOf("img");

        while (query.next()) {
            itemList.append(new QStandardItem (
                    query.value(field_e_text).toString() )
                            );
            itemList.append(new QStandardItem (
                    query.value(field_e_tag).toString() )
                            );
            itemList.append(new QStandardItem (
                    query.value(field_pos_x).toDouble() )
                            );
            itemList.append(new QStandardItem (
                    query.value(field_pos_y).toDouble() )
                            );
            itemList.append(new QStandardItem (
                    query.value(field_color).toString() )
                            );
            itemList.append(new QStandardItem (
                    query.value(field_font).toString() )
                            );
            itemList.append(new QStandardItem (
                    query.value(field_angle).toDouble() )
                            );
            itemList.append(new QStandardItem (
                    query.value(field_border).toInt() )
                            );

            {
                /** @remarks Перед использованием надо произвести
                  * обратную конвертацию fromBase64()
                  */
                QByteArray pixBuf = query.value(field_img_data)
                                                .toByteArray().toBase64();
                itemList.append(new QStandardItem ( QString(pixBuf) )
                            );

                qDebug() << "field_img_data "<< pixBuf
                         << " size: "        << pixBuf.size();
            }
            itemList.append(new QStandardItem (
                    query.value(field_img_scaled).toDouble() )
                            );
            itemList.append(new QStandardItem (
                    query.value(field_always_view).toInt() )
                            );
            itemList.append(new QStandardItem (
                    query.value(field_p_number).toInt() )
                            );
            itemList.append(new QStandardItem (
                    query.value(field_img).toInt() )
                            );
        }
        model->appendRow(itemList);
    }else{
        DumpError(query.lastError());
        emit error(VPrn::SQLCommonError,
                   QObject::trUtf8("Ошибка выполнения SQL запроса [%1]!\n%2")
                   .arg( query.lastError().text() )
                   .arg(QString(Q_FUNC_INFO))
                   );
    }
}
Q_EXPORT_PLUGIN2(IDB_model_plugin_plugin, DB_model_plugin);

