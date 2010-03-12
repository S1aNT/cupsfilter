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

#include <QtCore/QObject>
#include <QtCore/QDir>
#include <QtCore/QPluginLoader>



Server::Server(QWidget *parent)
    : QDialog(parent)
    , myServerGears(0)
    , myNet_plugin(0)
    , myAuth_plugin(0)
    , u_login(QString())
    , u_mandat(QString())
{

    /** @brief информационная форма
      * Имя юзера -- ПУПКИН
      * Мандат юзера СС
      * Статус демона (Есть связь/Нет связи/Нет места на диске)
      */

    resize(280, 210);
    setMinimumSize(QSize(280, 210));
    setMaximumSize(QSize(280, 210));
    QFont font;
    font.setFamily(QString::fromUtf8("Times New Roman"));
    font.setPointSize(12);
    font.setBold(true);
    font.setWeight(75);

    groupBox = new QGroupBox(this);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    groupBox->setGeometry(QRect(9, 5, 261, 81));
    groupBox->setTitle(tr("Параметры авторизации"));
    gridLayout = new QGridLayout(groupBox);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

    login_label = new QLabel(groupBox);
    login_label->setObjectName(QString::fromUtf8("label"));
    login_label->setFont(font);
    login_label->setText(tr("Логин"));
    gridLayout->addWidget(login_label, 0, 0, 1, 1);

    mandat_label = new QLabel(groupBox);
    mandat_label->setObjectName(QString::fromUtf8("label_2"));
    mandat_label->setFont(font);
    mandat_label->setText(tr("Мандат"));

    gridLayout->addWidget(mandat_label, 1, 0, 1, 1);

    lineEdit_2 = new QLineEdit(groupBox);
    lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
    lineEdit_2->setReadOnly(true);

    gridLayout->addWidget(lineEdit_2, 0, 1, 1, 1);

    lineEdit = new QLineEdit(groupBox);
    lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
    lineEdit->setReadOnly(true);

    gridLayout->addWidget(lineEdit, 1, 1, 1, 1);

    groupBox_2 = new QGroupBox(this);
    groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
    groupBox_2->setGeometry(QRect(10, 90, 261, 79));
    groupBox_2->setTitle(tr("Состояние сервера печати"));

    demonStatePlainTextEdit = new QPlainTextEdit(groupBox_2);
    demonStatePlainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
    demonStatePlainTextEdit->setGeometry(QRect(10, 16, 241, 51));
    demonStatePlainTextEdit->setReadOnly(true);

    quitButton = new QPushButton(this);
    quitButton->setObjectName(QString::fromUtf8("pushButton"));
    quitButton->setGeometry(QRect(98, 178, 75, 23));
    quitButton->setText(tr("Свернуть"));

    QObject::connect (quitButton,SIGNAL(clicked()),
                      this,SLOT(close()));

    setWindowTitle(tr("GateKeeper"));

    createActions();
    createTrayIcon();    
    setTrayStatus(VPrn::GateKeeperStarted,tr("Успешно загружен"));

    // Создаем основной объект
    myServerGears = new serverGears();
    loadPlugins();
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
        QMessageBox::information(this, tr("Systray"),
                                 tr("The program will keep running in the "
                                    "system tray. To terminate the program, "
                                    "choose <b>Quit</b> in the context menu "
                                    "of the system tray entry."));
        hide();
        event->ignore();
    }
}

