//#include <QtGui>
#include <QFile>
#include <QDebug>
#include <QLibrary>
#include <QSettings>
#include <QString>

#include "auth.h"
#include "tech_global.h"

using namespace SafeVirtualPrinter;


Auth::Auth()
{

}

void Auth::init (const QString &mandat_filename)
{
    //TODO написать чтение из файла мандата
}

void  Auth::init ()
{// проверка существования агента безопастности
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
        /*
        ABOUTPLUGIN aboutplugin = (ABOUTPLUGIN) myLib.resolve("AboutPlugin");
        DLLISAUTHUSER pluginIsAuthUser= (DLLISAUTHUSER) myLib.resolve("IsAuthUser");
        about_str =QString::fromUtf16((ushort*)aboutplugin());
        qDebug() << "lib load"  << "About " <<about_str;

        wprintf(TEXT("WUser:%s \n"),pluginGetUSER());
        wprintf(TEXT("WSeclabel:%s \n"),pluginGetCurrentSecLabel());
        */
        // Читаем данные через интерфейс к LDAP
        user_name =QString::fromUtf16((ushort*)pluginGetUSER());
        user_mandat =QString::fromUtf16((ushort*)pluginGetCurrentSecLabel());
        //qDebug() << "Name " << user_name <<" Mandat " << user_mandat;
        if (!user_name.isEmpty() && !user_mandat.isEmpty()){
            emit get_User_name_mandat(user_name,user_mandat);
        }
    }else{
        // Нет у меня другого выхода спросим у системы
        // определим из реестра путь к Logon User Name

        QSettings log_settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer",QSettings::NativeFormat);
        user_name =log_settings.value("Logon User Name").toString(); // returns "Logon User Name"
        qDebug() <<Q_FUNC_INFO <<log_settings.status();
        //user_name="usr1";
        //emit needShowAuthWindow(user_name);
        user_mandat="";
//        user_name = QString("u707on1");
        user_name = QString("usr1");
        emit get_User_name_mandat(user_name,user_mandat);
    }
}

Q_EXPORT_PLUGIN2(auth, Auth);
