#include <QDebug>
#include <QtGui/QApplication>
#include <QTextCodec>
#include <QFile>
#include <QByteArray>
#include <QMessageBox>
#include <QStringList>

#include <QTextStream>
#include <QDateTime>

#include "startdlg.h"


void myMessageOutput(QtMsgType type, const char *msg)
{
#if defined(Q_OS_UNIX)
    QFile logFile("/var/log/safeprinter/safe_printer.log");
#elif defined(Q_OS_WIN)
    QFile logFile("d:\\safe_printer.log");
#endif


    if (!logFile.open(QFile::Append| QFile::Text)){
	logFile.open(stderr, QIODevice::WriteOnly);
    }

    //logFile.open(stderr, QIODevice::WriteOnly);
    QTextStream out;
    out.setDevice(&logFile);
    out.setCodec("UTF-8");

    out << "\nDateTime: " << QDateTime::currentDateTime ().toString("dd.MM.yyyy hh:mm:ss") <<"\n";
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
    //QTextCodec::setCodecForLocale(codec);

   qInstallMsgHandler(myMessageOutput);

    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName( "Защищенный принтер" );
    QCoreApplication::setApplicationName( "safe_printer" );

    QStringList aList=app.arguments();
    if (aList.size()!=2){
	QMessageBox::critical(0,QObject::trUtf8("Обратитесь к системному администратору"),
					    QObject::trUtf8("Ошибка запуска приложения, не достаточно параметров"));
	qDebug() << "Need more command line arguments. example safe_printer input.file doc_name";
    }
    StartDlg w();
    w.setInputFile(aList.at(1));
    w.setDocTitle(aList.at(2));
    w.show();
    return app.exec();
}
