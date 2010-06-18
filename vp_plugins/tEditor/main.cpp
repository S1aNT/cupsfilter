#include <QtCore/QDebug>
#include <QtGui/QApplication>
#include <QtCore/QTextCodec>
#include <QtCore/QStringList>
#include <QtCore/QFile>

#include "mainwindow.h"


void myMessageOutput(QtMsgType type, const char *msg)
{
    QString log = QString ("%1/tEditor.log").arg( qApp->applicationDirPath() );
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

    QApplication app(argc, argv);
    Q_INIT_RESOURCE(images);

    QStringList aList=app.arguments();
    /*
     * @brief По количеству аргументов определяем режим работы
     * - sizeOf (argv[]) = 1 (без аргументов) режим создания пустого шаблона
     * - sizeOf (argv[]) = 2 (имя_файла_шаблона) редактирование выбранного шаблона
    */
    QString file_name;

    MainWindow window;

    window.show();

    switch (aList.size()){
     case 1:
       window.createNewTemplate();
       break;
     case 2:
       file_name = aList.at(1);
       window.loadFromFile(file_name);
       break;
    }
    return app.exec();
}
