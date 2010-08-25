#ifndef SERVERGEARS_H
#define SERVERGEARS_H

#define DEBUG_MODE

#include <QtCore/QDebug>

#include "message.h"
#include "mytypes.h"
#include "inet_plugin.h"
#include "igs_plugin.h"
#include "itmpl_sql_plugin.h"


#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QSet>

#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>

#include "printtask.h"

#ifdef DEBUG_MODE

#include <QProcess>
#include <QDataStream>
#endif

class QByteArray;



class serverGears : public QLocalServer
{
    Q_OBJECT

public:
    explicit serverGears(QObject *parent = 0,const QString &srvName = QString());

    QString lastError() const {return e_info;}
    MyCheckPoints checkPoints() const;

    /**
      * @fn void setAuthData(const QString &userName = QString(),const QString &userMandat = QString());
      * @brief Сохранение данных пользователея Имя и Мандат
      */
    void setAuthData(const QString &userName = QString(),const QString &userMandat = QString());
    /**
      * @fn void setNetPlugin(Inet_plugin *NetPlugin);
      * @brief Запомним указатель на загруженный сетевой плагин. вся работа с сетью
      * идет только внутри @class serverGears
      */
    void setNetPlugin(Inet_plugin *NetPlugin);
    /**
      * @fn void setGsPlugin(Igs_plugin *GsPlugin);
      * @brief Запомним указатель на загруженный плагин. вся работа с ghostscript
      * идет только внутри @class serverGears
      */
    void setGsPlugin(Igs_plugin *GsPlugin);
    /**
      * @fn void setTmplPlugin(Igs_plugin *GsPlugin);
      * @brief Запомним указатель на загруженный плагин. вся работа с шаблонами
      * идет только внутри @class serverGears
      */
    void setTmplPlugin(Itmpl_sql_plugin *TmplPlugin);
    /**
      * @fn void sayGoodBayAllClients();
      * @brief Отправить всем клиентам сообщение завершить работу
      */
    void sayGoodBayAllClients();

    /**
      * @fn void findTemplatesInPath(const QString t_path);
      * @brief Поиск файлов шаблонов отвечающих маске *.tmpl в заданном каталоге
      */
    void findTemplatesInPath(const QString &t_path);

#ifdef DEBUG_MODE
    /**
      * @fn void setPrinterList( PrinterList &p_list);
      * @brief Только для отладки передаю список принтеров прочитанный из ini файла
      */
    void setPrinterList( PrinterList &p_list){ prn_list = p_list;}
#endif

signals:
    void messageReady( const Message &msg );
    void networkProtocolError();
    void checkPointChanged(MyCheckPoints cp);
    void error(VPrn::AppErrorType eCode, QString e_info);
    void clearClientSpool(const QString c_uuid);

public slots:
    void printFormatedDocuments(const QString c_uuid);
private slots:
    /**
      * @fn void readyRead()
      * @brief Клиент передает данные, прочтем  и определим что он там шлет
      */
    void readyRead();
    /**
      * @fn void reciveNetworkMessage(const Message &msg);
      * @brief Обработка сообщения полученного из сети
      */
    void reciveNetworkMessage(const Message &r_msg);
    /**
      * @fn void client_init();
      * @brief При подключении нового клиента заполняеет список клиентов,
      * присвает этому клиенту уникалный индетификатор, настраивает сигналы и слоты
      * локального сокета, обслуживающего данного клиента.
      */
    void client_init();
    /**
      * @fn void disconnected();
      * @brief Клиент покинул, наше общество! Вычеркнем предателя из списка живых
      */
    void disconnected();
    /**
      * @fn void prepareError(QLocalSocket::LocalSocketError socketError);
      * @brief Обработчик ошибок возникших в сокете
      */
    void prepareError(QLocalSocket::LocalSocketError socketError);
    /**
      * @fn void do_docReady4work (const QString &client_uuid,int pCount);
      * @brief Плагин gs_plugin преобразовал документ  ps->pdf,
      * и подсчитал число страниц в документе для клиента =  client_uuid
      */
    void do_docReady4work (const QString &client_uuid,int pCount);
    /**
      * @fn void do_docReady4print (const QString &client_uuid);
      * @brief  Плагин gs_plugin преобразовал выбранный пользователем шаблон в pdf
      * объединил исходный документ с шаблоном и расположол их в каталога по номерам копий
      */
    void do_docReady4print (const QString &client_uuid);
    /**
      * @fn void do_docReady4preview (const QString &client_uuid);
      * @brief Плагин gs_plugin преобразовал подготовленные к печати экз документа в png картинки с
      * выбранной пользователем глубиной детализации, только первые листы или весь документ
      */
    void do_docReady4preview (const QString &client_uuid);
    /**
      * @fn void do_docConvertedToPdf(const QString &client_uuid);
      * @brief Плагин gs_plugin преобразовал документ из ps в pdf
      */
    void do_docConvertedToPdf(const QString &client_uuid);
private:
    /**
      * @var m_serverName;     Имя локального сервера для общения с Локальным миром
      * @var packetSize;       Размер передаваемого блока данных
      * @var currentDataBlock; Текущий полученный блок данных
      * @var m_state;          Статус Локального сервера
      * @var e_info;           Последняя возникшая ошибка
      * @var net_plugin;       Указатель на сетевой плагин
      * @var gs_plugin;        Указатель на плагин работающий с ghostscript
      * @var tmpl_plugin;      Указатель на плагин работающий с шаблонами
      * @var u_login;          Текущий логин пользователя
      * @var u_mandat;         Текущий мандат пользователя
      * @var netDemonReady;    Истина если произошла успешная авторизация GateKeeper на сетевом демоне
      * @var t_list;           Список путей к файдам шаблона

      * @var clients;          Список подключенных клиентов
      * @var clients_uuid;     Список UUID подключенных клиентов
      * @var clients_name;     Список имен подключенных клиентов
      */

