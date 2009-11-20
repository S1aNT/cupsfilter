#ifndef MEDIATOR_H
#define MEDIATOR_H

#include <QObject>
#include <QPluginLoader>
#include <QDir>
#include "inet_plugin.h"

class Mediator : public QObject
{
Q_OBJECT
    typedef enum{
        netPluginLoaded,
        authPluginLoaded,
        gsPluginLoaded,
        connected,
        converted,
        merged
    }State;
public:
    explicit Mediator(QObject *parent = 0);
    bool loadPlugin();

signals:
    void StateChanged(State state);
public slots:

private:
    Inet_plugin *net_plugin_Interface;
    /**
     * Read global settings from Application Dir
     */
    void readGlobal();


};

#endif // MEDIATOR_H
