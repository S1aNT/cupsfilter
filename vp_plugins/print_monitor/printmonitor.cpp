#include "printmonitor.h"
#include "message.h"
#include "templatesinfo.h"

#include <QtCore/QDebug>

#include <QtNetwork/QLocalSocket>

#include <QtCore/QPluginLoader>
#include <QtCore/QMapIterator>
#include <QtCore/QSettings>
#include <QtCore/QProcess>
#include <QtCore/QRegExp>
#include <QtCore/QPoint>
#include <QtCore/QTimer>

#include <QtCore/QFile>

#include <QtGui/QStandardItemModel>
#include <QtGui/QDesktopWidget>
#include <QtGui/QErrorMessage>
#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <QtGui/QPixmap>
#include <QtGui/QBrush>

#define  BUGSHUNTER_NO

#if defined(BUGSHUNTER)

#include <QTableView>

#endif

PrintMonitor::PrintMonitor(QWidget *parent,const QString &input_file)
    : QWizard(parent)
{

    intro_page     = new IntroPage(this);
    select_page    = new SelectPage(this);
    printData_page = new PrintDataPage(this);
    checkData_page = new CheckDataPage(this);
    preview_page   = new PreViewPage(this);

    setPage ( VPrn::Page_Intro,     intro_page  );
    setPage ( VPrn::Page_Select,    select_page );
//    setPage(VPrn::Page_SetBrak, setBrak_page);
    setPage ( VPrn::Page_PrintData, printData_page );
    setPage ( VPrn::Page_CheckData, checkData_page );
    setPage ( VPrn::Page_Preview,   preview_page);

    //    setPage(VPrn::Page_Finish,  finish_page);

    eMsgBox = new QErrorMessage(this);
    setStartId(VPrn::Page_Intro);
#ifndef Q_WS_MAC
    setWizardStyle(ModernStyle);
#endif

    if (input_file.isEmpty() || !QFile::exists(input_file)){
        eMsgBox->showMessage(
                QObject::trUtf8("Не задан исходный файл для печати!"),
                tr("error_message")
                );
        // Добавим метку показывающую что очередь печати пуста
        job_list.enqueue("nothing");
    }else{
        job_list.enqueue(input_file);
        // Добавим метку показывающую что очередь печати пуста
        job_list.enqueue("nothing");
        intro_page->setInputFile();
    }

    setOption(HaveHelpButton, true);
    setPixmap(QWizard::LogoPixmap, QPixmap(":/logo.png"));
    connect(this, SIGNAL(helpRequested()), this, SLOT(showHelp()));
    setWindowTitle(QObject::trUtf8("Защищенный принтер."));

    core_app = new Engine(0,qApp->applicationDirPath(),
                          QObject::trUtf8("Монитор печати "));

    connect (core_app, SIGNAL(reciveGoodBayMsg(QString)),
             this, SLOT(onGoodBayMsg(QString))
             );
    connect(core_app,   SIGNAL( LocalSrvRegistr() ),
            intro_page, SLOT  ( setLocalSrvRegistr() )
            );
    connect(core_app,   SIGNAL( RemoteDemonRegistr() ),
            intro_page, SLOT  ( setRemoteDemonRegistr() )
            );
    /** @short Так как прошли уже успешно пройденны фазы:
      * @li Загрузка плагинов
      * @li Регистрация на локальном сервере (Значит он существует и доступен)
      * @li Регистрация на удаленном сервере (Значит он существует и доступен)
      * можем отправить файлик на конвертацию, в данное время в этом есть смысл
      * можем запросить метаинформацию о шаблонах
      */
    connect(core_app,       SIGNAL( RemoteDemonRegistr()  ),
            this,           SLOT  ( sendFileToConvertor() )
            );

    connect(core_app,       SIGNAL( ReciveUserName() ),
            intro_page,     SLOT  ( setReciveUserName() )
            );
    connect(core_app,       SIGNAL( ReciveUserMandat() ),
            intro_page,     SLOT  ( setReciveUserMandat() )
            );
    connect(core_app,       SIGNAL( ReciveSecLevelList() ),
            intro_page,     SLOT  ( setReciveSecLevelList() )
            );
    connect(core_app,       SIGNAL( ReciveSecLevelList() ),
            this,           SLOT  ( onReciveSecLevelList() )
            );
    connect(core_app,       SIGNAL( RecivePrintersList() ),
            intro_page,     SLOT  ( setRecivePrintersList() )
            );
    connect(core_app,       SIGNAL( needAuthUser(const QString&) ),
            this,           SLOT  ( do_needAuthUser(const QString&) )
            );
    connect(core_app,       SIGNAL( gk_notReady(QString ) ),
            eMsgBox,        SLOT  ( showMessage (QString ) )
            );
    connect(core_app,       SIGNAL( getPrinterList (QStringList& ) ),
            select_page,    SLOT  ( setPrinterList (QStringList& ) )
            );
    connect(core_app,       SIGNAL( doc_converted ()),
            printData_page, SLOT  ( setDocConverted())
            );
    connect(core_app,       SIGNAL( getPagesCount  (int)),
            printData_page, SLOT  ( setPageCounted (int))
            );
    connect(core_app,       SIGNAL( firstPageSplitted()),
            printData_page, SLOT  ( on_firstPageSplitted())
            );
    connect(core_app,       SIGNAL( otherPageSplitted()),
            printData_page, SLOT  ( on_otherPageSplitted())
            );
    connect(printData_page, SIGNAL( field_checked()),
            this,           SLOT  ( check_docMB())
            );
    connect(printData_page, SIGNAL( templatesChanged(int) ),
            core_app,       SLOT  ( findTemplatesFilePathInModel(int) )
            );

    connect (core_app,      SIGNAL( authToDevice(bool,QString) ),
             checkData_page,SLOT  ( setAuthCheck(bool,QString) )
             );
    connect (core_app,      SIGNAL( needRegisterDocInBase() ),
             this,          SLOT  ( onNeedRegisterDocInBase() )
             );
    connect(core_app,       SIGNAL( RegisterDocInBase(bool,QString) ),
            checkData_page, SLOT  ( setMbCheck(bool,QString) )
            );
    connect(core_app,       SIGNAL( error (const QString&)),
            eMsgBox,        SLOT  ( showMessage (const QString&))
            );
    connect (checkData_page,SIGNAL( select_preview(bool) ),
             this,          SLOT  ( onSelectPreview(bool) )
            );
    connect(core_app,       SIGNAL( MergeDocWithTemplates(bool,QString) ),
            checkData_page, SLOT  ( setCheckMergeDocWithTemplates(bool,QString))
            );
    core_app->init();

    // свяжем модель метаинформации о шаблоне и ее основного потребителя
    printData_page->setModel( core_app->getInfoModel() );

#if defined(BUGSHUNTER)
    //!!!!!!!!!!!!!!!!!!!!!!!!!
     QTableView *tableWidget = new QTableView();
     tableWidget->setModel(core_app->getInfoModel());
     tableWidget->show();
    //!!!!!!!!!!!!!!!!!!!!!!!!!
#endif
}

