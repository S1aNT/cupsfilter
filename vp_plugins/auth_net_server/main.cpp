/**
 * @mainpage  Данный сервер служи для 2 целей авторизация пользователя и работа
 *  с сетью. Он загружает и использует только следующие плагин:
 *  auth_plugin  Предназначен для авторизации пользователя 
 *  net_plugin   Передает демону команды и выдает ответ от демона
*/

#include <QApplication>
#include <QtCore>

#include <stdlib.h>

#include "server.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Server server;
    server.show();
    return server.exec();
}
