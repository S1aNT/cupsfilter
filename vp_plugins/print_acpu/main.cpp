#include <QtGui/QApplication>
#include "maindialog.h"
#include "config.h"
#include "glass.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(images);

    QApplication a(argc, argv);
    installLog("t5",QObject::trUtf8("Техносерв А/Г"));
    
    Glass *infoGlass = new Glass();
    infoGlass->enableInfoTextBlock(QString("Test messages."));



    mainDialog dlg;
    infoGlass->install (dlg);
    dlg.show();

    return a.exec();
}
