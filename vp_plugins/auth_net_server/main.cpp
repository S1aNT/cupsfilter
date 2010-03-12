/**
 * @mainpage  Данный сервер служи для 2 целей авторизация пользователя и работа
 *  с сетью. Он загружает и использует только следующие плагин:
 *  auth_plugin  Предназначен для авторизации пользователя 
 *  net_plugin   Передает демону команды и выдает ответ от демона
*/

#include <QtGui/QSystemTrayIcon>
#include <QtGui/QMessageBox>
#include <QtCore/QTextCodec>

#include "server.h"

int main(int argc, char *argv[])
{

    Q_INIT_RESOURCE(images);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);

    QApplication app(argc, argv);


    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("Auth_net_Server"),
                              QObject::tr("Не могу определить тип system tray "
                                          " для вашей системы."));
        return 1;
    }

    QApplication::setQuitOnLastWindowClosed(false);
    Server server;
        server.show();
    return app.exec();
}
