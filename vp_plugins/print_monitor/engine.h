#ifndef ENGINE_H
#define ENGINE_H
#include "QtCore/QObject"
#include <QtCore/QFileInfo>

QT_FORWARD_DECLARE_CLASS ( QStringList )
QT_FORWARD_DECLARE_CLASS ( QLocalSocket )
QT_FORWARD_DECLARE_CLASS ( QStandardItemModel )

class mySocketClient;
class Message;
class TemplatesInfo;


#include "mytypes.h"
#include "igs_plugin.h"


using namespace VPrn;

class Engine :public QObject
{
    Q_OBJECT
public:

    Engine(QObject*parent = 0,const QString &app_path = QString(),
           const QString &client_name = QString());
    /**
      * @fn ~Engine();
      * @brief В деструкторе проверяем живость соденения и если оно есть прибиваем его
      */
    ~Engine();

    /**
      * @fn QStandardItemModel * getInfoModel();
      * @brief Возвращает указатель на заполненную модель метаинформации о шаблоне
      */
    QStandardItemModel * getInfoModel();

    /**
      * @fn QStandardItemModel * getPrintersModel();
      * @brief Возвращает указатель на модель содержащую информацию о принтерах
      */
    QStandardItemModel * getPrintersModel();

    void init();
    /**
      @fn void prepareFileToPrint(const QString & file_name);
      @brief Отпаравляет запрос на GateKeeper с требованием конвертировать ps -> pdf
      */
    void prepareFileToPrint(const QString & file_name);
    /**
      * @fn bool isError();
      * @brief Если ошибка вернет истину
      */
    bool isError(){return e_state;}
    /**
      * @fn QString lastError()
      * @brief Возвращает текстовое описание последней ошибки
      */
    QString lastError() const {return e_info;}

    /**
      * @fn  void setUserMandat(const QString & u_name,const QString & u_mandat);
      * @brief При авторизации с консоли юзер выбрам мандат
      * передадим его демону и запросим список уровней секретности для данного мандата
      */
    void setUserMandat(const QString & u_name,const QString & u_mandat);
    /**
      * @fn QStringList &getSecLevelList() const;
      * @brief Возвращает список уровней секретности
      */
    QStringList getSecLevelList() const {return secLevelList;}

    /**
     * @fn void authUserToPrinter(int printer_id);
     * @brief Авторизация текщего пользователя на доступ к выбранному принтеру
     * Пиоск принтера производится в модели
     */
    void authUserToPrinter(int printer_id);
    /**
      * @fn void checkMB( const QString &sql_query);
      * @brief Отправляет локальному серверу запрос на проверку существования
      * документа с заданными атрибутами (МБ,номер экз. и название документа
      * в БД учета
      */
    void checkMB( const QString &sql_query );
    /**
      * @fn void registerMB ( const QString &sql_query );
      * @brief Запрос на регистрацию документа в БД учета
      */
    void registerMB ( const QString &sql_query );

    void doMergeDocWithTemplates (QByteArray field_data,bool preview_mode);

signals:
    // Результат наложения шаблона на документ
    void MergeDocWithTemplates( bool flag,const QString &info);
    // список путей к сформированным  страницам предпросмотра
    void PicturesList (QStringList page_list);
    // Список путей уже сформированным документам готовым для печати
    void ReadyForPrintPages(QByteArray page_list);
    // Получено требование завершить работу
    void reciveGoodBayMsg(const QString &shutdown_info);
    // Требуется передать демону дополнительные параметы документа для его регистрации
    // в бд учета
    void needRegisterDocInBase();
    //Документ зарегистрирован в БД УЧЕТА
    void RegisterDocInBase(bool flag,const QString &info);
    // Результат авторизации доступа к устройству
    void authToDevice(bool flag,const QString &info);

    void infoMessage(const QString &info);
    void gk_notReady(const QString &ext_info); // локальный сервер не готов к работе
    // Требуется авторизовать пользователя есть имя, и связь с демоном
    void needAuthUser(const QString &login_mandat_list);
    void gk_fullReady(const QString &login,const QString &mandat);


    // Успешная загрузка плагинов
    void pluginsLoad();
    // Регистрация на локальном сервере
    void LocalSrvRegistr();
    void RemoteDemonRegistr();
    void ReciveUserName();
    void ReciveUserMandat();
    //Локальный сервер вернул список принтеров доступных данному пользователю
    void RecivePrintersList();
    void ReciveSecLevelList();
    // Документ успешно конвертирован в pdf
    void doc_converted();
    // Подсчитали количество страниц
    void getPagesCount (int p_count);
    /**
      * @fn void error(const QString &e_info);
      * @brief При возникновении ошибки, сигнал передает описание ошибки
      * @todo Убрать функции @sa @fn QString lastError() @sa @fn bool isError()
      */
    void error(const QString &e_info);
	
private slots:
    void do_checkPointChanged(MyCheckPoints r_cpoint);
    /**
      * @fn void parseMessage(const Message r_msg);
      * @brief Обработка полученных сообщений от локального сервера
      */
    void parseMessage(const Message &r_msg);
    /**
      * @fn void launchAndConnect();
      * @brief проверяет факт запуска GateKeeper (Запускает его если не запущен)
      * и устаналивает с ним долгие и доверительные отношения
      */
    void launchAndConnect();

