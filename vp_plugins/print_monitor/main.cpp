
#include "mytypes.h"
#include "printmonitor.h"

#include <QtSingleApplication>

#include <QtCore/QTextCodec>
#include <QtCore/QTranslator>
#include <QtCore/QLibraryInfo>

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QDateTime>

#include <QtGui/QMessageBox>


using namespace VPrn;



void myMessageOutput(QtMsgType type, const char *msg)
{
    QString log = QString ("%1/%2_print_monitor.log").arg(qApp->applicationDirPath(),QDateTime::currentDateTime ().toString("dd.MM.yyyy hh:mm:ss"));
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

    QStringList aList=app.arguments();
    if (aList.size()< 1){
        QMessageBox::critical(0,QObject::trUtf8("Обратитесь к системному администратору"),
                              QObject::trUtf8("Ошибка запуска приложения, не достаточно параметров запуска"));
        return 1;
    }
    QString in_file=aList.at(1);

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
