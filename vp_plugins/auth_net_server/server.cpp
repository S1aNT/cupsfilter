#include "server.h"


#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>

#include <QtGui/QAction>
#include <QtGui/QComboBox>
#include <QtGui/QPushButton>
#include <QtGui/QMessageBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QCheckBox>

#include <QtCore/QObject>
#include <QtCore/QDir>
#include <QtCore/QPluginLoader>
#include <QtCore/QSettings>
#include <QtCore/QCoreApplication>
#include <QCloseEvent>



Server::Server(QWidget *parent)
    : QDialog(parent)
    , myServerGears(0)
    , myNet_plugin(0)
    , myAuth_plugin(0)
    , m_GateKeeperReady(false)
{

    /** @brief информационная форма
      * Имя юзера -- ПУПКИН
      * Мандат юзера СС
      * Статус демона (Есть связь/Нет связи/Нет места на диске)
      */

    resize(280, 240);
    setMinimumSize(QSize(280, 240));
    setMaximumSize(QSize(280, 240));
    QFont font;
    font.setFamily(QString::fromUtf8("Times New Roman"));
    font.setPointSize(12);
    font.setBold(true);
    font.setWeight(75);

    groupBox = new QGroupBox(this);
    groupBox->setGeometry(QRect(9, 5, 261, 81));
    groupBox->setTitle(QObject::trUtf8("Параметры авторизации"));
    gridLayout = new QGridLayout(groupBox);

    login_label = new QLabel(groupBox);
    login_label->setFont(font);
    login_label->setText(QObject::trUtf8("Логин"));

    mandat_label = new QLabel(groupBox);
    mandat_label->setFont(font);
    mandat_label->setText(QObject::trUtf8("Мандат"));

    login_LE = new QLineEdit(groupBox);
    login_LE->setReadOnly(true);
    login_LE->setEnabled(false);

    mandat_LE = new QLineEdit(groupBox);
    mandat_LE->setReadOnly(true);
    mandat_LE->setEnabled(false);

    gridLayout->addWidget(login_label, 0, 0, 1, 1);
    gridLayout->addWidget(mandat_label, 1, 0, 1, 1);
    gridLayout->addWidget(login_LE, 0, 1, 1, 1);
    gridLayout->addWidget(mandat_LE, 1, 1, 1, 1);

    groupBox_2 = new QGroupBox(this);
    groupBox_2->setGeometry(QRect(10, 90, 261, 101));
    groupBox_2->setTitle(QObject::trUtf8("Состояние сервера"));

    demonState_LE = new QLineEdit(groupBox_2);
    demonState_LE->setGeometry(QRect(10, 20, 241, 20));
    demonState_LE->setReadOnly(true);
    demonState_LE->setEnabled(false);

    authCheckBox = new QCheckBox(groupBox_2);
    authCheckBox->setEnabled(false);
    authCheckBox->setChecked(false);
    authCheckBox->setText(QObject::trUtf8("Плагин авторизации загружен"));
    authCheckBox->setGeometry(QRect(10, 50, 241, 17));

    netCheckBox = new QCheckBox(groupBox_2);
    netCheckBox->setEnabled(false);
    netCheckBox->setChecked(false);
    netCheckBox->setText(QObject::trUtf8("Плагин сетевого обмена загружен"));
    netCheckBox->setGeometry(QRect(10, 70, 241, 17));

    quitButton = new QPushButton(this);    
    quitButton->setGeometry(QRect(100, 200, 75, 23));
    quitButton->setText(QObject::trUtf8("Свернуть"));

    QObject::connect (quitButton,SIGNAL(clicked()),
                      this,SLOT(close()));

    setWindowTitle(QObject::trUtf8("GateKeeper"));

    createActions();
    createTrayIcon();    
    setVisible(false);
    this->init();
}

void Server::init()
{
    bool Ok = true;
    {
        Ok &= readConfig();
        if ( Ok ){
            if (!localSrvName.isEmpty()){
                // Создаем основной объект
                myServerGears = new serverGears(this,localSrvName);
                connect (myServerGears,SIGNAL(checkPointChanged(MyCheckPoints)),
                         this, SLOT(do_ChekPointChanged(MyCheckPoints)));
                Ok &= loadPlugins();
                if (Ok){
                    myServerGears->setNetPlugin(myNet_plugin);

                    // Инициализация плагинов
#if defined(Q_OS_UNIX)
                    myAuth_plugin->init(ticket_fname);
#elif defined(Q_OS_WIN)
                    myAuth_plugin->init();
#endif
                    myNet_plugin->init(serverHostName, serverPort,myServerGears->getUuid());

                }else{
                    setTrayStatus(VPrn::gk_ErrorState,QObject::trUtf8("Ошибка при загрузке плагинов"));
                }
            }
        }else{
            //demonState_LE->setText(QObject::trUtf8("Ошибка конфигурации"));
            setTrayStatus(VPrn::gk_ErrorState,QObject::trUtf8("Ошибка при чтении файла конфигурации"));
        }
    }
    m_GateKeeperReady = Ok;
}

