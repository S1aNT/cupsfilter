#ifndef ENGINE_H
#define ENGINE_H
#include "QtCore/QObject"

class QLocalSocket;
class mySocketClient;
class Message;
class QStringList;

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
      * @fn bool isError();
      * @brief Если ошибка вернет истину
      */
    bool isError(){return e_state;};
    /**
      * @fn QString lastError()
      * @brief Возвращает текстовое описание последней ошибки
      */
    QString lastError() const {return e_info;};

    //VPrn::CoreOptions options() const;
    //void setOptions(VPrn::CoreOptions s_opt);
    void sendMessage2LocalSrv(const Message &s_msg);
    void init();
    /**
      * @fn  void setUserMandat(const QString & mandat);
      * @brief При авторизации с консоли юзер выбрам мандат
      * передадим его демону и запросим список уровней секретности для данного мандата
      */
    void setUserMandat(const QString & mandat);
    /**
      * @fn QStringList &getSecLevelList() const;
      * @brief Возвращает список уровней секретности
      */
    QStringList getSecLevelList() const {return secLevelList;};
signals:
    void infoMessage(const QString &info);
    void gk_notReady(const QString &ext_info); // локальный сервер не готов к работе
    // Требуется авторизовать пользователя есть имя, и связь с демоном
    void needAuthUser(const QString &login_mandat_list);
    void gk_fullReady(const QString &login,const QString &mandat);
    //Локальный сервер вернул список принтеров доступных данному пользователю

    void getPrinterList(const QString &prn_list);
    // Успешная загрузка плагинов
    void pluginsLoad();
    // Регистрация на локальном сервере
    void LocalSrvRegistr();
    void RemoteDemonRegistr();
    void ReciveUserName();
    void ReciveUserMandat();
    void RecivePrintersList();
    void ReciveSecLevelList();
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

private:
    /** @brief Список переменных из ini файла
      * @var serverHostName;   Имя удаленного сервера (Демон для mainfarme)
      * @var serverPort;       Порт для связи
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
      * @var client_uuid;      Уникальный номер полученный от GateKeeper
      * @var secLevelList;     Список уровней секретности для данного мандата
      */
    QString serverHostName;
    int serverPort;
    QString link_name;
    QString gsBin;
    QString pdftkBin;
    QString spoolDir;
    QString gatekeeper_bin;
    QString local_t_path;
    QString global_t_path;

    QString clientName; // Имя клиента
    QString e_info;
    bool e_state;
    mySocketClient *m_LocalClient;
    bool stopLaunch;
    QString m_appPath;
    Igs_plugin *gs_plugin;
    QString client_uuid; // Уникальный номер полученный от GateKeeper
    QStringList secLevelList;

    /**
      * @fn void readConfig();
      * @brief Читаем установки из ini файла
      */
    void readConfig(const QString &ini_file);
    /**
      * @fn bool loadPlugin();
      * @brief Загружаем и настраиваем плагины
      */
    bool loadPlugin();

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
};



#endif // ENGINE_H
