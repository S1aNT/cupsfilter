#include <QtGui/QApplication>
#include "mainwindow.h"
#include "config.h"

int main(int argc, char *argv[])
{
QApplication a(argc, argv);
    installLog("t5",QObject::trUtf8("Техносерв А/Г"));
    
    MainWindow w;
    w.show();

    return a.exec();
}
