#include <QtSingleApplication>
#include <QtGui/QMessageBox>
#include <QtCore/QTextCodec>
#include <QtCore/QStringList>
#include <QFile>
#include <QTextStream>

#include "mainwindow.h"

void myMessageOutput(QtMsgType type, const char *msg)
{
    QString log = QString ("%1/print_monitor.log").arg( qApp->applicationDirPath() );
    QFile logFile(log);

    if (!logFile.open(QFile::Append| QFile::Text)){
        logFile.open(stderr, QIODevice::WriteOnly);
    }

     QTextStream out;
     out.setDevice(&logFile);
     out.setCodec("UTF-8");

     out << "\nDateTime: " << QDateTime::currentDateTime ().toString("dd.MM.yyyy hh:mm:ss");
     switch (type) {
     case QtDebugMsg:
        out << QObject::trUtf8("Debug: %1\n").arg(QString(msg)) <<"\n";
         break;
     case QtWarningMsg:
         out << QObject::trUtf8("Warning: %1\n").arg(QString(msg))<<"\n";
         break;
     case QtCriticalMsg:
         out << QObject::trUtf8("Critical: %1\n").arg(QString(msg))<<"\n";
         break;
     case QtFatalMsg:
         out << QObject::trUtf8("Fatal: %1\n").arg(QString(msg))<<"\n";
         abort();
     }
     logFile.close();
}

int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);

    qInstallMsgHandler(myMessageOutput);

    QtSingleApplication app(argc, argv, true);

    QStringList aList=app.arguments();
    if (aList.size()< 2){
        QMessageBox::critical(0,QObject::trUtf8("Обратитесь к системному администратору"),
                              QObject::trUtf8("Ошибка запуска приложения, не задан файл для печати"));
        return 1;
    }

    QString in_file=aList.at(1);

    if (app.sendMessage(QObject::trUtf8("Потребован повторный запуск приложения!"))
        || app.isRunning())
        return 0;

    QApplication::setQuitOnLastWindowClosed(true);
    qApp->setStyleSheet(" QPushButton:hover {background-color: black;} "
                        " QPushButton:hover {background-color: white;} "
                        " QPushButton { border-style: outset; border-width: 2px;"
                        "  border-radius: 10px; border-color: beige;"
                        "  font: bold 10px;  min-width: 10em; padding: 6px}"
                        );

    MainWindow client;
    client.init ( qApp->applicationDirPath(),in_file );
    client.show();
    app.setActivationWindow(&client);

    QObject::connect(&app, SIGNAL(messageReceived(const QString&)),
                     &client, SLOT(appendStartMsg(const QString&)));

    return app.exec();
}
