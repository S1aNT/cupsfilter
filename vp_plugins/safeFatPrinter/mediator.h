#ifndef MEDIATOR_H
#define MEDIATOR_H

#include <QObject>
#include <QPluginLoader>
#include <QDir>
#include <QUuid>
#include "inet_plugin.h"
#include "igs_plugin.h"

class Mediator: public QObject
{
    Q_OBJECT
    Q_ENUMS(PluginState)
public:
    Mediator(QObject *parent = 0);
    void loadPlugin(const QString &app_dir);
    enum PluginState { netPluginLoaded, authPluginLoaded, gsPluginLoaded };

signals:
    void StateChanged(PluginState state);
    void error (const QString &msg);

public slots:
    void convert2pdf(QString &input_fn);
private:
    Inet_plugin *net_plugin_Interface;
    Igs_plugin *gs_plugin_Interface;

    QString sid;

    /**
     * Read global settings from Application Dir
     */
    void readGlobal();
    /**
     *  This is spider soul :) connect any with any
     */
    void spiderSoul();
};

#endif // MEDIATOR_H
