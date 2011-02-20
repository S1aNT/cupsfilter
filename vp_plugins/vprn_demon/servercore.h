#ifndef SERVERCORE_H
#define SERVERCORE_H

#include <QObject>
#include "datamodule.h"
class ServerCore : public QObject
{
        Q_OBJECT
public:    
    explicit ServerCore(QObject *parent = 0);
    /**
      * @fn bool init();
      * @brief возвращает состо€ние сервера готов ли он к работе или нет
      */
    bool isReady(){ return state;}

private:
    bool state;
    DataModule *dm;

    QStringList t_FileList;
    /**
      * @fn bool loadPlugins();
      * @brief «агружает требуемые дл€ работы демона плагины
      */
    bool loadPlugins();

    /**
      * @fn bool loadSettings();
      * @brief „итает ini файл с настройками приложени€
      */
    bool loadSettings();

};

#endif // SERVERCORE_H
