#include <QDebug>
#include <QtGui/QApplication>
#include <QTextCodec>
#include <QFile>
#include <QByteArray>
#include <QMessageBox>
#include <QStringList>
#include <QSettings>
#include <QTextStream>
#include <QDateTime>
#include "startdlg.h"



void myMessageOutput(QtMsgType type, const char *msg)
{

     QSettings settings(QSettings::IniFormat, QSettings::UserScope,"Техносервъ","Защищенный принтер");
      settings.setIniCodec("UTF-8");
      settings.beginGroup( "LOG" );
#if defined(Q_OS_UNIX)
	QFile logFile(settings.value("printer_log","/var/log/safeprinter/safe_printer.log").toString());
#elif defined(Q_OS_WIN)
	QFile logFile(settings.value("printer_log","d:\\safeprinter\\safe_printer.log").toString());
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

int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);
    //QTextCodec::setCodecForLocale(codec);

   qInstallMsgHandler(myMessageOutput);

    QApplication app(argc, argv);
    QStringList aList=app.arguments();
    if (aList.size()!=4){
	QMessageBox::critical(0,QObject::trUtf8("Обратитесь к системному администратору"),
					    QObject::trUtf8("Ошибка запуска приложения, не достаточно параметров"));

	qDebug() << QString("Need more command line arguments.Current arguments %1\nExample safe_printer input.file doc_name").arg(aList.size());
	app.exit(1);
    }else {

	StartDlg w;

	QSettings mandat_set("d:\\safeprinter.ini",QSettings::IniFormat);
	mandat_set.beginGroup("RULES");
	QString mandat =mandat_set.value("mandat","UNDEF").toString();
	mandat_set.endGroup();

	w.setDocName(aList.at(2));
	w.convertToPDF(aList.at(1));
	w.setMandat (aList.at(3),mandat);
	w.show();
	return app.exec();
    }

}

