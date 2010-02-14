#include "plugin_worker.h"
#include <QDir>
#include <QPluginLoader>
#include <QFile>
#include <QMessageBox>
#include <QSqlTableModel>

pluginWorker::pluginWorker(QObject *parent)
{
    //qRegisterMetaType<pluginsError>("pluginsError");
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
                         SLOT(do_Error(pluginsError,QString))
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

 void pluginWorker::do_Error(pluginsError errCode,QString error_message)
 {
     //qDebug() << "Error code " << errCode << " eMessage " << error_message;
     emit error(errCode,error_message);
     QMessageBox msgBox;
      msgBox.setText(error_message);
      msgBox.exec();
 }

 bool pluginWorker::openTemplates(const QString &db_fileName)
 {
     t_plugin->openTemplates(db_fileName);
     return t_plugin->isDBOpened();
 }

 bool pluginWorker::createEmptyTemplate()
 {
    t_plugin->createEmptyTemplate();
    return t_plugin->isDBOpened();
 }

 void pluginWorker::doCreateEmptyTemplate(const QString &fileName)
 {

 }

TemplateInfoEditModel *pluginWorker::getInfoModel()
{
     return t_plugin->getInfoModel();
}

/*
QSqlTableModel *pluginWorker::getInfoModel2()
{
     return t_plugin->getInfoModel2();
}
*/
QSqlQueryModel *pluginWorker::getPSizeModel()
{
     return t_plugin->getPSizeModel();
}
