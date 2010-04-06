#include "dbworker.h"
#include <QtCore/QStringList>

dbWorker::dbWorker(QObject *parent)
    : QObject(parent)
    , m_sqlDriver(false)
{
    qRegisterMetaType<pluginsError>("pluginsError");
    m_baseElemList
            << QObject::trUtf8("МБ")
            << QObject::trUtf8("Название док-та")
            << QObject::trUtf8("Гриф")
            << QObject::trUtf8("Пункт перечня")
            << QObject::trUtf8("Номер экз.")
            << QObject::trUtf8("Кол-во экз.")
            << QObject::trUtf8("Кол-во листов")
            << QObject::trUtf8("Исполнитель")
            << QObject::trUtf8("Отпечатал")
            << QObject::trUtf8("Телефон")
            << QObject::trUtf8("Инв. N")
            << QObject::trUtf8("Дата распечатки")
            << QObject::trUtf8("Получатель N1")
            << QObject::trUtf8("Получатель N2")
            << QObject::trUtf8("Получатель N3")
            << QObject::trUtf8("Получатель N4")
            << QObject::trUtf8("Получатель N5");

    m_sqlDriver =createConnection();
}

//--------------------------------- PUBLIC  FUNCTION --------------------------

void dbWorker::client_init(const QString &client_id)
{
    // Проверка что с данным клиентом не связанны модели
    QMap<QString,QAbstractItemModel *>::const_iterator
           find_item = clientsElementsInPages.find(client_id);
    if ( find_item != clientsElementsInPages.end() ){

    }


}

void dbWorker::openTemplate   (const QString &client_id,
                               const QString &templates_fn)
{
    //Проверим нет ли уже открытого соединения этим клиентом с БД
    if (QSqlDatabase::contains(client_id,Qt::CaseInsensitive)){
        //Закроем соединение
        QSqlDatabase::database(client_id,false).close();
        QSqlDatabase::removeDatabase(client_id);
        clientsOpenTempalates.remove(clients_id);
    }else{
        //Новое соединение с драйвером
        QSqlDatabase db = QSqlDatabase::database(client_id,true);
        bool Ok = true;
        {
            db.setDatabaseName(templates_fn);
            Ok &= db.open();
            if (Ok){
                Ok &= InitDB(db);
                //Запишем в список
                clientsOpenTempalates.insert(clients_id,templates_fn);
                // Прочтем данные из шаблона в модели

            }else{
                db.close();
                db.removeDatabase(client_id);
            }
        }
    }

}

//--------------------------------- PRIVATE FUNCTION --------------------------
bool dbWorker::createConnection()
{
    bool Ok = true;
    {
        DB_ = QSqlDatabase::addDatabase(QLatin1String("QSQLITE"));
        if (DB_.driver()
            && DB_.driver()->lastError().type() == QSqlError::ConnectionError) {
            emit error(DriverNotLoad,
                       QObject::trUtf8("Не могу загрузить драйвер sqlite!"));
            Ok = false;
        }

    }
    return Ok;
}

bool dbWorker::InitDB(QSqlDatabase &db)
{
    QSqlQuery pragma (db);
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

void dbWorker::DumpError (const QSqlError & lastError)
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
