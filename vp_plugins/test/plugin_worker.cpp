#include "plugin_worker.h"
#include <QDir>
#include <QPluginLoader>
#include <QFile>

using namespace VirtualPrninterPlugins;

pluginWorker::pluginWorker(QObject *parent)
{
    qRegisterMetaType<pluginsError>("pluginsError");
}

bool pluginWorker::loadPlugin()
{
    QDir pluginsDir(qApp->applicationDirPath());

#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" ||
        pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#endif
    pluginsDir.cd("plugins");
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)){
        QPluginLoader pluginMessageer(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginMessageer.instance();
        if (plugin) {
            t_plugin  = qobject_cast<Itmpl_sql_plugin *> (plugin);
            if (t_plugin){
                connect (plugin,
                         SIGNAL(error(pluginsError,QString)),
                         this,
                         SIGNAL(error(pluginsError,QString))
                         );
                return true;
            }
        }
    }
    return false;
}

 void pluginWorker::closeTemplates()
 {

 }

 bool pluginWorker::openTemplates(const QString &db_fileName)
 {
     return t_plugin->openTemplates(db_fileName);
 }

 bool pluginWorker::createEmptyTemplate(const QString &db_fileName)
 {
     return t_plugin->createEmptyTemplate(db_fileName);
 }
