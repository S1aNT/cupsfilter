#ifndef ENGINE_H
#define ENGINE_H
#include "QtCore/QObject"

class QLocalSocket;
class mySocketClient;
class Message;

#include "mytypes.h"
#include "igs_plugin.h"

using namespace VPrn;

class Engine :public QObject
{
    Q_OBJECT
public:    

    Engine(QObject*parent = 0,const QString &app_path = QString());
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

signals:
    void infoMessage(const QString &info);

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
      */

    /** @brief Локальные переменные
      * @var m_state;          Статус Локального сокета
      * @var e_info;           Последняя возникшая ошибка
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
    //QString rcp_file;

    QString e_info;
    bool e_state;

    mySocketClient *m_LocalClient;
    bool stopLaunch;
    QString m_appPath;
    Igs_plugin *gs_plugin;
    QString client_uuid; // Иникальный номер полученный от GateKeeper


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
