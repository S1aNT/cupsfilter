#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /*
    SpiderInTheMiddle = new Mediator(this);
    //connect (SpiderInTheMiddle,SIGNAL(StateChanged(plugin_state_t )),this,SLOT(updateStatusBar(plugin_state_t)));
    connect (SpiderInTheMiddle,SIGNAL(error(QString &)),this,SLOT(showError(QString &)));
    connect (SpiderInTheMiddle,SIGNAL(needShowAuthWindow(QString &)),this,SLOT(showAuthWindow(QString &)));
    */
}

void MainWindow::setSpider(Mediator *spider)
{
    spiderInTheMiddle =spider;
}



void MainWindow::enableGUI()
{
    //statusBar()->showMessage( );
}

void MainWindow::showAuthWindow(QString &user_name)
{
    /*

    QString msg;
    UMDlg = new getUserNameMandatDlg(this);
    UMDlg->setUserName(user_name);
    //UMDlg->setMandatModel(SpiderInTheMiddle->mandat_model());
    //TODO изменить на центр экрана
    QPoint pos = QPoint(300, 300);
    UMDlg->move(pos);
    int ret = UMDlg->exec();
    if (ret ==QDialog::Accepted){
        //SpiderInTheMiddle->setUserMandat(UMDlg->getCurrentUserMandat());

        msg= QObject::trUtf8("Авторизация с консоли");
        this->statusBar()->showMessage(msg);
        //control->getStampName();
        //control->getPrinterCount();
    }else{

        //msg= QObject::trUtf8("Ошибка получения имени пользователя или его мандата");
    }
*/
}

MainWindow::~MainWindow()
{
    delete ui;
}


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
void MainWindow::showError(QString &info)
{
    this->Mode=ERROR;

    QMessageBox msgBox;
    msgBox.setObjectName("info_msg_box");
    msgBox.setText(info);
    msgBox.addButton(QObject::trUtf8("OK"),QMessageBox::AcceptRole);
    msgBox.exec();
}

void MainWindow::getUserName()
{
    //net_plugin_Interface->sendData(tr("/cmd;:;300;:;test"));
   QFileDialog *f_dialog = new QFileDialog(this);
   //connect (f_dialog,SIGNAL(fileSelected(QString)),this,SLOT(showInfo(QString)));
//   connect (f_dialog,SIGNAL(fileSelected(QString &)),SpiderInTheMiddle,SLOT(convert2pdf(QString &)));
   f_dialog->exec();

}

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




