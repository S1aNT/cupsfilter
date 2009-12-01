#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QDesktopWidget desktop;
    QRect rect = desktop.availableGeometry(desktop.primaryScreen());
    //получаем прямоугольник с размерами как у экрана
    centerWindow = rect.center(); //получаем координаты центра экрана
    centerWindow.setX(centerWindow.x() - (this->width()/2));
    centerWindow.setY(centerWindow.y() - (this->height()/2));


    SpiderInTheMiddle = new Mediator(this);
    UMDlg = new getUserNameMandatDlg(this);
    askDlg = new firstAsk(this);

    askDlg->setWindowFlags(Qt::Dialog |  Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::WindowSystemMenuHint);

    connect (SpiderInTheMiddle,SIGNAL(error (QString )),this,SLOT(showCritError(QString)));
    connect (SpiderInTheMiddle,SIGNAL(needShowAuthWindow(QString &)),this,SLOT(showAuthWindow(QString&)));
    connect (SpiderInTheMiddle,SIGNAL(needShowSelectWindow()),this,SLOT(showSelectWindow()));
    connect (SpiderInTheMiddle,SIGNAL(pluginLoad(const QString &,int, const QColor &)),this,SIGNAL(pluginLoad(const QString &,int, const QColor &)));
    connect (qApp,SIGNAL(aboutToQuit()),this,SLOT(cleanUp()));
    connect (askDlg,SIGNAL(printerSelected(QString)),SpiderInTheMiddle,SLOT(authToPrinter(QString)));
}

void MainWindow::setFileToWork(QString &in_file)
{
    mainFileName=in_file;
}

void MainWindow::loadPlugin(const QString &app_dir)
{
    SpiderInTheMiddle->loadPlugin(app_dir);
}

void MainWindow::showCritError(QString e_msg)
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

void MainWindow::cleanUp()
{
    // в 1937 это называлось ЗАЧИСТКОЙ :)

    qDebug() << Q_FUNC_INFO << "cleanup" << this->sender();
}

//************************ Private slots ***************************
void MainWindow::checkPluginReady()
{
    emit closeSplash();
}

void MainWindow::showSelectWindow()
{
    QString msg;
    msg=QObject::trUtf8("[%1] Выбери принтер и режим работы").arg(SpiderInTheMiddle->getUserName());
    askDlg->setWindowTitle(msg);
    int ret = askDlg->exec();
    if (ret == QDialog::Accepted){
        emit closeSplash();
        this->show();
        switch (askDlg->getCurrentMode()){
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        }
    }
}

void MainWindow::showAuthWindow(QString &user_name)
{
    QString msg;
    UMDlg->setUserName(user_name);
    UMDlg->setMandatModel(SpiderInTheMiddle->mandat_model());
    UMDlg->move(centerWindow);
    int ret = UMDlg->exec();
    if (ret == QDialog::Accepted){
        SpiderInTheMiddle->setUserMandat(UMDlg->getCurrentMandat());

        msg= QObject::trUtf8("Авторизация с консоли");
        this->statusBar()->showMessage(msg);
    }else{
        close();
        //msg= QObject::trUtf8("Ошибка получения имени пользователя или его мандата");
    }

}
//***************************************************************************************



MainWindow::~MainWindow()
{
    delete ui;
}

//***************************************************************************************

/*
void MainWindow::updateStatusBar(plugin_state_t state)
{
    QString msg;
    switch (state){
    case netPluginLoaded:
        msg= QObject::trUtf8("Плагин: [Работы с сетью] успешно загружен.");
        break;
    case authPluginLoaded:
        msg = QObject::trUtf8("Плагин: [Аутентификации пользователя] успешно загружен.");
        break;
    case gsPluginLoaded:
        msg =QObject::trUtf8("Плагин: [Обработки ps и pdf документов] успешно загружен.");
        break;
    case connectedToDemon:
        msg =QObject::trUtf8("Успешно соединились с сервером безопасности");
        break;
    }
    statusBar()->showMessage(msg);
}
*/




void MainWindow::changeEvent(QEvent *e)
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




