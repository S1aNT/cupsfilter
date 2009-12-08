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
#include "itmpl_plugin.h"
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
    // Если в ini файле не установлен путь к логу то его и не сохраняем на диск
    // но вести все равно ведем, как же без него.
    bool isLogger(){return log_file.isEmpty();};
    // Геттеры
    //Вычислим координаты для виджета который хоти разместить в центре
    QPoint  getDeskTopCenter(int width,int height);

    QString getSpoolDir(){return spoolDIR;};
    QString getSeansSid(){return sid;};
    QString getUserName(){return user_name;};
    QString getUserMandat(){return user_mandat;};

    int getPageCountInDoc(){return pagesInDocCount;};

    QStandardItemModel *getDocumentModel() const {return doc_model;}
    QStringListModel *getStampModel() const{ return stampModel;}
    QStringListModel *getMandatModel() const{ return mandatModel;}
    QStringListModel *getPrintersModel() const{ return printersModel;}
    QStringListModel *getPageSizeModel() const {return pageSizeModel;}
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
    void allTemplatesPagesParsed(QGraphicsScene *scene_1,QGraphicsScene *scene_2,
                                 QGraphicsScene *scene_3,QGraphicsScene *scene_4); // успешый разбор всех страниц шаблона и запись их в сцены
    void needUpdatePage(int page); // Требование плагину обновить страницу шаблона номер

public slots:
    // Сохранить данные в лог
    void doSaveToLog(const QString & log_msg);

    // Вызов функции плагина преобразующего шаблон в набор сцен
    void do_convertTemplatesToScenes(const QString & templ_filename);
    // Сохранение выбранного пользователем принтера
    void setCurrentPrinter(const QString & printer);
    // авторизация текущего пользователя на предварительно выбранный принтер
    void do_needAuthUserToPrinter();
    void checkMBInBase(QString &mb_value, QString &copyNum_value,WorkMode w_mode);
    // Требование к плагину создать пустой шаблон
    void do_needCreateEmptyTemplates(const QString & file_name,
                                  const QString & t_name,
                                  const QString & t_author,
                                  const QString & t_desc,
                                  const QString & p_size,
                                  bool p_orient,
                                  const QString & c_date,
                                  qreal m_top,
                                  qreal m_bottom,
                                  qreal m_right,
                                  qreal m_left);
private slots:
    void do_User_name_mandat(QString &userName,QString &userMandat);
    void parseServerResponse(QString &responce_msg);
    void getMeMandatList(QString &userName);
    void parserGSMessage(TaskState state);
    void setPageCountInDoc(int p_count);
private:
    QStringList log_console;
    QString log_file;
    Inet_plugin *net_plugin;
    Igs_plugin *gs_plugin;
    Auth_plugin *auth_plugin;
    Itmpl_plugin *tmpl_plugin;

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
    QStringListModel *pageSizeModel;

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
