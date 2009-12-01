#include <QtGui/QApplication>
#include <QDebug>
#include <QTextCodec>
#include <QSplashScreen>
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
        SelectWindow window;
        QString in_file=aList.at(1);
        window.loadPlugin(qApp->applicationDirPath());
        window.setFileToWork(in_file);
        app.processEvents();
        window.show();
        return app.exec();

        /*
        QPixmap pixmap(":images/splash.png");
        QSplashScreen *splash  = new QSplashScreen(pixmap);
        QObject::connect (&window,SIGNAL(pluginLoad(const QString &,int, const QColor &)),splash,SLOT(showMessage(const QString &,int, const QColor &)));
        QObject::connect (&window,SIGNAL(closeSplash()),splash,SLOT(close()));

        splash->show();
        splash->showMessage(QObject::trUtf8("Загрузка плагинов"),Qt::AlignLeft| Qt::AlignBottom, QColor::fromRgb(170,255,0));
        QTimer::singleShot(1000, splash, SLOT(clearMessage()));

        //QTimer::singleShot(5000, &window, SLOT(checkPluginReady()));
        splash->finish(&window);//.hide();
        app.processEvents();
        */
    }

}
