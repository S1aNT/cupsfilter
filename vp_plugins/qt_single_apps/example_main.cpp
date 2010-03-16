#include <QtGui/QApplication>
#include "mainwindow.h"
#include "qtsapp/qtsingleapplication.h"

int main(int argc,char** argv)
{
    QtSingleApplication app(argc, argv, true);
    if (app.sendMessage("uinst?") || app.isRunning())
        return 0;

    MainWindow mainWnd;
    mainWnd.show();

    app.setActivationWindow(&mainWnd);

    return app.exec();
}
