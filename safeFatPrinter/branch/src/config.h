#ifndef CONFIG_H
#define CONFIG_H
#include <QFile>
#include <QSettings>
#include <QTextStream>
#include <QDateTime>



void myMessageOutput(QtMsgType type, const char *msg)
{
      QSettings settings(QSettings::IniFormat, QSettings::UserScope,"Техносервъ","Защищенный принтер");
      settings.setIniCodec("UTF-8");
      settings.beginGroup( "LOG" );
#if defined(Q_OS_UNIX)
	QFile logFile(settings.value("printer_log","/var/log/safeprinter/safe_printer.log").toString());
#elif defined(Q_OS_WIN)
	QFile logFile(settings.value("printer_log","c:\\spool\\safe_printer.log").toString());
#endif
      settings.endGroup();
    if (!logFile.open(QFile::Append| QFile::Text)){
	logFile.open(stderr, QIODevice::WriteOnly);
    }

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

#endif
