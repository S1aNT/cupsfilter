#include <QtGui/QApplication>
#include <QDebug>
#include <QTextCodec>

#include "config.h"
#include "tech_global.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(images);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    //QTextCodec::setCodecForTr( QTextCodec::codecForName( "cp866" ) );
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);
    qInstallMsgHandler(myMessageOutput);

    MainWindow window;
    window.loadPlugin(qApp->applicationDirPath());
        //app.processEvents();
    window.show();
    return app.exec();

}
