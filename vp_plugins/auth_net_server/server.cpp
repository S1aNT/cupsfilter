#include <QtCore/QDebug>
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
#include <QtGui/QCloseEvent>
#include <QtGui/QErrorMessage>

#include <QtCore/QTimer>
#include <QtCore/QObject>
#include <QtCore/QDir>
#include <QtCore/QPluginLoader>
#include <QtCore/QSettings>
#include <QtCore/QCoreApplication>


Server::Server(QWidget *parent)
        : QDialog(parent)
        , myServerGears(0)
        , myNet_plugin(0)
        , myAuth_plugin(0)
        , myGs_plugin(0)
        , myTmpl_plugin(0)
        , m_GateKeeperReady(false)
{

    /// Регистрируем типы @todo Надо сделать отдельную функцию,
    /// в которой регистрировать все типы и вызвать ее из main

    qRegisterMetaType<VPrn::MessageType>("MessageType");
    qRegisterMetaType<VPrn::AppErrorType>("AppErrorType");

    myEMsgBox = new QErrorMessage(this);

    resize(300, 240);
    setMinimumSize(QSize(320 , 260));
    setMaximumSize(QSize(320, 280));
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
    groupBox_2->setGeometry(QRect(10, 90, 310, 140));
    groupBox_2->setTitle(QObject::trUtf8("Состояние сервера, загрузки плагинов."));
    groupBox_2->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    demonState_LE = new QLineEdit(groupBox_2);
    demonState_LE->setGeometry(QRect(10, 20, 280, 20));
    demonState_LE->setReadOnly(true);
    demonState_LE->setEnabled(false);

    authCheckBox = new QCheckBox(groupBox_2);
    authCheckBox->setEnabled(false);
    authCheckBox->setChecked(false);
    authCheckBox->setText(QObject::trUtf8("плагин авторизации."));
    authCheckBox->setGeometry(QRect(10, 50, 280, 17));

    netCheckBox = new QCheckBox(groupBox_2);
    netCheckBox->setEnabled(false);
    netCheckBox->setChecked(false);
    netCheckBox->setText(QObject::trUtf8("плагин работы с сетью."));
    netCheckBox->setGeometry(QRect(10, 70, 280, 17));

    gsCheckBox = new QCheckBox(groupBox_2);
    gsCheckBox->setEnabled(false);
    gsCheckBox->setChecked(false);
    gsCheckBox->setText(QObject::trUtf8("плагин работы с GhostScript."));
    gsCheckBox->setGeometry(QRect(10, 90, 280, 17));

    tmplCheckBox = new QCheckBox(groupBox_2);
    tmplCheckBox->setEnabled(false);
    tmplCheckBox->setChecked(false);
    tmplCheckBox->setText(QObject::trUtf8("плагин работы с шаблонами."));
    tmplCheckBox->setGeometry(QRect(10, 110, 280, 17));

    quitButton = new QPushButton(this);
    quitButton->setGeometry(QRect(100, 225, 130, 23));
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
    QString eMsg;
    bool Ok = true;
    {
        Ok &= readConfig();
        if ( Ok ){

            // Создаем основной объект
            myServerGears = new serverGears(this,localSrvName);
            connect (myServerGears,SIGNAL(checkPointChanged(MyCheckPoints)),
                     this,         SLOT(do_ChekPointChanged(MyCheckPoints))
                     );
            connect ( myServerGears,SIGNAL( error(VPrn::AppErrorType, QString) ),
                      this,         SLOT  ( errorInfo(VPrn::AppErrorType, QString) )
                      );
            connect ( myServerGears,SIGNAL( clearClientSpool (const QString &) ),
                      this,         SLOT  ( clearClientSpool (const QString &) )
                      );

            myServerGears->findTemplatesInPath(local_t_path);
            myServerGears->findTemplatesInPath(global_t_path);

#ifdef DEBUG_MODE
                myServerGears->setPrinterList( printer_list );
#endif
            if ( loadPlugins() ){// Проверка удачной загузки плагинов
                /// @todo Перевести эти чекбоксы на событие инициализация плагина

                authCheckBox->setChecked(true);
                gsCheckBox->setChecked(true);
                tmplCheckBox->setChecked(true);
                netCheckBox->setChecked(true);

                //-------------------------------- Инициализация плагинов
#if defined(Q_OS_UNIX)
                //myAuth_plugin->init(ticket_fname);
                myAuth_plugin->init();
#elif defined(Q_OS_WIN)
                myAuth_plugin->init();
#endif
                myGs_plugin->init(gsBin, pdftkBin,spoolDir);
                myNet_plugin->init(serverHostName, serverPort);
                myTmpl_plugin->init(spoolDir);
                //--------------------------------- Передадим указатели на плагины
                myServerGears->setGsPlugin(myGs_plugin);
                myServerGears->setTmplPlugin(myTmpl_plugin);
                myServerGears->setNetPlugin(myNet_plugin);

            }else{
                eMsg = QObject::trUtf8("Ошибка при загрузке плагинов");
            }
        }else{
            eMsg = QObject::trUtf8("Ошибка при чтении файла конфигурации");
        }
    }
    if (Ok){
        setTrayStatus(VPrn::gk_Started,QObject::trUtf8("Готов работать!"));
    }else{
        setTrayStatus(VPrn::gk_ErrorState,eMsg);
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
    default:
        icon = QSystemTrayIcon::Information;
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
        //        QMessageBox::information(this, QObject::trUtf8("GateKeeper"),
        //                                 QObject::trUtf8("Данная программа будет продолжать работу в системном трее.\n"
        //                                                 "Для завершения работы, выберите Выход "
        //                                                 "в контекстном меню программы. "));
        if (m_GateKeeperReady){
            hide();
            event->ignore();
        }else{
            event->accept();
        }
    }

}

