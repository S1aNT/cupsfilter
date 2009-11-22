#include "mediator.h"

Mediator::Mediator(QObject *parent) :
    QObject(parent)
{
    this->spiderSoul();
    QUuid uSID=QUuid::createUuid () ;  //generate SID
    sid=uSID.toString().replace("{","").replace("}","");

}

void Mediator::loadPlugin(const QString &app_dir)
{
    QDir pluginsDir(app_dir);
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
    foreach (QString fileName, pluginsDir.entryList(QDir::Files))
        {
            QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
            QObject *plugin = pluginLoader.instance();
            if (plugin) {
                connect(plugin, SIGNAL(error(QString &)), this, SIGNAL (error(QString &)));

                net_plugin_Interface = qobject_cast<Inet_plugin *> (plugin);

                if (net_plugin_Interface) {
                    //connect(plugin, SIGNAL(serverResponse(QString)), this, SLOT(showInfo(QString)));
                    QString host = QObject::trUtf8("localhost");
                    if (net_plugin_Interface->init(host, 4242)){
                       emit StateChanged(netPluginLoaded);
                    }
                }

                gs_plugin_Interface = qobject_cast<Igs_plugin *> (plugin);
                if (gs_plugin_Interface) {
                    // remove after debug

                    QString gs_bin="C:\\Program Files\\gs\\gs8.70\\bin\\gswin32c.exe";
                    QString pdftk_bin="c:\\tools\\pdftk.exe";
                    QString temp_folder="c:\\spool";

                    if (gs_plugin_Interface->init(gs_bin, pdftk_bin,temp_folder,sid)){
                       emit StateChanged(gsPluginLoaded);
                    }
                }
            }
        }
}


//************************************** private function ***************************************
void Mediator::readGlobal()
{

}
void Mediator::spiderSoul()
{

}
//***********************************************************************************************
//*************************************** public slots*******************************************
 void Mediator::convert2pdf(QString &input_fn)
 {

 }
