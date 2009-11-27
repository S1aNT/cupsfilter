#include <QtGui>

#include "auth.h"



Auth::Auth()
{

}

void Auth::init (const QString &mandat_filename)
{
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
        //ABOUTPLUGIN aboutplugin = (ABOUTPLUGIN) myLib.resolve("AboutPlugin");
        DLLGETCURRENTUSER pluginGetUSER= (DLLGETCURRENTUSER) myLib.resolve("GetCurrentUser");
        DLLGETCURRENTSECLABEL pluginGetCurrentSecLabel=(DLLGETCURRENTSECLABEL) myLib.resolve("GetCurrentSecLabel");
        //DLLISAUTHUSER pluginIsAuthUser= (DLLISAUTHUSER) myLib.resolve("IsAuthUser");
        //about_str =QString::fromUtf16((ushort*)aboutplugin());
        //qDebug() << "lib load"  << "About " <<about_str;
        // Читаем данные через интерфейс к LDAP
        //wprintf(TEXT("WUser:%s \n"),pluginGetUSER());
        //wprintf(TEXT("WSeclabel:%s \n"),pluginGetCurrentSecLabel());

        user_name =QString::fromUtf16((ushort*)pluginGetUSER());
        user_mandat =QString::fromUtf16((ushort*)pluginGetCurrentSecLabel());
        //qDebug() << "Name " << user_name <<" Mandat " << user_mandat;
        emit get_User_name_mandat(user_name,user_mandat);
    }else{
        // Нет у меня другого выхода спросим у системы
        // определим из реестра путь к Logon User Name

        QSettings log_settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer",QSettings::NativeFormat);
        user_name =log_settings.value("Logon User Name").toString(); // returns "Logon User Name"
        //user_name="usr1";
        emit needShowAuthWindow(user_name);
    }
}






Q_EXPORT_PLUGIN2(auth, Auth);
