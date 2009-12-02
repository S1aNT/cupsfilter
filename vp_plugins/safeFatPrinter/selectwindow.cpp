#include "selectwindow.h"
#include "ui_selectwindow.h"

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
    WorkDlg = new workField(this);
    WorkDlg->setWindowFlags(Qt::Dialog |  Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::WindowSystemMenuHint);


    this->setWindowFlags(Qt::Dialog |  Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::WindowSystemMenuHint);
    this->move(calcCenter());
    connect (SpiderInTheMiddle,SIGNAL(StateChanged(WorkStep)),this,SLOT(checkPoint(WorkStep)));
    connect (SpiderInTheMiddle,SIGNAL(error (QString )),this,SLOT(showCritError(QString)));
    connect (SpiderInTheMiddle,SIGNAL(pluginMessage(const QString &)),this,SLOT (showPluginMessage(const QString &)));
    connect (SpiderInTheMiddle,SIGNAL(needShowAuthWindow(QString &)),this,SLOT(showAuthWindow(QString&)));

    connect (ui->printerCBox,SIGNAL(currentIndexChanged(QString)),SpiderInTheMiddle,SLOT(authToPrinter(QString)));

    //connect (qApp,SIGNAL(aboutToQuit()),this,SLOT(cleanUp()));

    connect(ui->printModeAccounting, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->printFromAccountPaper, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->printWithAccountingPaper, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(ui->printModeAccounting,0 );
    signalMapper->setMapping(ui->printFromAccountPaper,1);
    signalMapper->setMapping(ui->printWithAccountingPaper,2 );
    connect(signalMapper, SIGNAL(mapped(int)),  this, SLOT(setMode(int)));
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
    ui->printerCBox->setModel(SpiderInTheMiddle->printers_model());
    ui->printerCBox->setEnabled(true);
    ui->printFromAccountPaper->setEnabled(true);
    ui->printModeAccounting->setEnabled(true);
    ui->printWithAccountingPaper->setEnabled(true);
}

void SelectWindow::setMode (int signal_mode)
{
    work_mode = signal_mode;
    QString title;
    switch (signal_mode){
    case 0:
        title = QObject::trUtf8("Режим работы: [Учет листов]");
        WorkDlg->setWindowTitle(title);
        WorkDlg->move(calcCenter());
        break;
    case 1:
        break;
    case 2:
        break;
    }
    int ret = WorkDlg->exec();

}

void SelectWindow::showCritError(QString e_msg)
{
    QMessageBox msgBox;
    QPushButton *abortButton;
    msgBox.setIcon(QMessageBox::Critical);
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
    UMDlg->setMandatModel(SpiderInTheMiddle->mandat_model());
    UMDlg->move(this->calcCenter());
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

QPoint SelectWindow::calcCenter()
{
    QDesktopWidget desktop;
    QPoint centerWindow;

    QRect rect = desktop.availableGeometry(desktop.primaryScreen());
    //получаем прямоугольник с размерами как у экрана
    centerWindow = rect.center(); //получаем координаты центра экрана
    centerWindow.setX(centerWindow.x() - (this->width()/2));
    centerWindow.setY(centerWindow.y() - (this->height()/2));
    return centerWindow;
}
