#ifndef AUTH_H
#define AUTH_H

#include <QObject>
#if defined(Q_OS_WIN)
#include <windows.h>
#endif

#include <stdio.h>

#include "auth_plugin.h"
#include "mytypes.h"

using namespace VPrn;

class QString;

#if defined(Q_OS_WIN)
#define PLUGIN_API __declspec(dllimport)
typedef PLUGIN_API LPWSTR (*DLLGETCURRENTUSER)(void);
typedef PLUGIN_API LPWSTR (*DLLGETCURRENTSECLABEL)(void);
typedef PLUGIN_API bool (*DLLISAUTHUSER)(void);
typedef PLUGIN_API LPCTSTR (*ABOUTPLUGIN)(void);
#endif

class Auth : public QObject, Auth_plugin
{
    Q_OBJECT
    Q_INTERFACES(Auth_plugin)

public:
    Auth(){}
    void init (const QString &mandat_filename);
    void init ();

signals:
    void error(VPrn::AppErrorType errCode,QString error_message);

    void get_User_name_mandat(QString &userName,QString &userMandat);
private:
    QString user_name;
    QString user_mandat;
    QString plugin_path;

    // Получить имя пользователя из реестра
    QString ask4System();

};
//! [0]

#endif
