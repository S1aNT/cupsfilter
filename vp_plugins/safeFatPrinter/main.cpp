#include <QtGui/QApplication>
#include <QDebug>
#include <QTextCodec>
#include <QTimer>

//#include "mediator.h"
//#include "mainwindow.h"
#include "selectwindow.h"

#include "config.h"
#include "tech_global.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(images);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    //QTextCodec::setCodecForTr( QTextCodec::codecForName( "cp866" ) );
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);
    qInstallMsgHandler(myMessageOutput);

    QApplication app(argc, argv);
    QStringList aList=app.arguments();
    if (aList.size()< 2){
        QMessageBox::critical(0,QObject::trUtf8("Обратитесь к системному администратору"),
                              QObject::trUtf8("Ошибка запуска приложения, не достаточно параметров запуска"));
       return 1;
    }else {
        QString in_file=aList.at(1);
        SelectWindow window;
        window.setFileToWork(in_file);
        window.loadPlugin(qApp->applicationDirPath());
        //app.processEvents();
        window.show();
        return app.exec();
    }

}
