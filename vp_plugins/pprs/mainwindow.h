#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QDebug>
#include <QMainWindow>
#include <QUndoStack>
#include <QUndoGroup>
#include <QUndoView>


QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
QT_END_NAMESPACE


#include "mytypes.h"

using namespace VPrn;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();

//public slots:

private slots:
    //void errorInfo(pluginsError eCode,QString e_msg); // Ошибка А типа -  завершить работу

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createDockWindows();
    //void loadPlugins();

    void readGlobal(const QString &app_dir);

    QMenu *templatesMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    QMenu *toolsMenu;


    QToolBar *editToolBar;
    QToolBar *toolsToolBar;

    QAction *undoAct;
    QAction *redoAct;

    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *quitAct;

    //UndoFrame * myUndoFrame;
};

#endif
