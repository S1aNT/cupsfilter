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
    Q_ENUMS(WorkMode)
public:
    Mediator(QObject *parent = 0);

    void loadPlugin(const QString &app_dir);
    void plugin_init();
    void convert2pdf(QString &in_file);
    // Геттеры
    QString getSpoolDir(){return spoolDIR;};
    QString getUserName(){return user_name;};
    QString getUserMandat(){return user_mandat;};

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
    void mbNumberNotExist();
    void needShowPreviewPage(const QPixmap &preview_page); // Требуется показать страницу документа

public slots:
    // Сохранение выбранного пользователем принтера
    void setCurrentPrinter(const QString & printer);
    // авторизация текущего пользователя на предварительно выбранный принтер
    void do_needAuthUserToPrinter();
    void checkMBInBase(QString &mb_value, QString &copyNum_value,WorkMode w_mode);
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

    QString currentPrinter;
    QString sid;
    int pagesInDocCount; // число страниц в документе котрый отправили на печать
    QString currentTemplates_fname; // Текущий выбранный шаблон

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

    WorkMode work_mode;
protected:
    //Геттеры
    QString getElemTagById(int elem_id);
    QStringList getAllElem();
    int getElemIdByName(const QString elem_name);

    /**
     * Read global settings from Application Dir
     */
    void readGlobal(const QString &app_dir);
    void createModels();
    void insertDocToModel();
    void insertDocToModel(QString &item);
    void fillMap(); // Заполним список значениями

    void getSecretLevelName(); //
    void getEnablePrinter();
    QPixmap formatPage(const QString &in_file,int pageNum);

};

#endif // MEDIATOR_H
