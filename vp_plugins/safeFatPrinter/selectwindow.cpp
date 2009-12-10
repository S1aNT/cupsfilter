#include "selectwindow.h"
#include "ui_selectwindow.h"
#include <QTimer>
#include <QDir>

#include "getusernamemandatdlg.h"

SelectWindow::SelectWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::SelectWindow)
{
    ui->setupUi(this);
    ui->printerCBox->setEnabled(false);
    ui->printFromAccountPaper->setEnabled(false);
    ui->printModeAccounting->setEnabled(false);
    ui->printWithAccountingPaper->setEnabled(false);

    QString msg=QObject::trUtf8("Выбери принтер и режим работы");
    this->setWindowTitle(msg);

    SpiderInTheMiddle = new Mediator(this);
    UMDlg = new getUserNameMandatDlg(this);

    signalMapper = new QSignalMapper(this);

    this->setWindowFlags(Qt::Dialog |  Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::WindowSystemMenuHint);
    this->move(SpiderInTheMiddle->getDeskTopCenter(this->width(),this->height()));

    connect (SpiderInTheMiddle,SIGNAL(StateChanged(WorkStep)),this,SLOT(checkPoint(WorkStep)));
    connect (SpiderInTheMiddle,SIGNAL(error (QString )),this,SLOT(showCritError(QString)));
    connect (SpiderInTheMiddle,SIGNAL(pluginMessage(const QString &)),this,SLOT (showPluginMessage(const QString &)));
    connect (SpiderInTheMiddle,SIGNAL(needShowAuthWindow(QString &)),this,SLOT(showAuthWindow(QString&)));
    connect (ui->printerCBox,
             SIGNAL(currentIndexChanged(QString)),
             SpiderInTheMiddle,
             SLOT(setCurrentPrinter(QString))
             );


    connect (qApp,SIGNAL(aboutToQuit()),this,SLOT(cleanUp()));

    connect(ui->printModeAccounting, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->printFromAccountPaper, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->printWithAccountingPaper, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(ui->printModeAccounting,0 );
    signalMapper->setMapping(ui->printFromAccountPaper,1);
    signalMapper->setMapping(ui->printWithAccountingPaper,2 );
    connect(signalMapper, SIGNAL(mapped(int)),  SpiderInTheMiddle, SLOT(setMode(int)));
}


void SelectWindow::cleanUp()
{
    // Великая чистка 37 года
    QDir dir(SpiderInTheMiddle->getSpoolDir());
    QStringList filters;
    filters << QObject::trUtf8("*%1*.*").arg(SpiderInTheMiddle->getSeansSid());
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setNameFilters(filters);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        QFile::remove(fileInfo.absoluteFilePath());
    }

}

void SelectWindow::checkPoint(WorkStep step)
{
    switch (step){
    case authPluginInit:
        break;
    case netPluginInit:
        connected=true;
        break;
    case gsPluginInit:
        SpiderInTheMiddle->convert2pdf(mainFileName);
        break;
    case psToPdfConverted:
        converted=true;
        break;
    case filledPrinterList:
        if (connected && converted){
            this->enableGUI();//
        }
        break;
    }

}

void SelectWindow::enableGUI()
{
    ui->printerCBox->setModel(SpiderInTheMiddle->getPrintersModel());
    ui->printerCBox->setEnabled(true);
    ui->printFromAccountPaper->setEnabled(true);
    ui->printModeAccounting->setEnabled(true);
    ui->printWithAccountingPaper->setEnabled(true);
}

void SelectWindow::setMode (int signal_mode)
{
    /*
    work_mode = signal_mode;
    QString title;
    WorkDlg->move(SpiderInTheMiddle->getDeskTopCenter(WorkDlg->width(),WorkDlg->height()));
    WorkDlg->setPageSizeModel(SpiderInTheMiddle->getPageSizeModel());
    WorkDlg->setUserName(SpiderInTheMiddle->getUserName());
    switch (signal_mode){
    case 0:
        title = QObject::trUtf8("Режим работы: [Учет листов]");
        break;
    case 1:
        title = QObject::trUtf8("Режим работы: [Печать на учтенных листах]");
        break;
    case 2:
        title = QObject::trUtf8("Режим работы: [Печать документа с автоматическим учетом листов]");
        break;
    }
    WorkDlg->setWindowTitle(title);
    int ret = WorkDlg->exec();
    if (ret == QDialog::Accepted){
        QTimer::singleShot(500,qApp,SLOT(quit()));
    }
    */
}

void SelectWindow::showCritError(QString e_msg)
{
    QMessageBox msgBox;
    QPushButton *abortButton;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle(QObject::trUtf8("Информационное сообщение"));
    msgBox.setInformativeText(QObject::trUtf8("Для решения этой проблемы обратитесь к администратору безопасности!"));
    abortButton=msgBox.addButton(QObject::trUtf8("Выход"), QMessageBox::RejectRole);
    msgBox.setText(e_msg);
    QObject::connect(&msgBox,SIGNAL(rejected()),qApp,SLOT(quit()));
    msgBox.exec();
}

void SelectWindow::showAuthWindow(QString &user_name)
{
    this->hide();
    QString msg;
    UMDlg->setUserName(user_name);
    UMDlg->setMandatModel(SpiderInTheMiddle->getMandatModel());
    //UMDlg->move(SpiderInTheMiddle->getDeskTopCenter(UMDLg->width(),UMDLg->height()));
    int ret = UMDlg->exec();
    if (ret == QDialog::Accepted){
        SpiderInTheMiddle->setUserMandat(UMDlg->getCurrentMandat());
        msg= QObject::trUtf8("Авторизация с консоли");
        this->statusBar()->showMessage(msg);
    }else{
        close();
        //msg= QObject::trUtf8("Ошибка получения имени пользователя или его мандата");
    }
    this->show();
}

void SelectWindow::showPluginMessage(const QString &message)
{
    statusBar()->showMessage(message);
}

SelectWindow::~SelectWindow()
{
    delete ui;
}

void SelectWindow::loadPlugin(const QString &app_dir)
{
    SpiderInTheMiddle->loadPlugin(app_dir);
}

void SelectWindow::setFileToWork(QString &in_file)
{
    mainFileName=in_file;
}

void SelectWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


