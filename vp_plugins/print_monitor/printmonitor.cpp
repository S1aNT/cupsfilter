#include "printmonitor.h"
#include "message.h"


#include "ui_printmonitor.h"

#include <QtCore/QSettings>
#include <QtCore/QPluginLoader>
#include <QtCore/QProcess>
#include <QtCore/QFile>
#include <QtCore/QRegExp>
#include <QtCore/QPoint>

#include <QtNetwork/QLocalSocket>

#include <QtGui/QApplication>
#include <QtGui/QBrush>
#include <QtGui/QDesktopWidget>
#include <QtGui/QMessageBox>

PrintMonitor::PrintMonitor(QWidget *parent)
    : QWizard(parent)
    //, ui(new Ui::PrintMonitor)
{
    //ui->setupUi(this);
    intro_page = new IntroPage(this);
    select_page  = new SelectPage(this);
    setPage ( VPrn::Page_Intro,  intro_page  );
    setPage ( VPrn::Page_Select, select_page );

    //    setPage(VPrn::Page_SetData, setData_page);
    //    setPage(VPrn::Page_SetBrak, setBrak_page);
    //    setPage(VPrn::Page_Finish,  finish_page);

    //setStartId(VPrn::Page_Select);
    setStartId(VPrn::Page_Intro);
#ifndef Q_WS_MAC
    setWizardStyle(ModernStyle);
#endif

    setOption(HaveHelpButton, true);
    setPixmap(QWizard::LogoPixmap, QPixmap(":/logo.png"));
    connect(this, SIGNAL(helpRequested()), this, SLOT(showHelp()));
    setWindowTitle(QObject::trUtf8("Защищенный принтер."));

    core_app = new Engine(0,qApp->applicationDirPath(),QObject::trUtf8("Монитор печати "));

    connect(core_app,   SIGNAL( pluginsLoad() ),
            intro_page, SLOT  ( setPluginsLoad() )
            );
    connect(core_app,   SIGNAL( LocalSrvRegistr() ),
            intro_page, SLOT  ( setLocalSrvRegistr() )
            );
    connect(core_app,   SIGNAL( RemoteDemonRegistr() ),
            intro_page, SLOT  ( setRemoteDemonRegistr() )
            );
    connect(core_app,   SIGNAL( ReciveUserName() ),
            intro_page, SLOT  ( setReciveUserName() )
            );
    connect(core_app,   SIGNAL( ReciveUserMandat() ),
            intro_page, SLOT  ( setReciveUserMandat() )
            );
    connect(core_app,   SIGNAL( ReciveSecLevelList() ),
            intro_page, SLOT  ( setReciveSecLevelList() )
            );
    connect(core_app,   SIGNAL( RecivePrintersList() ),
            intro_page, SLOT  ( setRecivePrintersList() )
            );

    connect(core_app,SIGNAL( infoMessage(QString) ),
            this,    SLOT  ( showInfoMessage(QString) )
            );
    connect(core_app,SIGNAL( needAuthUser(const QString&) ),
            this,    SLOT  ( do_needAuthUser(const QString&) )
            );

    connect(core_app,SIGNAL( gk_notReady(QString) ),
            this,    SLOT  ( showCritMessage(QString) )
            );
    connect(core_app,SIGNAL( getPrinterList (const QString&)),
            this,    SLOT  ( setPrinterList (const QString&))
            );
    core_app->init();
}

PrintMonitor::~PrintMonitor()
{
    //    delete ui;
}

//void PrintMonitor::changeEvent(QEvent *e)
//{
//    QWizard::changeEvent(e);
//    switch (e->type()) {
//    case QEvent::LanguageChange:
//        ui->retranslateUi(this);
//        break;
//    default:
//        break;
//    }
//}

