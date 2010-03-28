#ifndef SERVER_H
#define SERVER_H

#include "servergears.h"
#include "auth_plugin.h"
#include "inet_plugin.h"
#include "igs_plugin.h"

#include "mytypes.h"

#include <QDialog>
#include <QtGui/QSystemTrayIcon>
#include <QtGui/QErrorMessage>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QGroupBox;
class QFormLayout;
class QLineEdit;
class QLocalServer;
class QComboBox;
class QMenu;
class QPlainTextEdit;
class QDialogButtonBox;
class QGridLayout;
class QCheckBox;
class QErrorMessage;
QT_END_NAMESPACE

using namespace VPrn;

class Server : public QDialog
{
    Q_OBJECT

public:
    Server(QWidget *parent = 0);
    void setVisible(bool visible);
    /**
      * @fn bool isReady();
      * @brief Возращает статус GateKeeper готов ли он к работе или нет
      */
    bool isReady(){return m_GateKeeperReady;};

    /**
      * @fn QString lastError();
      * @brief Возвращает последнюю возникшую ошибку
      */
    QString lastError(){return m_lastError;};
    /**
      * @fn void showCriticalInfo(constQStrin & info)
      * @brief  Отображает критическую ошибку и завершает работу программы
      */
    void showCriticalInfo(const QString & info);

protected:
    void closeEvent(QCloseEvent *event);
public slots:
    /**
       @fn void appendStartMsg(const QString & msg);
       @brief Служит для получения сообщения при старте другой копии программы 
     */
    void appendStartMsg(const QString & msg);
    /**
      * @fn void showBaloonMessage(int m_Type,const QString &b_msg)
      * @brief показывает сообщение в трее
      * @param m_Type тип сообщения critical,warning,information
      * @param b_msg Само сообщение
      */
    //void showBaloonMessage(int m_Type,const QString &b_msg);
    /**
      * @fn void showTrayMessage(trayIcons msg_type,
      *                          const QString & msg_title,
      *                          const QString & msg_body);
      * @brief Отображает в трее сообщение заданного типа
      */
    void showTrayMessage(trayIcons msg_type,
                         const QString & msg_title,
                         const QString & msg_body);
private slots:

    void iconActivated(QSystemTrayIcon::ActivationReason reason);

    void errorInfo(pluginsError eCode,QString e_msg);
    /**
      * @fn void setUserName(const QString & login,const QString &mandat);
      * @brief Заполнение параметров аторизации пользователя
      */
    void setUserName(QString & login,QString &mandat);
    /**
      * @fn void runTEditor()
      * @brief Запуск редактора шаблонов как отдельного приложения
      */
    void runTEditor();
    /**
      * @fn void do_ChekPointChanged(MyCheckPoints m_scheckPoint);
      * @brief Как только состояние сокета изменилось, его надо обработать
      */
    void do_ChekPointChanged(MyCheckPoints m_scheckPoint);

private:

    void createActions();
    void createTrayIcon();

    /**
      * @fn void init();
      * @brief Производит первоначальную загрузку приложения,
      */
    void init();

    /**
      * @fn bool loadPlugins()
      * @brief загрузка плагинов
      */
    bool loadPlugins();

    /**
      * @fn void setTrayStatus(trayStatus t_stat,const QString & t_msg);
      * @brief Устаналивает иконку и сообщение в зависимости от статуса
      */
    void setTrayStatus (trayStatus t_stat, const QString &t_msg);
    /**
      * @fn bool readConfig();
      * @brief читаем ini файл
      */
    bool readConfig();
    /**
      * @var mainGear; Основной модуль программы
      * @var myNet_plugin; Указатель на сетевой плагин
      * @var myAuth_plugin; Указатель на плагин авторизаци @todo (Нужен ли ?)
      * @var currentStatus Текущее статусное сообщение
      * @var m_GateKeeperReady; Шлюз готов к работе или нет
      * @var m_lastError;   последнее сообщение об ошибке
      */
    serverGears *myServerGears;
    Inet_plugin *myNet_plugin;
    Auth_plugin *myAuth_plugin;
    Igs_plugin  *myGs_plugin;

    bool    m_GateKeeperReady;
    QString m_lastError;
    QString currentStatus;
    QString current_sid;

    /**
      * @brief --------- Блок переменных из ini файла ------------------------
      * @var serverHostName; Сетевое имя демона или IP адрес
      * @var serverPort;     Порт для связи c демоном
      * @var localSrvName;   Имя локального сервера
      * @var ticket_fname;   Имя файла в котором храняться параметры авторизации UNIX only
      * @var gsBin;            Путь к исполняемому файлу gs
      * @var pdftkBin;         Путь к исполняемому файлу pdfTk
      * @var spoolDir;         Путь к каталогу временных файлов
      * @var local_t_path;     Путь к локальным шаблонам
      * @var global_t_path;    Путь к глобальным шаблонам
      * @var tEditor_bin;      Путь к  файлу редактора шаблонов
      *------------------------------------------------------------------------
      */
    QString serverHostName;
    int     serverPort;
    QString localSrvName;
    QString ticket_fname;
    QString gsBin;
    QString pdftkBin;
    QString spoolDir;
    QString local_t_path;
    QString global_t_path;
    QString tEditor_bin; 

    // Набор графических элементов
    QGroupBox   *groupBox;
    QFormLayout *formLayout;
    QLabel      *login_label;
    QLineEdit   *login_LE;
    QLabel      *mandat_label;
    QLineEdit   *mandat_LE;
    QGroupBox   *groupBox_2;
    QLineEdit   *demonState_LE;
    QPushButton *quitButton;
    QGridLayout *gridLayout;
    QCheckBox   *authCheckBox;
    QCheckBox   *netCheckBox;

    // Действия
    QAction *minimizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    QAction *runEditorAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    // Основное окошко ошибок
    QErrorMessage * myEMsgBox;
};

#endif
