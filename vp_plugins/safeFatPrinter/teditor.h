#ifndef TEDITOR_H
#define TEDITOR_H

#include <QDialog>
#include <QMenu>

QT_BEGIN_NAMESPACE
class QAction;
class QTabWidget;
class QToolBar;
class QStringListModel;
class QGraphicsScene;
class QMenuBar;
QT_END_NAMESPACE

class View;
class cmdFrame;
class AddTemplate;

#include "templ_info.h"
#include "templ_info_d.h"

class tEditor : public QDialog
{
Q_OBJECT
public:
    tEditor(QDialog *parent = 0);
    void showTemplatesInfo();

    void toggleAntialiasing();
    void pageSelect(int page);
    /*

    void loadTemplates();
    void saveTemplatesAs();
    void do_createEmptyTemplate();
    void saveUserName(QString & u_name);


    void do_needCreateEmptyTemplates(QString &file_name);
    void errorA(QString e_msg); // Ошибка А типа - завершать рабту
    void errorB(QString e_msg); // Ошибка Б типа -  не  завершить работу
    void do_CmdButtonClick(const QString &line);// Нажали командную кнопку
   */
public slots:
    void setPages(QGraphicsScene *first, QGraphicsScene *second,
                  QGraphicsScene *third, QGraphicsScene *fourth);
signals:
    void addBaseElementToPage(int, QStringList & line);
    void addImgElementToPage(int , QString &templ_fn);
private:
    void createActions();
    void createMenus();
    void createToolBars();
    //void createStatusBar();


    bool templ_load; // Признак что шаблон загружен
    int currentPage; // Текущая страница
    // Приватные данные
    Templ_info tInfo;


    AddTemplate *TProperDlg;
    cmdFrame * CmdButtonBox;

    QStringListModel *page_size_model;
    QStringList elemList;

    QTabWidget * tabWidget;


    QMenu *templatesMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    QMenu *toolsMenu;

    QMenuBar *menuBar;

    QToolBar *editToolBar;
    QToolBar *toolsToolBar;

    QAction *antialiasAct;
    QAction *newAct;
    QAction *loadAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *showInfoAct;

    QAction *undoAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *quitAct;
    QAction *flipAction;
    QAction *addBaseElem;
};

#endif // TEDITOR_H
