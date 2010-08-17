#include <QtCore/QVariant>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QSettings>

#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <QtGui/QAction>
#include <QtGui/QStackedWidget>

#include <QtGui/QErrorMessage>

#include <QTableView>


#include "mainwindow.h"
#include "mytypes.h"
#include "engine.h"
#include "getusernamemandatdlg.h"

using namespace VPrn;

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , pageId(0)

{

    //получим размер экрана
    desktop_avail = desktop.availableGeometry(desktop.primaryScreen());

    this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::MinimumExpanding);
    //this->setMinimumSize(320,240);
    this->setMaximumSize(desktop_avail.width()-5,desktop_avail.height()-5);
    this->move( calcDeskTopCenter(this->width(),this->height() ));

    verticalLayoutWidget = new QWidget;
    mainLayout = new QVBoxLayout(verticalLayoutWidget);

    //------------------------------- Центральная часть    -------------------------------
    stackedWidget = new QStackedWidget;
    introPage     = new IntroPage();
    selectPage    = new SelectPage();
    printDataPage = new PrintDataPage();
    checkDataPage = new CheckDataPage();
    preViewPage   = new PreViewPage();
    finishPage    = new FinishPage();

    stackedWidget->addWidget(introPage);
    stackedWidget->addWidget(selectPage);
    stackedWidget->addWidget(printDataPage);
    stackedWidget->addWidget(checkDataPage);
    stackedWidget->addWidget(preViewPage);
    stackedWidget->addWidget(finishPage);
    //Установка макс размеров
    printDataPage->setMaximumSize(desktop_avail.width()-5,desktop_avail.height()-5);

    //------------------------------- Нижний блок - Кнопки -------------------------------
    line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    horizontalLayoutWidget = new QWidget();
    horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);

    helpButton = new QPushButton(horizontalLayoutWidget);
    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    prevButton = new QPushButton(horizontalLayoutWidget);
    nextButton = new QPushButton(horizontalLayoutWidget);
    restartButton = new QPushButton(horizontalLayoutWidget);
    horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    exitButton = new QPushButton(horizontalLayoutWidget);

    helpButton->setText(QObject::trUtf8("Справка"));
    prevButton->setText(QObject::trUtf8("< Назад"));
    nextButton->setText(QObject::trUtf8("Вперед >"));
    restartButton->setText(QObject::trUtf8("В начало"));
    exitButton->setText(QObject::trUtf8("Выход"));

    horizontalLayout->addWidget(helpButton);
    horizontalLayout->addItem(horizontalSpacer);
    horizontalLayout->addWidget(prevButton);
    horizontalLayout->addWidget(nextButton);
    horizontalLayout->addWidget(restartButton);
    horizontalLayout->addItem(horizontalSpacer_2);
    horizontalLayout->addWidget(exitButton);
    //-------------------------------------- Компоновка ----------------------------------------------------
    mainLayout->addWidget(stackedWidget);
    mainLayout->addWidget(line);    
    mainLayout->addWidget(horizontalLayoutWidget);

    setLayout(mainLayout);
    this->setWindowTitle(stackedWidget->currentWidget()->windowTitle() );
}

void MainWindow::init(const QString &app_dir,const QString &input_file)
{
    this->readConfig(app_dir);
    //Основной потребитель ошибок
    eMessage = new QErrorMessage(this);

    //Формируем хранителя данных
    data_module = new DataModule(this);

    //Запуск ядра
    engine = new Engine(this,link_name,gatekeeper_bin);

    createConnection();

    if ( data_module->setWorkFile(input_file)){
        introPage->setInputFile();
    }

    engine->setDocCardModel    ( data_module->getDocCardModel()   );
    engine->setPrintersModel   ( data_module->getPrintersModel()  );
    engine->setStampsModel     ( data_module->getStampsModel()    );
    engine->setTemplatesModel  ( data_module->getTemplatesModel() );
    engine->setMandatsModel    ( data_module->getMandatsModel()   );
    engine->launchAndConnect();

    selectPage->setPrintersModel     ( data_module->getPrintersModel()  );
    printDataPage->setSecListModel   ( data_module->getStampsModel()    );
    printDataPage->setCardDocModel   ( data_module->getDocCardModel()   );
    printDataPage->setTemplatesModel ( data_module->getTemplatesModel() );

}

void MainWindow::appendStartMsg(const QString &txt_msg)
{
    eMessage->showMessage(txt_msg);
}

MainWindow::~MainWindow()
{

}

