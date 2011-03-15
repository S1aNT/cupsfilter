//#include <QtGui>
#include <QFile>
#include <QDebug>
#include <QLibrary>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QRegExp>

#include "auth.h"

using namespace VPrn;

#define MY_DEBUG_OFF

void Auth::init (const QString &mandat_filename)
{
    QFile ticket;
    QString e_msg;
    // Проверим факт существования по указанному пути
    if (QFile::exists(mandat_filename)) {
        QFile ticket (mandat_filename);
        QString text;
        if (ticket.open(QIODevice::ReadOnly)) {
            QTextStream in_stream(&ticket);
            in_stream >> text;
        }
        ticket.close();
        // Разберем по кирпичику
        QStringList list = text.split(",");
        QString str;
        QString val;
        QString key;
        for (int i=0; i< list.size();i++){
            QRegExp rx("^(.+)=(.+)$");
            if(rx.indexIn(list.at(i)) != -1){
                val =rx.cap(1);
                key = rx.cap(2);
                if (val.compare("uid")==0){
                    user_name = key;
                }
                if (val.compare("mid")==0){
                    user_mandat = key;
                }
            }
        }
        if (user_name.isEmpty()){
            user_name = ask4System();
        }
        // Проверка на русские буквы
        QByteArray buf = user_name.toLatin1();
        if (!buf.isEmpty()){
            emit get_User_name_mandat(user_name,user_mandat);
        }else{

            emit error(VPrn::AuthCommonError,
	        QObject::trUtf8("Ошибка преобразования имени пользователя %1 в Latin1.%2\n")
                       .arg(user_name)
			.arg(QString(Q_FUNC_INFO))
);
        }

    }else{
        emit error(VPrn::FileIOError,
		QObject::trUtf8("Error: Файл мандата [%1] не найден!\n%2")
			.arg(mandat_filename)
                        .arg(QString(Q_FUNC_INFO)));
    }

}

QString Auth::ask4System()
{
    QString logon_user_name;

#if defined(Q_OS_UNIX)
    logon_user_name = QString(getenv("USERNAME"));
#elif defined(Q_OS_WIN)
    // Нет у меня другого выхода спросим у системы
    // определим из реестра путь к Logon User Name
    QSettings log_settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer",QSettings::NativeFormat);
    logon_user_name = log_settings.value("Logon User Name").toString();
#endif

#if defined (MY_DEBUG)
    //logon_user_name ="usr1";
#endif
    return logon_user_name;
}

void  Auth::init ()
{
#if defined(Q_OS_WIN)

    // проверка существования агента безопастности
    // определим из реестра путь к библиотеке ldap_plus.dll
    QSettings settings("HKEY_LOCAL_MACHINE\\SOFTWARE\\pGina",QSettings::NativeFormat);
    plugin_path =settings.value("pathPlugin").toString(); // returns "full path"
    // Загрузка библиотеки
    QLibrary myLib;
    myLib.setFileName(plugin_path);
    myLib.load();
    //qDebug()<< "Library " << myLib.fileName();
    if (myLib.isLoaded()){
        DLLGETCURRENTUSER pluginGetUSER= (DLLGETCURRENTUSER) myLib.resolve("GetCurrentUser");
        DLLGETCURRENTSECLABEL pluginGetCurrentSecLabel=(DLLGETCURRENTSECLABEL) myLib.resolve("GetCurrentSecLabel");
        // Читаем данные через интерфейс к LDAP
        user_name =QString::fromUtf16((ushort*)pluginGetUSER());
        user_mandat =QString::fromUtf16((ushort*)pluginGetCurrentSecLabel());
    }
#endif

    if (user_name.isEmpty()){
        user_name = ask4System();
    }
    // Проверка на русские буквы
    QByteArray buf = user_name.toLatin1();
    if (!buf.isEmpty()){
        emit get_User_name_mandat(user_name,user_mandat);
    }else{
        emit error(VPrn::AuthCommonError,
		QObject::trUtf8("Ошибка преобразования имени пользователя %1 в Latin1.\n%2")
                   .arg(user_name).arg(QString(Q_FUNC_INFO)));
    }
}

Q_EXPORT_PLUGIN2(auth, Auth);
