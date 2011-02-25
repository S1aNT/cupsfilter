#include <qapplication.h>
#include <qglobal.h>

#include "common.h"
#include "sptsdserver.h"

void sptsdMsgHandler( QtMsgType type, const char *msg );
QString readSettings(QString key);

			    															
int main(int argc, char *argv[])
{
    bool useGUI = false;
    Q_UINT16 port = 4243;
    int conn = 100;
    
    
//    qInstallMsgHandler(sptsdMsgHandler);
    QApplication qApp(argc, argv, useGUI);
    qDebug ("\n");
    qDebug ("Starting daemon ...");

    sptsdServer *server = new sptsdServer(port, conn);
    
    if(!server->ok())
	qFatal("Could not listen on port " + QString("%1").arg(port));

    qDebug ("Ready to accept connections on port " + QString("%1").arg(port));

    return qApp.exec();
}



void sptsdMsgHandler( QtMsgType type, const char *msg )
{

 QDir	logDir(readSettings("/sptsd/sptsd_log_dir"));
 QFile	logFile(logDir.path() + "/" + readSettings("/sptsd/sptsd_log_file"));

 fprintf(stdout, "Opening log file: " + logFile.name() + "\n");
 fprintf(stdout, "Writing: %s \n", msg);
 
 if (!logDir.exists(logDir.path())) {
    fprintf(stdout,  "Creating new log directory " + logDir.path() + "\n");
    if (!logDir.mkdir(logDir.path(), true)) {
        fprintf(stderr,  "Can't create log directory" + logDir.path() + "\n" + "Try to create it manually.\n");
	qApp->exit();
    }
 }

 if (!logFile.open(IO_WriteOnly | IO_Append)) 
    fprintf(stderr, "Can't write to log file: " + logFile.name());
 else
 {
    QTextStream log(&logFile);
    switch ( type ) {
        case QtDebugMsg:
    	    log << "Debug: " << QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz   ") << msg << endl;
    	    break;
	case QtWarningMsg:
    	    log << "Warning: " << QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz   ") << msg << endl;
    	    break;
	case QtFatalMsg:
    	    log << "Fatal: " << QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz   ") << msg << endl;
    	    abort();                  
   }  
 }
 logFile.flush();
 logFile.close();
}

QString readSettings (QString key)
{
//    fprintf(stdout, "Reading '" + key + "' from /usr/lib/qt3/etc/settings/sptsdrc \n");

    QSettings config(QSettings::Ini);
    config.insertSearchPath(QSettings::Unix, "/usr/lib/qt3/etc/settings");
    QString value = config.readEntry("sptsd" + key);
    return value;   
}