//------------------------------------PRIVATE SLOTS ------------------------------------
void MainWindow::do_next()
{
    switch (pageId +1){
    case VPrn::Page_Intro:
    case VPrn::Page_Select:
        break;
    case VPrn::Page_PrintData:
        //Увеличим окно
        this->resize(800,600);
        break;
    case VPrn::Page_CheckData:
        //Уменьшим окно        
        this->resize(320,240);
        break;
    case VPrn::Page_Preview:
        this->resize(800,600);
        break;
    case VPrn::Page_Finish:
        break;
    case VPrn::Page_SetBrak:
        break;
    }
    if (lastCheck(pageId)){
        pageId++;
        if ( ( pageId > VPrn::Page_Finish ) || ( pageId > stackedWidget->count()-1 ) ){
            pageId = VPrn::Page_Finish;
        }
        if (pageId == VPrn::Page_Preview){
            // Настроим кнопки
            nextButton->setText(QObject::trUtf8("Печать"));
        }
        stackedWidget->setCurrentIndex(pageId);
        this->setWindowTitle(stackedWidget->currentWidget()->windowTitle() );
    }
}

void MainWindow::do_prev()
{
    switch (pageId - 1){
    case VPrn::Page_Intro:
    case VPrn::Page_Select:
        //Уменьшим окно
        this->resize(320,240);
        break;
    case VPrn::Page_PrintData:
        //Увеличим окно
        this->resize(800,600);
        break;
    case VPrn::Page_CheckData:
        //Уменьшим окно
        this->resize(320,240);
        nextButton->setText(QObject::trUtf8("Вперед >"));
        break;
    case VPrn::Page_Preview:
        //Увеличим окно
        this->resize(800,600);        
        break;
    case VPrn::Page_Finish:
        break;
    case VPrn::Page_SetBrak:
        break;
    }
    pageId--;
    if (pageId < VPrn::Page_Intro) {
        pageId = VPrn::Page_Intro;
    }    
    stackedWidget->setCurrentIndex(pageId);
    this->setWindowTitle(stackedWidget->currentWidget()->windowTitle() );
}

void MainWindow::do_restart()
{
    pageId =0;
    stackedWidget->setCurrentIndex(pageId);
    this->setWindowTitle(stackedWidget->currentWidget()->windowTitle() );
    this->resize(320,240);
    selectPage->needRestart();
    printDataPage->needRestart();
    checkDataPage->needRestart();
    preViewPage->needRestart();
    engine->needRestart();
}

void MainWindow::do_needAuthUser(const QString &login_mandat_list)
{
    QRegExp rx("\\[(.+)\\];:;(.+)");
    //rx.setMinimal(true);
    if(rx.indexIn(login_mandat_list) != -1){
        QString m_login  = rx.cap(1);
        QString m_list  = rx.cap(2);

        // Запуск окошка авторизации
        getUserNameMandatDlg *UMDlg = new getUserNameMandatDlg(this);
        UMDlg->move(this->calcDeskTopCenter(UMDlg->width(),UMDlg->height()));
        UMDlg->setUserName(m_login);
        UMDlg->setMandatList(QStringList() << m_list.split(";:;"));
        connect(UMDlg,SIGNAL(setAuthData(const QString &,const QString &)),
                engine, SLOT(setAuthData(const QString &,const QString &))
                );

        UMDlg->exec();
    }else{
        eMessage->showMessage(QObject::trUtf8("Ошибка разбора сообщения сервера."));
    }
}

void MainWindow::sendFileToConvertor()
{
    engine->prepareFileToPrint(data_module->getWorkFile());
}

