/**
  * Программа просмотра регистрируемых событий
  */
#include <QtGui/QApplication>
#include <QDebug>
#include <QTextCodec>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);
    qInstallMsgHandler(myMessageOutput);

    QApplication app(argc, argv);
    Q_INIT_RESOURCE(images);

    MainWindow window;
    window.show();
    return app.exec();
}
