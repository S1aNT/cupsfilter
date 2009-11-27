#ifndef MEDIATOR_H
#define MEDIATOR_H

#include <QDebug>
#include <QObject>
#include <QPluginLoader>
#include <QDir>
#include <QUuid>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QColor>

#include "tech_global.h"
#include "inet_plugin.h"
#include "igs_plugin.h"
#include "auth_plugin.h"




typedef enum{
   netPluginLoaded,
   authPluginLoaded,
   gsPluginLoaded,
   connectedToDemon
}plugin_state_t;

class Mediator: public QObject
{
    Q_OBJECT

public:
    Mediator(QObject *parent = 0);
    void loadPlugin(const QString &app_dir);
    QString getUserName(){return user_name;};
    QString getUserMandat(){return user_mandat;};
    QStandardItemModel *document_model () const {return doc_model;}
    QStringListModel *stamp_model() const{ return stampModel;}
    QStringListModel *mandat_model() const{ return mandatModel;}
    void setUserMandat(QString mnd){user_mandat=mnd;};
    bool isConnected(){return connect_state;};
    void plugin_init();
    bool isValid(){return valid_status;};
    bool isAuth();
    bool isHaveMandatList();
    void setWorkMode(int mode){work_mode = mode;};
signals:
    void StateChanged(plugin_state_t state);
    void error (QString msg);
    void needShowAuthWindow(QString &userName);
    void pluginLoad(const QString &message, int alignment = Qt::AlignLeft| Qt::AlignBottom, const QColor & color = QColor::fromRgb(170,255,0));


public slots:
    void convert2pdf(const QString &input_fn);
private slots:
    void do_User_name_mandat(QString &userName,QString &userMandat);
    void parseServerResponse(QString &responce_msg);
    void getMeMandatList(QString &userName);
    void parseError(QString msg);
private:
    Inet_plugin *net_plugin;
    Igs_plugin *gs_plugin;
    Auth_plugin *auth_plugin;

    QString sid;

    QString user_name;
    QString user_mandat;
    int work_mode;

    QStandardItemModel *doc_model;
    QStringListModel *stampModel;
    QStringListModel *mandatModel;

    bool connect_state;

    bool valid_status;
    bool auth_status;

protected:
    /**
     * Read global settings from Application Dir
     */
    void readGlobal();
    /**
     *  This is spider soul :) connect any with any
     */
    void spiderSoul();
    void createModels();
    void test_cmd();
};

#endif // MEDIATOR_H
