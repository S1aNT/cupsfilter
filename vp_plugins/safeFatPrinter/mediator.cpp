#include "mediator.h"

Mediator::Mediator(QObject *parent) :
    QObject(parent)
{
}

bool Mediator::loadPlugin()
{
    QDir pluginsDir(qApp->applicationDirPath());
#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#endif
    pluginsDir.cd("plugins");
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        if (plugin) {
            net_plugin_Interface = qobject_cast<Inet_plugin *>(plugin);

            if (net_plugin_Interface){
                connect(plugin,SIGNAL(serverResponse(QString)),this,SLOT(showInfo(QString)));
                connect(plugin,SIGNAL(error(QString)),this,SLOT(showInfo(QString)));

                QString host=QObject::trUtf8("localhost");
                net_plugin_Interface->init(host,4242);
                return true;
            }
        }
    }

    return false;
}

//************************************** private function ***************************************
void Mediator::readGlobal()
{

}
//***********************************************************************************************
