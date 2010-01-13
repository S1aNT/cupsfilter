#ifndef MEDIATOR_H
#define MEDIATOR_H

#include <QDebug>
#include <QObject>
#include <QList>

#include "tech_global.h"
#include "inet_plugin.h"
#include "igs_plugin.h"
#include "itmpl_plugin.h"
#include "auth_plugin.h"

using namespace SafeVirtualPrinter;

QT_BEGIN_NAMESPACE
class QPoint;
class QStandardItemModel;
class QStringListModel;
class QDate;
class QFileInfo;

QT_END_NAMESPACE


class workField;
class tEditor;

class Mediator: public QObject
{
    Q_OBJECT
    Q_ENUMS(WorkStep)
    Q_ENUMS(WorkMode)
public:
    Mediator(QObject *parent = 0);
    //Геттеры
    //QStandardItemModel *getDocumentModel() const {return doc_model;}
    QStringListModel *getStampModel() const{ return stampModel;}
    QStringListModel *getMandatModel() const{ return mandatModel;}
    QStringListModel *getPrintersModel() const{ return printersModel;}
    QStringListModel *getPageSizeModel() const {return pageSizeModel;}

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
    void print_allowed(); // Сообщение от демона что печать разрешена
public slots:
    // Сохранить данные в лог
    void doSaveToLog(const QString & log_msg);
    // Выбор режима работы
    void setMode (int mode);
    // Вызов функции плагина преобразующего шаблон в набор сцен
    void do_convertTemplatesToScenes(const QString & templ_filename);
    // Сохранение выбранного пользователем принтера
    void setCurrentPrinter(const QString & printer);
    // авторизация текущего пользователя на предварительно выбранный принтер
    void do_needAuthUserToPrinter();
    void checkMBInBase(QString &mb_value, QString &copyNum_value,int w_mode);

private slots:
    void do_User_name_mandat(QString &userName,QString &userMandat);
    void parseServerResponse(QString &responce_msg);
    void getMeMandatList(QString &userName);
    void parserGSMessage(TaskState state);
    void setPageCountInDoc(int p_count);

    void do_showEditorWithData();
    void do_userSelectTemplates(const QString &templ);
    /**
     * @brief Печать текущего документа используя шаблон
     */
    void do_needPrintPage(const QString & t_file_name);

    // Шаблон преобразован в pdf получим имена страниц
    void mergeDocWithTemplate(QString &first,QString &second,
                              QString &third,QString &fourth);

    // Обработка ошибок (запись в лог перед передачей дальше)
    void doError(QString msg);

    // Запись текуще модели в файл
    void do_saveModelInFile();
private:
    QStringList log_console;

    /**
     * @brief различные файлы
     */
    QString model_data_file;
    QString log_file;

    /**
     *  @brief Указатели на плагины
     */
    Inet_plugin *net_plugin;
    Igs_plugin *gs_plugin;
    Auth_plugin *auth_plugin;
    Itmpl_plugin *tmpl_plugin;
    //-------------------------------------------------------------------------
    /**
     * @brief Указатели на диалоговые окна
     */
    workField *WorkDlg;

    //-------------------------------------------------------------------------

    bool connect_state;

    QString currentPrinter;
    QString sid;
    int pagesInDocCount; // число страниц в документе котрый отправили на печать
    QString currentTemplates_fname; // Текущий выбранный шаблон

    QString user_name;
    QString user_mandat;


    QMap <QString,QString> printer_device_list;
    // Набор моделей [для сборки :)]
    QStringListModel *stampModel;
    QStringListModel *mandatModel;
    QStringListModel *printersModel;
    QStringListModel *pageSizeModel;
    QStringListModel *localTModel;
    QStringListModel *globalTModel;

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
    QString tEditor_bin;
    QString ticket_fname;
    QString rcp_file;

    QString local_t_path;
    QString global_t_path;

    QList <QFileInfo> local_fileInfoList;
    QList <QFileInfo> global_fileInfoList;

    QString ftpTemplatesDir;

    //WorkMode work_mode;
    int currentMode;
protected:
    // Сердце всей программы соединяет сигналы от дилалогов со слотами
    void connector();

    QString getElemTagById(int elem_id);
    //QStringList getAllElem();
    //int getElemIdByName(const QString elem_name);

    /**
     * @brief Read global settings from Application Dir
     */
    void readGlobal(const QString &app_dir);

    void createModels();
    /**
     * @brief Заполним список содержимого локальных и глобальных каталогов
     */
    QStringListModel *getFolderModel(bool mode);

    /**
     * @brief Преобразует имя шаблона в полный путь к файлу
     */
    QString fileNameToFullPath(const QString &in_name);
    //void insertDocToModel();
    //void insertDocToModel(QString &item);
    //void fillMap(); // Заполним список значениями

    void getSecretLevelName(); //
    void getEnablePrinter();
    QPixmap formatPage(const QString &in_file,int pageNum);

};

#endif // MEDIATOR_H
