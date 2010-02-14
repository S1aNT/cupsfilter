#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QDebug>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QAction;
class QListWidget;
class QMenu;
class QTextEdit;
class QStringListModel;
class QStandardItemModel;
QT_END_NAMESPACE

class View;
class cmdFrame;

//#include "itmpl_plugin.h"
//#include "templ_info.h"
#include "itmpl_sql_plugin.h"
#include "auth_plugin.h"
#include "addtemplate.h"
#include "mytypes.h"

using namespace VPrn;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
    //Директор по такелажу :) - т.е грузчик !(Грузит шаблон из файла)
    void loadFromFile(const QString &file_name);

public slots:
    void createNewTemplate();

private slots:
    void showTemplatesInfo();
    void pageSelect(int page);
    void toggleAntialiasing();
    void loadTemplates();
    void saveTemplatesAs();

    void do_needCreateEmptyTemplates(QString &file_name);
    void saveUserName(QString & u_name);

    void setPages(QGraphicsScene *first, QGraphicsScene *second,
                  QGraphicsScene *third, QGraphicsScene *fourth);

    void errorA(QString e_msg); // Ошибка А типа - завершать рабту
    void errorB(QString e_msg); // Ошибка Б типа -  не  завершить работу
    void errorInfo(pluginsError eCode,QString e_msg); // Ошибка А типа -  завершить работу
    void do_CmdButtonClick(const QString &line);// Нажали командную кнопку
    void do_angle_direct();
    void do_viewCode();
signals:
    void addBaseElementToPage(int, const QString & line);
    void addImgElementToPage(int , QString &templ_fn);
private:
    /**
      * @brief Полученый из плагина набор страниц преобразует во вкладки редактора
      * Требования:
      * @li плагин tmpl_sql_plugin загружен
      * @li @fn loadTemplates() или  @fn do_needCreateEmptyTemplates() успешно отработали
      void setPage4List();//const QList<QGraphicsScene *> scene);
      */


    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createDockWindows();
    void loadPlugins();

    void error(QString e_msg,bool admin);
    void printTempl();
    void readGlobal(const QString &app_dir);
    /**
     * @fn Поворачивает страницу на 90 градусов по часовой или против
     */
    void flipPage(bool angle_direct);
    bool curPageOrient; // текущая ориентация страницы
    Auth_plugin *auth_plugin;
    Itmpl_sql_plugin *tmpl_plugin;

    QTabWidget * tabWidget;
    QMenu *templatesMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    QMenu *toolsMenu;

    QToolBar *editToolBar;
    QToolBar *toolsToolBar;

    QAction *antialiasAct;
    QAction *newAct;
    QAction *loadAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *showInfoAct;

    QAction *printAct;
    QAction *undoAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *quitAct;
    QAction *flipAction;
    QAction *addBaseElem;
    QAction *portretAct;
    QAction *landscapeAct;
    QAction *viewCodeAct;

    // Приватные данные

    int currentPage;
    QString userName;

    QString currentTemplates;
    QStringListModel *page_size_model;
    QStringList elemList;
    bool templ_load; // Признак что шаблон загружен

    AddTemplate *TProperDlg;
    cmdFrame * CmdButtonBox;

    // Переменные из установок
    QString serverHostName;
    int serverPort;
    QString spoolDir;
    QString ticket_fname;
    QString local_t_path; // Путь к локальным шаблонам пользователя
    QString global_t_path;
    QString ftpTemplatesDir;

    // Модель данных
    QStandardItemModel *doc_model; // Перейти на модель !!!!!
};

#endif
