#include "printmonitor.h"
#include "message.h"


//#include "ui_printmonitor.h"

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
#include <QtGui/QPixmap>
#include <QtGui/QMessageBox>
#include <QtGui/QErrorMessage>

PrintMonitor::PrintMonitor(QWidget *parent)
    : QWizard(parent)
    //, ui(new Ui::PrintMonitor)
{
    job_list.enqueue("nothing");// Добавим метку показывающую что очередь печати пуста
    //ui->setupUi(this);
    intro_page = new IntroPage(this);
    select_page  = new SelectPage(this);
    printData_page = new PrintDataPage(this);

    setPage ( VPrn::Page_Intro,  intro_page  );
    setPage ( VPrn::Page_Select, select_page );
    setPage ( Page_PrintData,    printData_page );
    //    setPage(VPrn::Page_SetBrak, setBrak_page);
    //    setPage(VPrn::Page_Finish,  finish_page);

    eMsgBox = new QErrorMessage(this);

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
    connect(core_app,       SIGNAL( getPrinterList (const QString&)),
            select_page,    SLOT  ( setPrinterList (const QString&))
            );
    connect(core_app,       SIGNAL( doc_converted ()),
            printData_page, SLOT  ( setDocConverted())
            );
    connect(core_app,       SIGNAL( getPagesCount  (int)),
            printData_page, SLOT  ( setPageCounted (int))
            );
    connect(core_app,       SIGNAL( next_page()),
            this,           SLOT  ( print_next())
            );
    connect(core_app,SIGNAL( error (const QString&)),
            eMsgBox, SLOT  ( showMessage (const QString&))
            );
    core_app->init();
}

PrintMonitor::~PrintMonitor()
{
    //    delete ui;
}


int PrintMonitor::nextId() const
{
    int rez;
    switch (currentId()) {
    case VPrn::Page_Intro:
        if (field("loadPlugin").toBool() &&
            field("connect2LocalServer").toBool() &&
            field("connect2RemoteDemon").toBool() &&
            field("reciveUserName").toBool() &&
            field("reciveUserMandat").toBool() &&
            field("reciveSecLabel").toBool() &&
            field("recivePrintersList").toBool()){
            rez =  Page_Select;
            // Запуск процесса конвертации ps -> pdf
        } else {            
            rez = VPrn::Page_Intro;
        }
        break;

    case VPrn::Page_Select:
        if (field("markBrakDoc").toBool()){
            rez =  VPrn::Page_SetBrak;
        }else{
            if (field("accountingDoc").toBool()){
                printData_page->setLabelText(QObject::trUtf8("Учет документа в БД"));                
                rez = VPrn::Page_PrintData;
                printData_page->setMode(0);
            }else {
                if (field("printDoc").toBool() ){
                    printData_page->setLabelText(QObject::trUtf8("Печать документа на учтенных листах"));
                    printData_page->setMode(1);
                    rez = VPrn::Page_PrintData;
                }else{
                    if (field("both_step").toBool()){
                        printData_page->setLabelText(QObject::trUtf8("Печать документа на учтенных листах, с автоматическим учетом в БД"));
                        printData_page->setMode(2);
                        rez = VPrn::Page_PrintData;
                    }else{
                        rez = VPrn::Page_Select;
                    }
                }
            }
        }
        break;
    }
    return rez;
}

//----------------------------- PUBLIC SLOTS -------------------------------------
void PrintMonitor::setFile4Work(const QString &input_file)
{
    if (input_file.isEmpty() || !QFile::exists(input_file)){
        this->setToolTip(QObject::trUtf8("Не задан исходный файл для печати!"));
    }else{
        job_list.enqueue(input_file);
    }
}
//------------------------- PRIVATE SLOTS --------------------------------------
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
            //showInfoMessage( QObject::trUtf8("Авторизация с консоли") );
        }
    }else{
        eMsgBox->showMessage(QObject::trUtf8("Ошибка разбора сообщения сервера."));
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

void PrintMonitor::print_next()
{
    if (job_list.head() != "nothing"){
      //  QString p_file = ;
         core_app->prepareFileToPrint(job_list.dequeue());
    }

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