void Server::createActions()
{
    minimizeAction = new QAction(tr("Свернуть в трей"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    restoreAction = new QAction(tr("Восстановить"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    quitAction = new QAction(tr("Выход"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void Server::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);    
}

//************************** PRIVATE SLOTS *************************************
void Server::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    //    switch (reason) {
    //    case QSystemTrayIcon::Trigger:
    //    case QSystemTrayIcon::DoubleClick:
    //        iconComboBox->setCurrentIndex((iconComboBox->currentIndex() + 1)
    //                                      % iconComboBox->count());
    //        break;
    //    case QSystemTrayIcon::MiddleClick:
    //        showMessage();
    //        break;
    //    default:
    //        ;
    //    }
}

void Server::messageClicked()
{
    QMessageBox::information(0, tr("Systray"),
                             tr("Sorry, I already gave what help I could.\n"
                                "Maybe you should try asking a human?"));
}

void Server::errorInfo(pluginsError eCode,QString e_msg)
{

}

void Server::setUserName(QString & login,QString &mandat)
{
    u_login = login;
    u_mandat = mandat;
    trayStatus my_TrayStatus;
    QString t_msg;
    if (u_login.isEmpty()){
        my_TrayStatus = VPrn::ErrorState;
        t_msg = tr("Ошибка авторизации пользователя.Не удалось получить логин!");
    }else{
        if (u_mandat.isEmpty()){
            my_TrayStatus = VPrn::UserLogin;
            t_msg = tr("Пользователь зарегистрирован в ОС");
        }else{
            my_TrayStatus = VPrn::UserAuthorized;
            t_msg = tr("Пользователь зарегистрирован в СУРД.");
        }
    }
    setTrayStatus(my_TrayStatus,t_msg);
}

void Server::setTrayStatus (trayStatus t_stat, const QString &t_msg)
{
    trayIcons m_type = VPrn::InfoType;
    currentStatus = t_msg;
    QIcon currentIcon;

    switch (t_stat){
    case GateKeeperStarted:
        currentIcon = QIcon(":/shield.png");        
        break;
    case UserAuthorized:
        currentIcon = QIcon(":/key.png");
        break;
    case UserLogin:
        currentIcon = QIcon(":/user.png");
        break;
    case DoPrintJob:
        currentIcon = QIcon(":/print.png");
        break;
    case DoReportJob:
        currentIcon = QIcon(":/monitor.png");
        break;
    case WarningState:
        currentIcon = QIcon(":/warning.png");
        m_type = VPrn::WarnType;
        break;
    case ErrorState:
        currentIcon = QIcon(":/stop.png");
        m_type = VPrn::CritType;
        break;
    }
    trayIcon->setIcon(currentIcon);
    trayIcon->setToolTip(currentStatus);
    setWindowIcon(currentIcon);

    if (!trayIcon->isVisible()){
        trayIcon->show();
    }
    showTrayMessage(m_type,tr("GateKeeper"),t_msg);
}

//------------------------------------------------------------------------------
void Server::loadPlugins()
{
    QDir pluginsDir(qApp->applicationDirPath());

    Inet_plugin *net_plugin_Interface;
    Auth_plugin *auth_plugin_Interface;

#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" ||
        pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
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
            connect(plugin, SIGNAL(error(pluginsError,QString )),
                    this,   SLOT (errorInfo(pluginsError,QString ))
                    );
            /// Загрузка плагина авторизации
            auth_plugin_Interface = qobject_cast<Auth_plugin *>(plugin);
            if (auth_plugin_Interface){
                myAuth_plugin = auth_plugin_Interface;

                connect(plugin, SIGNAL(get_User_name_mandat(QString &,QString &)),
                        this,   SLOT(setUserName(QString&,QString&))
                        );
#if defined(Q_OS_UNIX)
                myAuth_plugin->init(ticket_name);
#elif defined(Q_OS_WIN)
                myAuth_plugin->init();
#endif                
            }            
            /// Загрузка сетевого плагина
            net_plugin_Interface = qobject_cast<Inet_plugin *> (plugin);
            if (net_plugin_Interface) {
                // Сохраним указатель на плагин как данные класса
                myNet_plugin=net_plugin_Interface;
                //                connect(plugin, SIGNAL(serverResponse(QString &)),
                //                        this,   SLOT(parseServerResponse(QString &))
                //                        );
                //                myNet_plugin->init(serverHostName, serverPort,sid);
                //                QString  msg= QObject::trUtf8("Плагин: [Работы с сетью] успешно загружен.");

            }

        }
    }
}
