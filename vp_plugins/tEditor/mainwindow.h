#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QAction;
class QListWidget;
class QMenu;
class QTextEdit;
class QStringListModel;
QT_END_NAMESPACE

class View;

#include "itmpl_plugin.h"
#include "auth_plugin.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();


private slots:
    void pageSelect(int page);
    void toggleAntialiasing();
    void loadTemplates();
    void createEmptyTemplates();

    void saveUserName(QString & u_name);

    void setPages(QGraphicsScene *first, QGraphicsScene *second,
                  QGraphicsScene *third, QGraphicsScene *fourth);

    void errorA(QString e_msg); // Ошибка А типа - админ безопастности
    void errorB(QString e_msg); // Ошибка Б типа - сисадмин
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
    QAction *printAct;
    QAction *undoAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *quitAct;
    int currentPage;
    QString userName;

    QStringListModel *page_size_model;

    // Переменные из установок
    QString local_path; // Путь к локальным шаблонам пользователя

};

#endif
