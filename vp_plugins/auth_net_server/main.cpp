/**
 * @mainpage  Данный сервер служи для 2 целей авторизация пользователя и работа
 *  с сетью. Он загружает и использует только следующие плагин:
 *  auth_plugin  Предназначен для авторизации пользователя
 *  net_plugin   Передает демону команды и выдает ответ от демона
*/

#include <QtSingleApplication>
#include <QtGui/QSystemTrayIcon>
#include <QtGui/QMessageBox>
#include <QtGui/QLabel>
#include <QtCore/QTextCodec>



#include "server.h"
#include "config.h"

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
        QMessageBox::critical(0, QObject::trUtf8("Auth_net_Server"),
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
