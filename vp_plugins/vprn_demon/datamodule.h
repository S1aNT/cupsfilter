#ifndef DATAMODULE_H
#define DATAMODULE_H

#include <QObject>
/**
  * @class DataModule предназначен для работы с БД SQLITE
  * @brief Проверяет возможность раобы с драйвером БД SQLITE,
  * создает глобальную БД приложения хранящую данные:
  * @li о доступных шаблонах
  * @li подключенных клиентах (Мастер печати, Редактор шаблона, Генератор отчета)
  * @li текущем сеансе (личные настройки пользователя)
  * Работающий с созданной под конкретного клиента БД.

*/

#include "mytypes.h"
using namespace VPrn;

class DataModule : public QObject
{
        Q_OBJECT
public:
    explicit DataModule(QObject *parent = 0);

    bool isSqliteReady(){return db_state;}
    void setSpoolDir(const QString &spool);

signals:

    void error(VPrn::AppErrorType eCode, const QString &e_info);

private:
    bool db_state;
    QString spool_dir;
    /**
      * @fn void isSqliteReady();
      * @brief Проверка возможности работы с БД SQLITE
      */
    void checkSqliteReady();

    /**
      * @fn void createAppDB();
      * @brief Создает глобальную БД приложения
      */
    void createAppDB();
};

#endif // DATAMODULE_H
