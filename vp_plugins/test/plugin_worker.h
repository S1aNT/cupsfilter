#ifndef PLUGIN_WORKER__H
#define PLUGIN_WORKER__H
#include <QMetaType>
#include <QtCore>
#include <QDebug>
#include "itmpl_sql_plugin.h"


using namespace VirtualPrninterPlugins;

class pluginWorker :public QObject
{
    Q_OBJECT

public:
    pluginWorker(QObject *parent=0);
    bool loadPlugin();
    bool openTemplates(const QString &db_fileName);
    bool createEmptyTemplate(const QString &db_fileName);
    void closeTemplates();

signals:
    void error(pluginsError errCode,QString error_message);
public slots:
    void do_Error(pluginsError errCode,QString error_message);
private:
    Itmpl_sql_plugin *t_plugin;
    pluginsError errCode;
    QString errMsg;
protected:
    void setPluginError (pluginsError e_code){errCode = e_code;};
    void setErrorMessage (const QString &e_msg){errMsg = e_msg;};
};


Q_DECLARE_METATYPE(VirtualPrninterPlugins::pluginsError)

#endif //PLUGIN_WORKER_