void PrintMonitor::appendStartMsg(const QString &start_msg)
{
    QMessageBox msgbox;
    msgbox.setText(start_msg);
    msgbox.exec();
}

PrintMonitor::~PrintMonitor()
{
    core_app->deleteLater();
}

//------------------------- PRIVATE SLOTS --------------------------------------
void PrintMonitor::onGoodBayMsg(const QString &shutdown_info)
{
    int ret = QMessageBox::warning(this, tr("Print Monitor"),shutdown_info,QMessageBox::Ok);
    if (ret == QMessageBox::Ok){
        QTimer::singleShot(500,qApp,SLOT(quit()));
    }
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
            //intro_page->setReciveUserName();
            core_app->setUserMandat(m_login,UMDlg->getCurrentMandat());
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
                                  "Может стоит обратиться к системному администратору?");

    QMessageBox::information(this, QObject::trUtf8("Справка Защищенного принтера"), message);

    lastHelpMessage = message;
}

void PrintMonitor::sendFileToConvertor()
{
#if defined (MY_DEBUG)
    QString  test = job_list.head();
#endif
    if (field("inputFile").toBool() &&
        job_list.head() != "nothing"){
        core_app->prepareFileToPrint(job_list.dequeue());
    }
}

void PrintMonitor::onReciveSecLevelList()
{
    printData_page->setSecList(core_app->getSecLevelList());
}

void PrintMonitor::check_docMB()
{
    core_app->authUserToPrinter( select_page->getSeclectPrinter() );
    core_app->checkMB( printData_page->getSQL_mb_check() );
}

void PrintMonitor::onNeedRegisterDocInBase()
{
    core_app->registerMB( printData_page->getSQL_mb_register() );
}

void PrintMonitor::onSelectPreview(bool mode)
{
    core_app->doMergeDocWithTemplates(printData_page->getAllFieldData(),mode );
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

/*
int PrintMonitor::nextId() const
{

    int rez = VPrn::Page_Intro;

    switch (currentId()) {
    case VPrn::Page_Intro:
        if (field("connect2LocalServer").toBool() &&
            field("connect2RemoteDemon").toBool() &&
            field("reciveUserName").toBool() &&
            field("reciveUserMandat").toBool() &&
            field("reciveSecLabel").toBool() &&
            field("recivePrintersList").toBool()){
            rez =  VPrn::Page_Select;
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
    case VPrn::Page_PrintData:
        if (printData_page->validator()){
            rez = VPrn::Page_CheckData;
        }else{
            rez = VPrn::Page_PrintData;
        }


        break;
    case VPrn::Page_CheckData:
        //checkData_page->initializePage();

        break;
    }

    return rez;

}
*/
