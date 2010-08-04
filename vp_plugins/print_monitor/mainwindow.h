#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "intropage.h"
#include "selectpage.h"
#include "printdatapage.h"
#include "checkdatapage.h"
#include "previewpage.h"
#include "finishpage.h"
#include "datamodule.h"

#include <QtGui/QDialog>
#include <QtGui/QDesktopWidget>

QT_BEGIN_NAMESPACE
class QAction;
class QStackedWidget;
class QFrame;
class QComboBox;
class QLabel;
class QStandardItemModel;
class QHBoxLayout;
class QVBoxLayout;
class QSpacerItem;
class QErrorMessage;
QT_END_NAMESPACE

namespace Ui {
    class MainWindow;
}

class Engine;

class MainWindow : public QWidget {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void init(const QString &app_dir,const QString &input_file);
public slots:
    void appendStartMsg(const QString &txt_msg);

protected:
    void changeEvent(QEvent *e);

private slots:
    void do_next();
    void do_prev();
    void do_restart();
    void do_needAuthUser(const QString &login_mandat_list);
    void sendFileToConvertor();        

private:        
    QDesktopWidget desktop;
    int            pageId;
    IntroPage      *introPage;
    SelectPage     *selectPage;
    PrintDataPage  *printDataPage;
    CheckDataPage  *checkDataPage;
    PreViewPage    *preViewPage;
    FinishPage     *finishPage;

    QStackedWidget *stackedWidget;
    QHBoxLayout    *horizontalLayout;
    QVBoxLayout    *mainLayout;

    QPushButton    *helpButton;    
    QPushButton    *prevButton;
    QPushButton    *nextButton;
    QPushButton    *restartButton;
    QPushButton    *exitButton;
    QSpacerItem    *horizontalSpacer;
    QSpacerItem    *horizontalSpacer_2;

    QWidget        *verticalLayoutWidget;
    QWidget        *horizontalLayoutWidget;
    QFrame         *line;
    QErrorMessage  *eMessage;

    DataModule     *data_module;
    Engine         *engine;

    QString link_name;
    QString gatekeeper_bin;
    QRect   desktop_avail;

    //----------------------------------------
    void createConnection();
    QPoint calcDeskTopCenter(int width,int height);
    void readConfig(const QString &app_dir);
    /**
      * @fn bool lastCheck(int page);
      * @brief Проверка можно ли перейти на следующую страницу
      */
    bool lastCheck(int page);

};

#endif // MAINWINDOW_H
