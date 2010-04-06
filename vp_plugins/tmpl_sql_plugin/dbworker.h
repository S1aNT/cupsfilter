#ifndef DBWORKER_H
#define DBWORKER_H
/// @todo Займусь этим классом когда будет время
#define DBWORKER_DEBUG

#include "mytypes.h"
#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>

QT_FORWARD_DECLARE_CLASS ( QSqlQuery )
QT_FORWARD_DECLARE_CLASS ( QSqlQueryModel )
QT_FORWARD_DECLARE_CLASS ( QSqlError )
QT_FORWARD_DECLARE_CLASS ( QSqlTableModel )

QT_FORWARD_DECLARE_CLASS ( QAbstractItemModel )
QT_FORWARD_DECLARE_CLASS ( QSortFilterProxyModel )
QT_FORWARD_DECLARE_CLASS ( QStringList )

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QMap>

using namespace VPrn;

class dbWorker : public QObject
{
Q_OBJECT
public:
    explicit    dbWorker(QObject *parent = 0);
    /**
      * @fn bool isSQLDriverLoaded() {return m_sqlDriver;}
      * @brief Возвращяает результат проверки загрузки драйвера доступа к SqLite
      */
    bool isSQLDriverLoaded() {return m_sqlDriver;}

    QStringList getPagesList(){return m_pagesList;}
    QStringList getBaseElemList(){return m_baseElemList;}
    /**
      * @fn void client_init(const QString &client_id) const;
      * @brief Инициализация клиента, создаются модели с которыми он будет работать
      */
    void client_init(const QString &client_id);

    QAbstractItemModel *getTElemModel(const QString &client_id);
    QAbstractItemModel *getTInfoModel(const QString &client_id);

    /**
      * @fn void openTemplate (const QString &client_id,const QString &templates_fn);
      * @brief Клиент запросил открытие БД шаблона
      * @param const QString &client_id индетификатор клиента
      * @param const QString &templates_fn Файл шаблона
      */
    void openTemplate   (const QString &client_id,const QString &templates_fn);
    /**
      * @fn void saveTemplateAs (const QString &client_id,const QString &templates_fn);
      * @brief Клиент запросил сохранить открытую БД шаблона с другим именем
      * @param const QString &client_id индетификатор клиента
      * @param const QString &templates_fn Файл шаблона
      */
    void saveTemplateAs (const QString &client_id,const QString &templates_fn);
    /**
      * @fn void closeTemplate  (const QString &client_id);
      * @brief Клиент завершил работу с текущим шаблоном
      * @param const QString &client_id индетификатор клиента
      */
    void closeTemplate  (const QString &client_id);

signals:
    void error(pluginsError errCode,QString error_message);

public slots:
    /**
      * @fn void saveTemplateAs (const QString &client_id,const QString &templates_fn);
      * @brief Клиент запросил сохранить открытую БД шаблона,т.е сохранить
      * данные из моделей в соответсвующие таблицы
      */
    void saveTemplate   (const QString &client_id);
    /**
      * @fn void setTemplateInfo(const QString &client_id);
      * @brief Запись в БД  текущего шаблона информации о свойствах шаблона
      */
    void saveTemplateInfo(const QString &client_id);
    /**
      * @fn void setTemplateElem(const QString &client_id);
      * @brief Запись в текущий открытый шаблон,данных из хеша
      */
    void saveTemplateElem(const QString &client_id,QHash <QString,QString> &data);
private:
    bool m_sqlDriver;
    /**
      * @var clientsOpenTempalates;    Список: клиент_id, Имя_открытого_шаблона
      * @var clientsElemModel;         Список: клиент_id, ЭлементыНаВсехСтраницах
      * @var clientsTemplateInfoModel; Список: клиент_id, ИнформацияОШаблоне
      */
    QMap<QString,QString>                  clientsOpenTempalates;
    QMap<QString, QSortFilterProxyModel *> clientsElemModel;
    QMap<QString, QSortFilterProxyModel *> clientsTemplateInfoModel;

    /**
      * @var TElemModel; Общая на всех модель в которую записываются элементы шаблона
      * @var TInfoModel; Общая на всех модель в которую записываются информация о шаблоне
      */
    QAbstractItemModel *TElemModel;
    QAbstractItemModel *TInfoModel;

    QStringList m_pagesList;
    QStringList m_baseElemList;
    /**
      * @var Singleton DB with default connection
      */
    QSqlDatabase DB_;
    //-------------------------------------------------------------------------
    /**
      * @fn bool createConnection();
      * @brief Создаем соединение по умолчанию с БД sqlite заодно проверяя
      * есть ли данный драйвер в системе.
      * Каждый новый клиент использует сове сообственное сооединение с БД
      * используя в качестве имени соденинеия свой id
      */
    bool createConnection();

    void DumpError (const QSqlError & lastError);
    /**
      * @brief Предварительная настройка БД
      */
    bool InitDB (QSqlDatabase &db);

};

#endif // DBWORKER_H
