
#include <QtSingleApplication>
#include <QtCore/QTextCodec>
#include <QtCore/QTranslator>
#include <QtCore/QLibraryInfo>
#include <QtGui/QMessageBox>

#include "config.h"
#include "mytypes.h"
#include "printmonitor.h"

using namespace VPrn;

#define MY_DEBUG

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

#if defined (Q_OS_WIN32)
    in_file="d:/opt/vprn/test.ps";
#elif defined (Q_OS_UNIX)
    in_file="/opt/vprn/test.ps";
#endif

#else
    QStringList aList=app.arguments();
    if (aList.size()< 1){
        QMessageBox::critical(0,QObject::trUtf8("Обратитесь к системному администратору"),
                              QObject::trUtf8("Ошибка запуска приложения, не достаточно параметров запуска"));
        return 1;
    }
    QString in_file=aList.at(1);
#endif    
    if (app.sendMessage(QObject::trUtf8("Попытка запуска второй копии приложения!"))
        || app.isRunning())
        return 0;
    QString translatorFileName = QLatin1String("qt_");
    translatorFileName += QLocale::system().name();
    QTranslator *translator = new QTranslator(&app);
    if (translator->load(translatorFileName,
                         QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        app.installTranslator(translator);

    PrintMonitor monitor(0,in_file);

    QObject::connect(&app, SIGNAL(messageReceived(const QString&)),
                     &monitor, SLOT(appendStartMsg(const QString&)));

    app.setActivationWindow(&monitor);
    monitor.show();
    return app.exec();
}
