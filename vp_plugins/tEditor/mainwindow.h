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
private slots:
    void showTemplatesInfo();
    void pageSelect(int page);
    void toggleAntialiasing();
    void loadTemplates();
    void do_createEmptyTemplate();
    void saveUserName(QString & u_name);
    void setPages(QGraphicsScene *first, QGraphicsScene *second,
                  QGraphicsScene *third, QGraphicsScene *fourth);

    void do_needCreateEmptyTemplates(QString &file_name);
    void errorA(QString e_msg); // Ошибка А типа - завершать рабту
    void errorB(QString e_msg); // Ошибка Б типа -  не  завершить работу
    void do_CmdButtonClick(const QString &line);// Нажали командную кнопку
    void test_slot();
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

    //Inet_plugin *net_plugin;
    //Igs_plugin *gs_plugin;
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
    QAction *showInfoAct;

    QAction *printAct;
    QAction *undoAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *quitAct;
    QAction *flipAction;
    QAction *addBaseElem;
    // Приватные данные
    Templ_info tInfo;
    int currentPage;
    QString userName;

    QStringListModel *page_size_model;
    QStringList elemList;
    bool templ_load; // Признак что шаблон загружен

    AddTemplate *TProperDlg;
    cmdFrame * CmdButtonBox;
    // Переменные из установок
    QString local_path; // Путь к локальным шаблонам пользователя

};

#endif