void Server::createActions()
{
    minimizeAction = new QAction(QObject::trUtf8("Свернуть в трей"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    restoreAction = new QAction(QObject::trUtf8("Восстановить"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    quitAction = new QAction(QObject::trUtf8("Выход"), this);
    connect(quitAction, SIGNAL(triggered()),this, SLOT(dead_hands()));

    // runEditorAction  = new QAction(QObject::trUtf8("Запуск редактора"), this);
    // connect(runEditorAction, SIGNAL(triggered()), this, SLOT(runTEditor()));
}

void Server::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    //trayIconMenu->addAction(runEditorAction);
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

void Server::errorInfo(VPrn::AppErrorType eCode,QString e_msg)
{
    QString extMsg = QString("eCode %1. AppsError:%2\n%3")
                     .arg(eCode,0,10)
                     .arg(e_msg)
                     .arg(QString(Q_FUNC_INFO));
    myEMsgBox->showMessage(extMsg);
    qDebug() << extMsg;
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
    case VPrn::loc_NeedShutdown:
        showTrayMessage(InfoType,QObject::trUtf8("GateKeeper"),
                        QObject::trUtf8("Последний клиент отключен, завершаю работу."));
        QTimer::singleShot(3000,quitAction,SLOT(trigger()));

        break;
    case VPrn::net_Connected:
        netCheckBox->setChecked(true); //Включаем флажок сеть установленна и доступна
        break;
    case VPrn::loc_ServerStart:
        setTrayStatus(VPrn::gk_Started,QObject::trUtf8("Готов работать!"));
        break;
    case VPrn::loc_NewClientStarted:
        showTrayMessage(InfoType,QObject::trUtf8("GateKeeper"),
                        QObject::trUtf8("Подключен новый клиент!"));
        break;
    case VPrn::loc_Disconnected:
        showTrayMessage(InfoType,QObject::trUtf8("GateKeeper"),
                        QObject::trUtf8("Локальный клиент отключен!"));
        break;
    case VPrn::loc_CantStartListen:
    case VPrn::glob_Error:
        setTrayStatus(VPrn::gk_ErrorState,myServerGears->lastError());
        break;
    default:
        /** do nothing **/
        break;
    }
}

void Server::runTEditor()
{

}

void Server::dead_hands()
{
    // Всем клиентам говорим GoodBye, так как клиенты, у меня беспомощные без сервера,
    // то при закрытии его отправим клиента сообщение выгружаться !!!
    if (myServerGears){
        myServerGears->sayGoodBayAllClients();
    }
    // Очищаем файлы созданные самим gs, потом будем выгружать библиотеку
    if (myGs_plugin){
        myGs_plugin->final_clear();
    }

    QTimer::singleShot(500,qApp,SLOT(quit()));
}

void Server::clearClientSpool( const QString &client_uuid )
{
    // Великая чистка 37 года,чистим ВСЕ файлы созданные в интересах клиента
    QDir dir(spoolDir);
    QStringList filters;
    filters << QObject::trUtf8("*%1*.*").arg( client_uuid );
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setNameFilters(filters);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        QFile::remove(fileInfo.absoluteFilePath());
    }

}
//------------------------------------------------------------------------------
bool Server::loadPlugins()
{
    QDir pluginsDir(qApp->applicationDirPath());

    Inet_plugin      *net_plugin_Interface;
    Auth_plugin      *auth_plugin_Interface;
    Igs_plugin       *gs_plugin_Interface;
    Itmpl_sql_plugin *tmpl_plugin_Interface;

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
        QPluginLoader pluginManager(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginManager.instance();
        if (plugin) {
            bool needUnlugin = true;
            {
                if (!myTmpl_plugin){
                    tmpl_plugin_Interface = qobject_cast<Itmpl_sql_plugin *> (plugin);
                    if ( tmpl_plugin_Interface ){
                        needUnlugin = false;
                        myTmpl_plugin = tmpl_plugin_Interface;

                        connect ( plugin, SIGNAL ( error(VPrn::AppErrorType, QString) ),
                                  this,   SLOT   ( errorInfo(VPrn::AppErrorType, QString) ) );
                    }
                }
                if (!myGs_plugin){
                    /// Загрузка плагина работы с ghostscript
                    gs_plugin_Interface = qobject_cast<Igs_plugin *> (plugin);
                    if (gs_plugin_Interface){
                        needUnlugin = false;
                        myGs_plugin = gs_plugin_Interface;

                        connect ( plugin, SIGNAL ( error(VPrn::AppErrorType, QString) ),
                                  this,   SLOT   ( errorInfo(VPrn::AppErrorType, QString) ) );

                        connect (plugin,
                                 SIGNAL( docReady4work(const QString &,int) ),
                                 myServerGears,
                                 SLOT ( do_docReady4work (const QString &,int) )
                                 );
                        connect (plugin,
                                 SIGNAL( docConvertedToPdf(const QString &) ),
                                 myServerGears,
                                 SLOT ( do_docConvertedToPdf(const QString &)  )
                                 );
                        connect (plugin,
                                 SIGNAL( docReady4print(const QString &) ),
                                 myServerGears,
                                 SLOT ( do_docReady4print (const QString &) )
                                 );
                        connect (plugin,
                                 SIGNAL( docReady4preview(const QString &) ),
                                 myServerGears,
                                 SLOT ( do_docReady4preview (const QString &) )
                                 );
                    }
                }
                if (!myAuth_plugin){
                    /// Загрузка плагина авторизации
                    auth_plugin_Interface = qobject_cast<Auth_plugin *>(plugin);
                    if (auth_plugin_Interface){
                        needUnlugin = false;
                        myAuth_plugin = auth_plugin_Interface;

                        connect ( plugin, SIGNAL ( error(VPrn::AppErrorType, QString) ),
                                  this,   SLOT   ( errorInfo(VPrn::AppErrorType, QString) ) );

                        connect(plugin, SIGNAL(get_User_name_mandat(QString &,QString &)),
                                this,   SLOT(setUserName(QString&,QString&))
                                );
                    }
                }
                if (!myNet_plugin){
                    /// Загрузка сетевого плагина
                    net_plugin_Interface = qobject_cast<Inet_plugin *> (plugin);
                    if (net_plugin_Interface) {
                        needUnlugin = false;
                        // Сохраним указатель на плагин как данные класса
                        myNet_plugin = net_plugin_Interface;
                        connect ( plugin, SIGNAL ( error(VPrn::AppErrorType, QString) ),
                                  this,   SLOT   ( errorInfo(VPrn::AppErrorType, QString) ) );
                        connect (plugin,  SIGNAL   (messageReady(Message)),
                                 myServerGears,   SLOT(reciveNetworkMessage(Message))
                                 );
                    }
                }
            }
            if ( needUnlugin ){
                // Выгрузим его нафиг не наш он плагин, сто пудово :)
                qDebug() << QObject::trUtf8("Plugin's [%1] unload").arg(pluginsDir.absoluteFilePath(fileName));
                pluginManager.unload();
            }
        }
    }

    return (myNet_plugin && myAuth_plugin && myGs_plugin && myTmpl_plugin);
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


#if QT_VERSION >= 0x040500
            settings.setIniCodec("UTF-8");
#endif
            settings.beginGroup("SERVICE");
            serverHostName = settings.value("server").toString();
            serverPort     = settings.value("port").toInt();
            localSrvName   = settings.value("link_name").toString();
            settings.endGroup();

            settings.beginGroup("POSTSCRIPT");
            gsBin = settings.value("gs_bin").toString();
            settings.endGroup();
            settings.beginGroup("PDF");
            pdftkBin = settings.value("pdfTK").toString();
            settings.endGroup();

            settings.beginGroup("USED_DIR_FILE");
            spoolDir       = settings.value("spool_dir").toString();
#if defined(Q_OS_UNIX)
            ticket_fname = settings.value("session_ticket").toString();
#endif
            tEditor_bin  = settings.value("tEditor_bin").toString();
            settings.endGroup();

            settings.beginGroup("TEMPLATES");
            local_t_path  = settings.value("local_templates").toString();
            global_t_path = settings.value("global_templates").toString();
            settings.endGroup();

#ifdef DEBUG_MODE
            int size = settings.beginReadArray("PrinterList");
            for (int i = 0; i < size; i++) {
                settings.setArrayIndex(i);
                Printers printer;
                printer.name     = settings.value("name").toString();
                printer.ip       = settings.value("ip").toString();
                printer.p_qqueue = settings.value("p_qqueue").toString();
                printer_list.append(printer);
            }
            settings.endArray();
#endif
            // Тестируем переменные
            if ( serverHostName.isEmpty() ||
                 localSrvName.isEmpty()   ||
                 spoolDir.isEmpty()       ||
                 gsBin.isEmpty()          ||
                 pdftkBin.isEmpty()       ||
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
#if defined(Q_OS_UNIX)
    if (Ok){
        //Проверка что файлик трубы не существует, посмотреть где он сохраняется под окошками
        QString file_pipe;
        file_pipe = QString ("/tmp/=%1").arg(localSrvName);
        if (QFile::exists(file_pipe)){
            QFile::remove(file_pipe);
        }
    }
#endif
    return Ok;
}

