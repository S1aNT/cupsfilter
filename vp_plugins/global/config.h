#ifndef CONFIG_H
#define CONFIG_H
#include <QString>
#include <QTextCodec>
#include <QTextStream>
#include <QFile>
//#include <iostream>
//using std::cout;
//using std::endl;
  
// Типы сообщений
static const char* msgType[] =
{
    "(II) ", // Info
    "(WW) ", // Warning
    "(EE) ", // Error
    "(FF) " // Fatal error
};

// Вывод логов в файл
void customMessageHandler(QtMsgType type, const char* msg);
// Создание файла для логов
void installLog();

// Информация об ОС
QString getOSInfo();


// Вывод логов в файл
void customMessageHandler(QtMsgType type, const char* msg)
{
    QFile logFile(QString ("%1.log").arg( qApp->arguments().at(0) ) );

    if (!logFile.open(QFile::WriteOnly |
                      QIODevice::Append |
                      QIODevice::Unbuffered)){
        logFile.open(stderr, QIODevice::WriteOnly);
    }

    QTextStream logStream;
    logStream.setDevice(&logFile);

#ifdef Q_WS_WIN
    logStream.setCodec("Windows-1251");
#else // Под остальными ОС - utf8
    logStream.setCodec("utf-8");
#endif
    logStream << msgType[type] << msg << endl;
    //std::cout << msgType[type] << msg << std::endl;

    logFile.close();    
}

// Создание файла для логов
void installLog(const QString &app_name,const QString &app_company )
{
    QFile logFile(QString ("%1.log").arg( qApp->arguments().at(0) ) );

    if (!logFile.open(QFile::WriteOnly |
                      QIODevice::Append |
                      QIODevice::Unbuffered)){
        logFile.open(stderr, QIODevice::WriteOnly);
    }

    QTextStream logStream;
    logStream.setDevice(&logFile);

#ifdef Q_WS_WIN
    //logStream.setCodec("Windows-1251");
    logStream.setCodec("utf-8");
#else // Под остальными ОС - utf8
    logStream.setCodec("utf-8");
#endif

    // Запись заголовка с информацией о запуске
    QString TextDescription = QObject::tr(
            "Appplication:%1  from %2\n"
            "Built on " __DATE__ " at " __TIME__ ".\n"
            "Based on Qt %3.\n")
            .arg(app_name,app_company,QLatin1String(QT_VERSION_STR)
                 );

    logStream << endl << TextDescription << endl;
    logStream << getOSInfo() << endl;
    logStream << QString("Markers:--------------\n(II) informational\n(WW) warning") << endl;
    logStream << QString("(EE) error\n(FF) fatal error.\n") << endl;    
    logStream << QString("Runned at %1.")
            .arg(QDateTime::currentDateTime().toString()) << endl << endl;
    logFile.close();
    qInstallMsgHandler(customMessageHandler);
    qDebug("Success opening log file");
}


// Информация об ОС
QString getOSInfo()
{
    QString infoStr("Current Operating System: %1");
#ifdef Q_OS_WIN
    switch(QSysInfo::windowsVersion())
    {
    case QSysInfo::WV_NT: return infoStr.arg("Windows NT");
    case QSysInfo::WV_2000: return infoStr.arg("Windows 2000");
    case QSysInfo::WV_XP: return infoStr.arg("Windows XP");
    case QSysInfo::WV_2003: return infoStr.arg("Windows 2003");
    case QSysInfo::WV_VISTA: return infoStr.arg("Windows Vista");    
    default: return infoStr.arg("Windows");
    }
#endif // Q_OS_WIN

#ifdef Q_OS_MAC
    switch(QSysInfo::MacintoshVersion())
    {
    case QSysInfo::MV_CHEETAH: return infoStr.arg("Mac OS X 1.0 Cheetah");
    case QSysInfo::MV_PUMA: return infoStr.arg("Mac OS X 1.1 Puma");
    case QSysInfo::MV_JAGUAR: return infoStr.arg("Mac OS X 1.2 Jaguar");
    case QSysInfo::MV_PANTHER: return infoStr.arg("Mac OS X 1.3 Panther");
    case QSysInfo::MV_TIGER: return infoStr.arg("Mac OS X 1.4 Tiger");
    case QSysInfo::MV_LEOPARD: return infoStr.arg("Mac OS X 1.5 Leopard");
    case QSysInfo::MV_SNOWLEOPARD: return infoStr.arg("Mac OS X 1.6 Snow Leopard");
    default: return infoStr.arg("Mac OS X");
    }
#endif // Q_OS_MAC

#ifdef Q_OS_LINUX
#ifdef LINUX_OS_VERSION
    return infoStr.arg(LINUX_OS_VERSION);
#else
    return infoStr.arg("Linux");
#endif // LINUX_OS_VERSION
#endif // Q_OS_LINUX
}

#endif
