#include <QApplication>
#include <QTextCodec>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(images);
        QTextCodec *codec = QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForTr(codec);
        //QTextCodec::setCodecForLocale(cyrillicCodec);
        QTextCodec::setCodecForCStrings(codec);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
