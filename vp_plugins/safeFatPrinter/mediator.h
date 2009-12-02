#ifndef MEDIATOR_H
#define MEDIATOR_H

#include <QDebug>
#include <QObject>
#include <QCoreApplication>
#include <QPluginLoader>
#include <QDir>
#include <QUuid>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QColor>
#include <QPrinterInfo>
#include <QMap>
#include <QMapIterator>
#include <QSettings>
#include <QDate>

#include "tech_global.h"
#include "inet_plugin.h"
#include "igs_plugin.h"
#include "auth_plugin.h"

using namespace SafeVirtualPrinter;




class Mediator: public QObject
{
    Q_OBJECT
    Q_ENUMS(WorkStep)

public:

    Mediator(QObject *parent = 0);

    void loadPlugin(const QString &app_dir);
    void plugin_init();
    // Геттеры
    QString getUserName(){return user_name;};
    QString getUserMandat(){return user_mandat;};
    QString getElemTagById(int elem_id);

    int getElemIdByName(const QString elem_name);
    int getPageCountInDoc(){return pagesInDocCount;};

    QStandardItemModel *document_model () const {return doc_model;}
    QStringListModel *stamp_model() const{ return stampModel;}
    QStringListModel *mandat_model() const{ return mandatModel;}
    QStringListModel *printers_model() const{ return printersModel;}
    // Сеттеры
    void setUserMandat(QString mnd);

signals:
    void error (QString msg);
    void needShowAuthWindow(QString &userName); // Требуется показать окно выбора мандата
    void pluginMessage(const QString &message);
    // Сигнал высылается при прохождении очередного шага загрузки
    void StateChanged(WorkStep);

public slots:
    void convert2pdf(const QString &input_fn);
    void authToPrinter(const QString & printer);

private slots:
    void do_User_name_mandat(QString &userName,QString &userMandat);
    void parseServerResponse(QString &responce_msg);
    void getMeMandatList(QString &userName);
    void parserGSMessage(TaskState state);
    void setPageCountInDoc(int p_count);
private:
    Inet_plugin *net_plugin;
    Igs_plugin *gs_plugin;
    Auth_plugin *auth_plugin;

    bool connect_state;

    QString sid;
    int pagesInDocCount; // число страниц в документе котрый отправили на печать

    QString user_name;
    QString user_mandat;

    QMap <QString, int> elemTag;
    // Набор моделей [для сборки :)]
    QStandardItemModel *doc_model;
    QStringListModel *stampModel;
    QStringListModel *mandatModel;
    QStringListModel *printersModel;

    // Блок переменных из ini файла
    QString serverHostName;
    int serverPort;
    int timeout_connect;
    int timeout_read;
    int timeout_write;

    QDate begin_date;
    QDate end_date;

    QString gsBin;
    QString pdftkBin;
    QString spoolDIR;
    QString ticket_fname;
    QString rcp_file;

    QString localTemplates;
    QString globalTemplates;
    QString ftpTemplatesDir;
protected:
    /**
     * Read global settings from Application Dir
     */
    void readGlobal(const QString &app_dir);

    void createModels();
    void fillMap(); // Заполним список значениями

    void getSecretLevelName(); //
    void getEnablePrinter();
};

#endif // MEDIATOR_H
