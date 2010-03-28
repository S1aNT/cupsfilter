
#include <QtSingleApplication>
#include <QtCore/QTextCodec>
#include <QtCore/QTranslator>
#include <QtCore/QLibraryInfo>
#include <QtGui/QMessageBox>

#include "config.h"
#include "mytypes.h"
#include "printmonitor.h"

using namespace VPrn;

int main(int argc, char *argv[])
{

    Q_INIT_RESOURCE(images);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);
    //qInstallMsgHandler(myMessageOutput);

    QtSingleApplication app(argc, argv, true);

    QString in_file;
#if defined (MY_DEBUG)
    in_file="test.ps";
#else
    QStringList aList=app.arguments();
    if (aList.size()< 2){
        QMessageBox::critical(0,QObject::trUtf8("Обратитесь к системному администратору"),
                              QObject::trUtf8("Ошибка запуска приложения, не достаточно параметров запуска"));
        return 1;
    }
    QString in_file=aList.at(1);
#endif
    /**
      @todo На базе сообщения организовать очередь печати !
      * После того как пользователь заканчивает работу с текущим документом
      * Происходит проверка очереди, нет ли там ожидающих документов,
      * если есть, то весь цикл запускается заново, но с шага 2 так как
      * логин, мандат уже есть, связь с локальным сервером установленна,
      * единственная проверка которая необходима, эта связь с мишиным демоном,
      * не отвалилась ли
      */

    if (app.sendMessage(in_file)
        || app.isRunning())
        return 0;        
    QString translatorFileName = QLatin1String("qt_");
    translatorFileName += QLocale::system().name();
    QTranslator *translator = new QTranslator(&app);
    if (translator->load(translatorFileName,
                         QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        app.installTranslator(translator);

    PrintMonitor monitor;    

    QObject::connect(&app, SIGNAL(messageReceived(const QString&)),
                     &monitor, SLOT(appendStartMsg(const QString&)));

    app.setActivationWindow(&monitor);    
    monitor.setFile4Work(in_file);
    app.processEvents();
    monitor.show();
    return app.exec();
}