//int PrintMonitor::nextId() const
//{
//    switch (currentId()) {
//    case Page_Intro:
//        if (field("loadPlugin*").toBool() &&
//            field("connect2LocalServer*").toBool() &&
//            field("connect2RemoteDemon*").toBool() &&
//            field("reciveUserName*").toBool() &&
//            field("reciveUserMandat*").toBool() &&
//            field("reciveSecLabel*").toBool() &&
//            field("recivePrintersList*").toBool()){
//            return Page_Select;
//
//        } else {
//            //return Page_Register;
//            return -1;
//        }
//        break;
//         default:
//        return -1;
//    }
//}
//------------------------- PRIVATE SLOTS --------------------------------------
void PrintMonitor::showInfoMessage(const QString &info)
{
    //    QListWidgetItem *item = new QListWidgetItem(ui->infoListWidget,QListWidgetItem::UserType);
    //    item->setText(info);
    //    item->setForeground(QBrush(Qt::black));
}

void PrintMonitor::showCritMessage(const QString &info)
{
    //    QListWidgetItem *item = new QListWidgetItem(ui->infoListWidget,QListWidgetItem::UserType);
    //    item->setText(info);
    //    item->setForeground(QBrush(Qt::red));

}

void PrintMonitor::do_needAuthUser(const QString &login_mandat_list)
{
    QRegExp rx("\\[(.+)\\];:;(.+)");
    //rx.setMinimal(true);
    if(rx.indexIn(login_mandat_list) != -1){
        QString m_login  = rx.cap(1);
        QString m_list  = rx.cap(2);

        // Запуск окошка авторизации
        UMDlg = new getUserNameMandatDlg(this);
        UMDlg->move(this->getDeskTopCenter(UMDlg->width(),UMDlg->height()));
        UMDlg->setUserName(m_login);
        UMDlg->setMandatList(QStringList() << m_list.split(";:;"));

        int ret = UMDlg->exec();
        if (ret == QDialog::Accepted){
            intro_page->setReciveUserName();
            core_app->setUserMandat(UMDlg->getCurrentMandat());
            showInfoMessage( QObject::trUtf8("Авторизация с консоли") );
        }
    }else{
        showCritMessage(QObject::trUtf8("Ошибка разбора сообщения сервера."));
    }
}

void PrintMonitor::setPrinterList (const QString &pList)
{
    QStringList remote_printer;
    QStringList devices_info;
    QString pline;
    remote_printer = pList.split("###;:;");
    for (int i = 0; i < remote_printer.size(); i++) {

        pline = remote_printer.at(i);
        pline.replace("###","");
        devices_info = pline.split(";:;");
        printer_device_list.insert(devices_info.at(0),devices_info.at(1));
        pline = devices_info.at(0);
        printer_list.append(pline.section(".",1,1)); // Имя принтера после точки
        showInfoMessage( QObject::trUtf8("Добавлен принтер %1").arg(pline.section(".",1,1)) );
    }
}

void PrintMonitor::showHelp()
{
    static QString lastHelpMessage;

    QString message;

    switch (currentId()) {
    case Page_Intro:
        message = QObject::trUtf8("На данном этаме происходит инициализация виртуального защищеного "
                     "принтера. В случае ошибки иницализации не выбранный пункт укажет на место ошибки.");
        break;
    default:
        message = QObject::trUtf8("Такая справка лучше, чем ее полное отсутствие.");
    }

    if (lastHelpMessage == message)
        message = QObject::trUtf8("Извините по данному режиму работы нет помощи. "
                     "Может стоит обратьтиться к системному администратору?");

    QMessageBox::information(this, QObject::trUtf8("Справка Защищенного принтера"), message);

    lastHelpMessage = message;
}

//------------------------------- PRVATE ---------------------------------------
QPoint PrintMonitor::getDeskTopCenter(int width,int height)
{
    QDesktopWidget desktop;
    QPoint centerWindow;

    QRect rect = desktop.availableGeometry(desktop.primaryScreen());
    //получаем прямоугольник с размерами как у экрана
    centerWindow = rect.center(); //получаем координаты центра экрана
    centerWindow.setX(centerWindow.x() - (width/2));
    centerWindow.setY(centerWindow.y() - (height/2));
    return centerWindow;
}
