#ifndef CONFIG_H
#define CONFIG_H
#include <QFile>
#include <QTextStream>
#include <QDateTime>


void myMessageOutput(QtMsgType type, const char *msg)
{
    QFile logFile("safe_printer.log");

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

#endif
