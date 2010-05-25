#include <QtCore/QtDebug>
#include <QtCore/QTextCodec>

#include "mainwindow.h"

int main(int argc, char *argv[])
{

    Q_INIT_RESOURCE(images);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);

    //qInstallMsgHandler(myMessageOutput);

    QApplication app(argc, argv);

    MainWindow window;
    window.show();
    window.test();
    return app.exec();
}
