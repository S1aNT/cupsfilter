/**
  * The text in UTF-8 encoding!
  */

#include <QtCore/QTextCodec>
#include <QtCore/QCoreApplication>
#include "config.h"
#include "servercore.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(images);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);
    QCoreApplication a(argc, argv);
    installLog("GateKeeper",QObject::trUtf8("Техносерв А/Г"));
    ServerCore server;

    if ( !server.isReady() ){
        return -1;
    }
    return a.exec();
}
