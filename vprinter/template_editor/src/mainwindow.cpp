#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "docview.h"
#include <QtGui>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    DocView *view = new DocView(trUtf8("Пример окна"));
   setCentralWidget(view);
 }

MainWindow::~MainWindow()
{
    delete ui;
}