    void do_printCurrentDoc();
    /**
      * @fn void do_UserDemands2Restart(const QString &mb,int cur_copy,int total_copy,
                                bool all_copy);
      * @brief Пользователь потребовал  запуск мастера с нуля,
      * логин мандат уже есть, документ конвертирован в pdf, число страниц
      * подсчитанно, необходимо только удалить каталоги 1-copy - 5-copy
      * и запустить весь цикл заново.
      */
    void do_UserDemands2Restart(const QString &mb,int cur_copy,int total_copy,
                                bool all_copy);
private:
    /** @brief Список переменных из ini файла
      * @var link_name;        Имя локального сервера для общения с Локальным миром
      * @var gsBin;            Путь к исполняемому файлу gs
      * @var pdftkBin;         Путь к исполняемому файлу pdfTk
      * @var spoolDir;         Путь к каталогу временных файлов
      * @var gatekeeper_bin;   Путь к исполняемому файлу GateKeeper
      * @var local_t_path;     Путь к локальным шаблонам
      * @var global_t_path;    Путь к глобальным шаблонам
      *
      * @brief Локальные переменные
      * @var clientName;       Имя клиента приложения для индетификации в GateKeeper
      * @var e_info;           Последняя возникшая ошибка
      * @var e_state;          Статус Локального сокета
      * @var m_LocalClient;    Указатель на локальный сокет
      * @var stopLaunch;       Триггер повторного запуска GateKeeper
      * @var m_appPath;        Путь запуска приложения для поиска ini файла
      * @var gs_plugin;        Указатель на плагин gs @todo Нужен ли ?
      * @var client_uuid;       Уникальный номер полученный от GateKeeper
      *
      * @var currentUserName;   Имя текущего пользоватеяля
      * @var currentUserMandat; Мандат  текущего пользоватеяля
      * @var tInfo;             Хранилище метаинформации о шаблонах
      * @var prn_list;          Список принтеров полученый от сервера
      * @var printersModel      Модель в которой храниться список принтеров
      */
    QString link_name;
    QString gsBin;
    QString pdftkBin;
    QString spoolDir;
    QString gatekeeper_bin;

    QString          clientName;
    QString          e_info;
    bool             e_state;
    mySocketClient   *m_LocalClient;
    bool             stopLaunch;
    QString          m_appPath;
    Igs_plugin       *gs_plugin;
    QString          client_uuid;
    QString          currentUserName;
    QString          currentUserMandat;
    int              currentSelectPrinterId; // Текущий выбранный пользователем принтер
    int              pCnt;                 // Число страниц в текущем документе
    TemplatesInfo    *tInfo;
    QStandardItemModel *printersModel;

    QStringList      secLevelList;
    //QMap <QString,QString> printer_device_list;

    /**
      *@fn void AfterConnectSteps();
      *@brief После установления соедиения с сервером, когда он вернул статус
      *полностью или частично готов к работе, выполняем безусловый набор действий
      *@li Получение метаинформации о шаблонах из списка
      */
    void afterConnectSteps();
    /**
      * @fn void sendMessage2LocalSrv(const Message &s_msg);
      * @brief отправка сообщения локальному серверу
      */
    void sendMessage2LocalSrv(const Message &s_msg);
    /**
      * @fn void readConfig();
      * @brief Читаем установки из ini файла
      */
    void readConfig(const QString &ini_file);

    /**
      * @fn setError(const QString &info);
      * @brief Устанавливает состояние ошибки,  записывает новое описание ошибки
      */
    void setError(const QString &info);
    /**
      * @fn setError();
      * @brief Устанавливает состояние ошибки = НЕТ ошибки
      */
    void setError();

    /**
      * @fn void savePrintersListToModel(const QString &prn_list);
      * @brief Заполняет модель данными из списка prn_list по канонам дзенбуддизма
      */
    void savePrintersListToModel(const QString &prn_list);
    /**
      * @fn findPrinterInModel(int printer_id);
      * @brief Поиск в модели выбранного пользователем принтера
      * и возврат его в виде IP.NAME для авторизации
      */

    QString findPrinterInModel(int printer_id);
};



#endif // ENGINE_H