    QString              m_serverName;
    QLocalServer         *m_server;
    qint32               packetSize;

    QString              e_info;
    Inet_plugin          *net_plugin;
    Igs_plugin           *gs_plugin;
    Itmpl_sql_plugin     *tmpl_plugin;
    QString              u_login;
    QString              u_mandat;
    bool                 netDemonReady;
    MyCheckPoints        m_checkPoint;
    QStringList          templatesFileInfoList;
    QSet<QLocalSocket *> clients;    
    QMap<QLocalSocket *,QString> clients_uuid;
    QMap<QLocalSocket *,QString> clients_name;
    QMap<QString,QString> clientsPrintTask; // clients_uuid,taskID
    PrinterList prn_list;
    /**
      * @var QMap <QString,PrintTask> prn_taskList;
      * @brief очередь заданий на печать для клиента с cliend_id
      */

    QMap <QString,PrintTask *> printTaskList;
    //-----------------------------------------------------------------------------
    /**
      * @fn void markDocInBaseAsFault(const QString &client_uuid,
                                      const QString &data_str);
      * Пометка документа с данными МБ,номером экз. и тд. как брак
      */
    void markDocInBaseAsFault(const QString &client_uuid,
                              const QString &data_str);
    /**
      * @fn void printCurrentDoc(const QString &client_uuid,QString printer_queue);
      * @brief Печать текущего сформированнного документа
      */
    void printCurrentFormatedDoc(const QString &client_uuid,QString printer_queue);

    /**
      * @fn void createFormatedDoc(const QString &client_uuid,
                                    VPrn::PreviewMode prn_mode,QByteArray data);
      * @brief Формирует документ, готовит pdf из шаблона, объединяет их с документом
      * Определяет весь документ конвертировать в png или только первые страницы
      */
    void createFormatedDoc(const QString &client_uuid,
                           VPrn::PreviewMode prn_mode,QByteArray data);

    /**
      * @fn void parseMessage( const Message &msg, const QString &c_uuid);
      * @brief Обработка сообщения от конкретного клиента
      */
    void parseMessage( const Message &m_msg, const QString &c_uuid);

    /**
      * @fn void sendMessage( const Message &msg, const QLocalSocket *client);
      * @brief Отправка сообщения конкретному клиенту
      */
    void sendMessage( const Message &m_msg, QLocalSocket *client);

     /**
      * @fn void setCheckPoint(MyCheckPoints cp);
      * @brief Сохраняет последнюю пройденную контрольну точку и испускает
      * сигнал с этим радостным событием.
      */
    void setCheckPoint(MyCheckPoints cp);
    /**
      * @fn QLocalSocket *findClient(const QString &c_uuid);
      * @brief Ищет по uuid зарегистрированного клиента и возвращает
      * указатель на него или 0
      * @returns QLocalSocket * or 0
      */
    QLocalSocket *findClient(const QString &c_uuid);

     /**
       * @fn qint64 getCompresedFile(const QString &fileName,
       *                                 QByteArray &data);
       * @brief Читает существующий файл во временный буфер,
       * сжимает его  с помощью zlib и пишет в data
       */
     qint64 getCompresedFile(const QString &fileName,
                                        QByteArray &data);
     void createPrintTask(const QString &client_uuid,
                          const QString &printer_queue,
                          const QString &first_page,
                          const QString &other_page,
                          const QString &over_page,
                          const QString &last_page);

     void splitListToFile(const QStringList fList,
                          QString &first_page,
                          QString &other_page,
                          QString &over_page,
                          QString &last_page );
};

#endif // SERVERGEARS_H