//------------------------------------ PRIVATE ---------------------------------------------
void MainWindow::createConnection()
{
    connect (helpButton, SIGNAL(clicked()),
             this, SLOT( showHelp() )
             );
    connect (exitButton,SIGNAL(clicked()),
             this,      SLOT(close())
             );
    connect (nextButton,SIGNAL(clicked()),
             this,      SLOT(do_next())
             );
    connect (prevButton,SIGNAL(clicked()),
             this,      SLOT(do_prev())
             );
    connect (restartButton,SIGNAL(clicked()),
             this,      SLOT(do_restart())
             );

    connect (data_module,SIGNAL(error(QString,QString)),
             eMessage,SLOT(showMessage(QString,QString))
             );

    connect (engine,   SIGNAL( error(QString,QString) ),
             eMessage, SLOT  ( showMessage (QString,QString) )
             );
    connect(engine,    SIGNAL( LocalSrvRegistr() ),
            introPage, SLOT  ( setLocalSrvRegistr() )
            );
    connect(engine,    SIGNAL( RemoteDemonRegistr() ),
            introPage, SLOT  ( setRemoteDemonRegistr() )
            );
    connect(engine,    SIGNAL(ReciveUserName()  ),
            introPage, SLOT  (setReciveUserName() )
            );
    connect(engine,    SIGNAL( ReciveUserMandat() ),
            introPage, SLOT  ( setReciveUserMandat() )
            );

    /** @short Так как прошли уже успешно пройденны фазы:
      * @li Регистрация на локальном сервере (Значит он существует и доступен)
      * @li Регистрация на удаленном сервере (Значит он существует и доступен)
      * можем отправить файлик на конвертацию, в данное время в этом есть смысл
      * можем запросить метаинформацию о шаблонах
      */
    connect(engine,    SIGNAL( RemoteDemonRegistr()  ),
            this,      SLOT  ( sendFileToConvertor() )
            );
    connect(engine,    SIGNAL( needAuthUser(const QString&) ),
            this,      SLOT  ( do_needAuthUser(const QString&) )
            );
    connect(engine,       SIGNAL( ReciveSecLevelList() ),
            introPage,    SLOT  ( setReciveSecLevelList() )
            );
    connect(engine,       SIGNAL( RecivePrintersList() ),
            introPage,    SLOT  ( setRecivePrintersList() )
            );
    connect(selectPage,    SIGNAL (selectedPrinter(int)),
            engine,        SLOT   (do_selectedPrinter(int))
            );
    connect(printDataPage, SIGNAL ( field_checked() ),
            engine,        SLOT   ( authUserToPrinter() )
            );
    connect(printDataPage, SIGNAL ( field_checked() ),
            engine,        SLOT   ( checkMB() )
            );

    connect (engine,       SIGNAL ( authToDevice(bool,QString) ),
             checkDataPage,SLOT   ( setAuthCheck(bool,QString) )
             );

    connect(engine,        SIGNAL ( RegisterDocInBase(bool,QString) ),
            checkDataPage, SLOT   ( setMbCheck(bool,QString) )
            );

    connect(checkDataPage, SIGNAL (select_mode(int)),
            engine,        SLOT   (do_select_mode(int))
            );

    connect(engine,        SIGNAL ( MergeDocWithTemplates(bool,QString) ),
            checkDataPage, SLOT   ( setCheckMergeDocWithTemplates(bool,QString))
            );
    connect (engine,       SIGNAL ( PicturesList(QStringList ) ),
             preViewPage,  SLOT   ( showPicturesList(QStringList ))
             );
    connect (preViewPage,  SIGNAL (printCurrentDoc()),
             engine,       SLOT   (do_printCurrentDoc())
             );


}

QPoint MainWindow::calcDeskTopCenter(int width,int height)
{
    QPoint centerWindow;
    //получаем прямоугольник с размерами как у экрана
    centerWindow = desktop_avail.center(); //получаем координаты центра экрана
    centerWindow.setX(centerWindow.x() - (width/2));
    centerWindow.setY(centerWindow.y() - (height/2));
    return centerWindow;
}

void MainWindow::readConfig(const QString &app_dir)
{
    QString ini_file =QString("%1/Technoserv/safe_printer.ini").arg(app_dir);
    if (QFile::exists(ini_file)){
        QSettings settings (ini_file,QSettings::IniFormat);

#if QT_VERSION >= 0x040500
        settings.setIniCodec("UTF-8");
#endif

        settings.beginGroup("SERVICE");
        link_name      = settings.value("link_name").toString();
        settings.endGroup();

        settings.beginGroup("USED_DIR_FILE");
        //spoolDir       = settings.value("spool_dir").toString();
        gatekeeper_bin = settings.value("gatekeeper_bin").toString();
        settings.endGroup();
    }else{
        eMessage->showMessage(QObject::trUtf8("Файл с настройками программы %1 не найден!\n%2")
                              .arg(ini_file)
                              .arg(QString(Q_FUNC_INFO))
                              );
    }
}

bool MainWindow::lastCheck(int page)
{
    bool Ok = false;
    {
        switch (page +1){
        case VPrn::Page_Intro:
            break;
        case VPrn::Page_Select:
            Ok = introPage->enableNext();
            break;
        case VPrn::Page_PrintData:
            Ok = selectPage->enableNext();
            break;
        case VPrn::Page_CheckData:
            Ok = printDataPage->enableNext();
            break;
        case VPrn::Page_Preview:
            Ok = checkDataPage->enableNext();
            break;
        case VPrn::Page_Finish:
            Ok = preViewPage->enableNext();
            break;
        case VPrn::Page_SetBrak:
            break;
        }
    }
    return Ok;

}

//------------------------------------- PROTECTED ----------------------------------------
void MainWindow::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        //ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