void Server::showTrayMessage(trayIcons msg_type,
                             const QString & msg_title,
                             const QString & msg_body)
{
    QSystemTrayIcon::MessageIcon icon;
    switch (msg_type){
    case InfoType:
        icon = QSystemTrayIcon::Information;
        break;
    case WarnType:
        icon = QSystemTrayIcon::Warning;
        break;
    case CritType:
        icon = QSystemTrayIcon::Critical;
        break;
    }
    trayIcon->showMessage(msg_title, msg_body, icon, 7 * 1000);
}

void Server::setVisible(bool visible)
{
    minimizeAction->setEnabled(visible); 
    restoreAction->setEnabled(!visible);
    QDialog::setVisible(visible);
}

void Server::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {
        QMessageBox::information(this, QObject::trUtf8("GateKeeper"),
                                 QObject::trUtf8("Данная программа будет продолжать работу в системном трее.\n"
                                    "Для завершения работы, выберите Выход "
                                    "в контекстном меню программы. "));
        hide();
        event->ignore();
    }

}

void Server::createActions()
{
    minimizeAction = new QAction(QObject::trUtf8("Свернуть в трей"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    restoreAction = new QAction(QObject::trUtf8("Восстановить"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    quitAction = new QAction(QObject::trUtf8("Выход"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    runEditorAction  = new QAction(QObject::trUtf8("Запуск редактора"), this);
    connect(runEditorAction, SIGNAL(triggered()), this, SLOT(runTEditor()));
}

void Server::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(runEditorAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);    

    //connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(messageClicked()));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

}

void Server::appendStartMsg(const QString & msg)
{
    showTrayMessage(InfoType,QObject::trUtf8("GateKeeper"),msg);
}

//************************** PRIVATE SLOTS *************************************
void Server::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    bool flip;
    switch (reason) {
        //case QSystemTrayIcon::MiddleClick:
        //case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        flip = this->isVisible();
        setVisible(!flip);
        break;
    default:
        ;
    }
}

void Server::errorInfo(pluginsError eCode,QString e_msg)
{

}

void Server::setUserName(QString & login,QString &mandat)
{
    //u_login = login;
    //u_mandat = mandat;
    trayStatus my_TrayStatus;
    QString info_msg;
    if (login.isEmpty()){
        my_TrayStatus = VPrn::gk_ErrorState;
        info_msg = QObject::trUtf8("Ошибка авторизации пользователя.Не удалось получить логин!");
    }else{
        login_LE->setText(login);

        if (mandat.isEmpty()){
            my_TrayStatus = VPrn::gk_UserLogin;
            info_msg = QObject::trUtf8("Пользователь зарегистрирован в ОС");
        }else{
            my_TrayStatus = VPrn::gk_UserAuthorized;
            info_msg = QObject::trUtf8("Пользователь зарегистрирован в СУРД.");
            mandat_LE->setText(mandat);
        }
    }
    setTrayStatus(my_TrayStatus,info_msg);
    authCheckBox->setChecked(true);
    if (myServerGears){
        myServerGears->setAuthData(login,mandat);
    }
}

void Server::setTrayStatus (trayStatus t_stat, const QString &t_msg)
{
    trayIcons m_type = VPrn::InfoType;
    currentStatus = t_msg;
    QIcon currentIcon;

    switch (t_stat){
    case gk_Started:
        currentIcon = QIcon(":/shield.png");        
        break;
    case gk_UserAuthorized:
        currentIcon = QIcon(":/key.png");
        break;
    case gk_UserLogin:
        currentIcon = QIcon(":/user.png");
        break;
    case gk_DoPrintJob:
        currentIcon = QIcon(":/print.png");
        break;
    case gk_DoReportJob:
        currentIcon = QIcon(":/monitor.png");
        break;
    case gk_WarningState:
        currentIcon = QIcon(":/warning.png");
        m_type = VPrn::WarnType;
        break;
    case gk_ErrorState:
        currentIcon = QIcon(":/error.png");
        m_type = VPrn::CritType;
        m_GateKeeperReady = false;
        break;
    }
    trayIcon->setIcon(currentIcon);
    trayIcon->setToolTip(currentStatus);
    setWindowIcon(currentIcon);

    if (!trayIcon->isVisible()){
        trayIcon->show();
    }
    showTrayMessage(m_type,QObject::trUtf8("GateKeeper"),t_msg);
    demonState_LE->setText(t_msg);
}

void Server::do_ChekPointChanged(MyCheckPoints m_scheckPoint)
{
    switch (m_scheckPoint){
    case VPrn::net_Connected:
        netCheckBox->setChecked(true); //Включаем флажок сеть установленна и доступна
        break;
    case VPrn::loc_ServerStart:
        setTrayStatus(VPrn::gk_Started,QObject::trUtf8("Готов работать!"));
        break;
    case VPrn::loc_NewClientStarted:
        showTrayMessage(InfoType,QObject::trUtf8("GateKeeper"),QObject::trUtf8("Подключен новый клиент!"));
        break;

//    case VPrn::loc_MessageRecive:
//        showTrayMessage(InfoType,QObject::trUtf8("GateKeeper"),QObject::trUtf8("Полученно сообщение от локального клиента!"));
//        break;
    case VPrn::loc_Disconnected:
        showTrayMessage(InfoType,QObject::trUtf8("GateKeeper"),QObject::trUtf8("Локальный клиент отключен!"));
        break;
    case VPrn::loc_CantStartListen:
    case VPrn::glob_Error:
        setTrayStatus(VPrn::gk_ErrorState,myServerGears->lastError());
        break;
    }
}

void Server::runTEditor()
{
    if (tEditor_bin.isEmpty()){
        setTrayStatus(VPrn::gk_ErrorState,
                      QObject::trUtf8("Ошибка в файле параметров.\nПуть к редактору шаблонов не задан"));
    }else{
    }
}



//------------------------------------------------------------------------------
bool Server::loadPlugins()
{
    QDir pluginsDir(qApp->applicationDirPath());

    Inet_plugin *net_plugin_Interface;
    Auth_plugin *auth_plugin_Interface;

    if (pluginsDir.dirName().toLower() == "debug" ||
        pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();

#if defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#endif
    pluginsDir.cd("plugins");
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)){
        QPluginLoader pluginMessageer(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginMessageer.instance();
        if (plugin) {
            bool needUnloadPlugin = true;
            {
                if (!myAuth_plugin){
                    /// Загрузка плагина авторизации
                    auth_plugin_Interface = qobject_cast<Auth_plugin *>(plugin);
                    if (auth_plugin_Interface){
                        needUnloadPlugin = false;
                        myAuth_plugin = auth_plugin_Interface;

                        connect(plugin, SIGNAL(error(pluginsError,QString )),
                                this,   SLOT (errorInfo(pluginsError,QString ))
                                );

                        connect(plugin, SIGNAL(get_User_name_mandat(QString &,QString &)),
                                this,   SLOT(setUserName(QString&,QString&))
                                );
                    }
                }
                if (!myNet_plugin){
                    /// Загрузка сетевого плагина
                    net_plugin_Interface = qobject_cast<Inet_plugin *> (plugin);
                    if (net_plugin_Interface) {
                        needUnloadPlugin = false;
                        // Сохраним указатель на плагин как данные класса
                        myNet_plugin = net_plugin_Interface;
                        connect(plugin, SIGNAL(error(pluginsError,QString )),
                                this,   SLOT (errorInfo(pluginsError,QString ))
                                );
                        connect(plugin, SIGNAL(messageReady(Message)),
                                myServerGears,   SLOT(reciveNetworkMessage(Message))
                                );
                    }
                }
            }
            if ( needUnloadPlugin ){
                // Выгрузим его нафиг не наш он плагин, сто пудово :)
                qDebug() << QObject::trUtf8("Plugin's [%1] unload").arg(pluginsDir.absoluteFilePath(fileName));
                pluginMessageer.unload();
            }
        }
    }
    return (myNet_plugin && myAuth_plugin);

}

bool Server::readConfig()
{    
    bool Ok = true;
    {
        // Читаем файл настроек
        QString ini_path =QString("%1/Technoserv/safe_printer.ini")
                          .arg(qApp->applicationDirPath());
        if (QFile::exists(ini_path)){
            QSettings settings (ini_path,QSettings::IniFormat);
            settings.setIniCodec("UTF-8");
            settings.beginGroup("SERVICE");
            serverHostName = settings.value("server").toString();
            serverPort     = settings.value("port").toInt();
            localSrvName   = settings.value("link_name").toString();
            settings.endGroup();
#if defined(Q_OS_UNIX)
            settings.beginGroup("USED_DIR_FILE");
            spoolDir     = settings.value("spool_dir").toString();
            ticket_fname = settings.value("session_ticket").toString();
            tEditor_bin  = settings.value("tEditor_bin").toString();
            settings.endGroup();
#elif defined(Q_OS_WIN)
            settings.beginGroup("USED_DIR_FILE");
            spoolDir= settings.value("spool_dir").toString();
            tEditor_bin  = settings.value("tEditor_bin").toString();
            settings.endGroup();
#endif
            // Тестируем переменные
            if ( serverHostName.isEmpty() ||
                 localSrvName.isEmpty()   ||
                 spoolDir.isEmpty()       ||
                 serverPort < 1024        ||
                 serverPort > 9999  ){
                Ok = false;
                m_lastError =  QObject::trUtf8("Отсутствует или имеет не верное значение один из параметров,"
                                  "в файле конфигурации");

            }
        }else{
            Ok  = false;
            m_lastError = QObject::trUtf8("Файл [%1] с настройками программы не найден!")
                          .arg(ini_path);
        }
    }
    return Ok;
}

void Server::showCriticalInfo(const QString & info)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle(QObject::trUtf8("GateKeeper"));
    msgBox.setText(info);
    QPushButton *exitButton  = msgBox.addButton(QObject::trUtf8("Выход"), QMessageBox::ActionRole);
    connect (exitButton,SIGNAL(clicked()), qApp, SLOT(quit()));
    msgBox.exec();

}
