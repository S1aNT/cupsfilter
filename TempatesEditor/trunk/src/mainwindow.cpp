#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "View.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    View *view = new View();
    view->setTemplatesList(QStringList() << QObject::trUtf8("Сов. секретный приказ"));
    setCentralWidget(view);
    this->resize(800,600);
}

MainWindow::~MainWindow()
{
    delete ui;
}

