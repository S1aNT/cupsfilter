#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if (!loadPlugin()) {
        statusBar()->showMessage(tr("Could not load the plugin"));
    }else {
        statusBar()->showMessage(tr("Ready"));
     }
   connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(getUserName()));

}

MainWindow::~MainWindow()
{
    delete ui;
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
    net_plugin_Interface->sendData(tr("/cmd;:;300;:;test"));
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




