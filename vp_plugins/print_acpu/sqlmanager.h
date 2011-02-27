#ifndef SQLMANAGER_H
#define SQLMANAGER_H

#include <QObject>
//#include <QtSql/QSqlDatabase>
#include <QStandardItemModel>

#include "message.h"
#include "mytypes.h"
using namespace VPrn;

class sqlManager : public QObject
{
    Q_OBJECT
public:
    explicit sqlManager(QObject *parent = 0);
    ~sqlManager();
    void setModel ( QStandardItemModel* model = 0 );

public slots:
    /**
      * @fn void convertModelToMsg ( int rec_number);
      * @brief Преобразует карточку документа, сохранненую в модели
      * @sa sqlManager::m_model в сообщение типа @sa Message
      * @param int rec_number Номер записи в модели содержащей карточки документов
      */
    void convertModelToMsg(int rec_number);
    /**
      * @fn void findDocCardInBase(const QString &mb, const QString &copy_number);
      * @brief Поиск в БД учета докумета с атрибутами
      * @param QString mb          Номер мб
      * @param QString copy_number Номер экземпляра
      * @param int     find_type   Режим поиска (имеющие метку)
      * 0 - Отпечатан и Чистовик
      * 1 - Отпечатан и брак
      * 2 - Учтен
      */
    void findDocCardInBase(const QString &mb, const QString &copy_number,int find_type);

signals:
    void error(VPrn::AppErrorType errCode,const QString &error_message);
    /**
      * @fn void sendMessageToDemon( const Message &msg );
      * @brief Стандизированное сообщение для отправки локальному демону
      */
    void sendMessageToDemon( const Message &msg );

private:
    QStandardItemModel* m_model;
    //  void DumpError (const QSqlError & lastError);
};

#endif // SQLMANAGER_H
