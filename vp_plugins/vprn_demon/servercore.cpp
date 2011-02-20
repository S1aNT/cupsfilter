#include "servercore.h"

#include <QtCore/QStringList>

ServerCore::ServerCore(QObject *parent)
        : QObject(parent)
        , state (false)
{
    dm = new DataModule(this);
    state = loadSettings() && loadPlugins();

}


//---------------------------- PRIVATE -----------------------------------------
bool ServerCore::loadPlugins()
{
    return true;
}

bool ServerCore::loadSettings()
{
    return true;
}

