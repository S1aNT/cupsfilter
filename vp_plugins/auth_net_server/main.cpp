/**
 * @mainpage  Данный сервер служит для 2 целей авторизация пользователя и работа
 *  с сетью. Он загружает и использует только следующие плагин:
 *  auth_plugin  Предназначен для авторизации пользователя
 *  net_plugin   Передает демону команды и выдает ответ от демона
*/

#include <QtSingleApplication>
#include <QtGui/QSystemTrayIcon>
#include <QtGui/QMessageBox>
#include <QtGui/QLabel>
#include <QtCore/QTextCodec>
#include <QtCore/QFile>


#include "server.h"

void myMessageOutput(QtMsgType type, const char *msg)
{
    QString log = QString ("%1/auth_net_server.log").arg( qApp->applicationDirPath() );
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

    Q_INIT_RESOURCE(images);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);
    qInstallMsgHandler(myMessageOutput);

    QtSingleApplication app(argc, argv, true);



    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::trUtf8("GateKeeper"),
                              QObject::trUtf8("Не могу определить тип system tray "
                                          " для вашей системы."));
        return 1;
    }

    if (app.sendMessage(QObject::trUtf8("Потребован повторный запуск приложения!"))
        || app.isRunning())
        return 0;

    QApplication::setQuitOnLastWindowClosed(false);

    Server server;
    server.hide();
    app.setActivationWindow(&server);

    QObject::connect(&app, SIGNAL(messageReceived(const QString&)),
                     &server, SLOT(appendStartMsg(const QString&)));

    return app.exec();
}
