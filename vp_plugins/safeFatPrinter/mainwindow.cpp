#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    errorMessageDialog = new QErrorMessage(this);

    SpiderInTheMiddle = new Mediator(this);
    connect (SpiderInTheMiddle,SIGNAL(StateChanged(PluginState)),this,SLOT(updateStatusBar(PluginState)));

    connect (SpiderInTheMiddle,SIGNAL(error(QString &)),errorMessageDialog,SLOT(showMessage(QString &)));
    SpiderInTheMiddle->loadPlugin(qApp->applicationDirPath());

   connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(getUserName()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::showError(QString &info)
{


}

void MainWindow::updateStatusBar(Mediator::PluginState state)
{
    QString msg=QObject::trUtf8("Плагин: %1 успешно загружен.");
    switch (state){
    case Mediator::netPluginLoaded:
        msg.arg("Работы с сетью");
        break;
    case Mediator::authPluginLoaded:
        msg.arg("Аутентификации пользователя");
        break;
    case Mediator::gsPluginLoaded:
        msg.arg("Обработки ps и pdf документов");
        break;
    }
    statusBar()->showMessage(msg);
}
void MainWindow::showInfo(QString info)
{
    QMessageBox msgBox;
    msgBox.setObjectName("info_msg_box");
    msgBox.setText(info);
    msgBox.addButton(QObject::trUtf8("OK"),QMessageBox::AcceptRole);
    msgBox.exec();
}

void MainWindow::getUserName()
{
    //net_plugin_Interface->sendData(tr("/cmd;:;300;:;test"));
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




