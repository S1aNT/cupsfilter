#ifndef AUTH_H
#define AUTH_H

#include <QObject>
#include <windows.h>
#include <stdio.h>
#include "auth_plugin.h"

class QString;

#define PLUGIN_API __declspec(dllimport)
typedef PLUGIN_API LPWSTR (*DLLGETCURRENTUSER)(void);
typedef PLUGIN_API LPWSTR (*DLLGETCURRENTSECLABEL)(void);
typedef PLUGIN_API bool (*DLLISAUTHUSER)(void);
typedef PLUGIN_API LPCTSTR (*ABOUTPLUGIN)(void);

class Auth : public QObject, Auth_plugin
{
    Q_OBJECT
    Q_INTERFACES(Auth_plugin)

public:
    Auth();
    void init (const QString &mandat_filename);
    void init ();
    QString getUserName(){return user_name;};
    QString getUserMandat(){return user_mandat;};

signals:
    void error(QString error_message);
    //void needShowAuthWindow(QString &userName);
    void get_User_name_mandat(QString &userName,QString &userMandat);

protected:
    QString user_name;
    QString user_mandat;
    QString plugin_path;
};
//! [0]

#endif
