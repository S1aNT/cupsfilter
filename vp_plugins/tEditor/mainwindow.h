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
#include "itmpl_plugin.h"
#include "auth_plugin.h"
#include "addtemplate.h"
#include "templ_info.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
    //Директор по такелажу :) - т.е грузчик !(Грузит шаблон из файла)
    bool loadFromFile(const QString &file_name);
    bool loadFromFileWithDat(const QString &file_name,const QString &file_name_dat);
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
    void do_CmdButtonClick(const QString &line);// Нажали командную кнопку
    void do_angle_direct();
    void do_viewCode();
signals:
    void addBaseElementToPage(int, QStringList & line);
    void addImgElementToPage(int , QString &templ_fn);
private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createDockWindows();
    void loadPlugins();

    void error(QString e_msg,bool admin);
    void printTempl();
    void readGlobal(const QString &app_dir);
    /*
     * Поворачивает страницу на 90 градусов по часовой или против
     */
    void flipPage(bool angle_direct);
    bool curPageOrient; // текущая ориентация страницы
    Auth_plugin *auth_plugin;
    Itmpl_plugin *tmpl_plugin;

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
    Templ_info tInfo;
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
