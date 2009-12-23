#include <QStringListModel>
#include <QGraphicsScene>
#include <QAction>
#include <QToolBar>
#include <QSplitter>
#include <QMenuBar>
#include <QGridLayout>
#include <QPrinter>


#include "view.h"
#include "cmdFrame.h"
#include "addtemplate.h"

#include "teditor.h"

tEditor::tEditor(QDialog *parent) :
        QDialog(parent)
{
    // Создадим красоту феншуйскую
    QGridLayout *mlayout = new QGridLayout();
    menuBar = new QMenuBar(this);
    templatesMenu = new QMenu(tr("&Шаблоны"));
    //templatesMenu->addAction(newAct);
    //templatesMenu->addAction(loadAct);
    //templatesMenu->addAction(saveAsAct);
    //templatesMenu->addAction(showInfoAct);
    templatesMenu->addSeparator();
    menuBar->addMenu(templatesMenu);

    editMenu = new QMenu(tr("Правка"));
    menuBar->addMenu(editMenu);

    toolsMenu = new QMenu(tr("Утилиты"));
   // toolsMenu->addAction(antialiasAct);
    menuBar->addMenu(toolsMenu);

    helpMenu = new QMenu(tr("&Справка"));
    //helpMenu->addAction(aboutAct);
    menuBar->addMenu(helpMenu);

    //toolsToolBar = new QToolBar(this);
    //editToolBar = new QToolBar((tr("Правка")),this);

    // Создаем нужные окошки
    CmdButtonBox = new cmdFrame();
    TProperDlg  = new AddTemplate();

    //TProperDlg->setAttribute(Qt::WA_DeleteOnClose);
    TProperDlg->setWindowFlags(Qt::Dialog |
                               Qt::CustomizeWindowHint |
                               Qt::WindowTitleHint |
                               Qt::WindowCloseButtonHint |
                               Qt::WindowSystemMenuHint);
    //
    this->resize(800,600);

    tabWidget = new QTabWidget(this);
    View * view = new View(QObject::trUtf8("Лицевая сторона 1-го листа"));
    view->setMinimumSize(tabWidget->sizeHint());
    tabWidget->addTab(view,QObject::trUtf8("1-я страница"));

    view = new View(QObject::trUtf8("Лицевая сторона 2-го листа"));
    view->setMinimumSize(tabWidget->sizeHint());
    tabWidget->addTab(view,QObject::trUtf8("2-я страница"));

    view = new View(QObject::trUtf8("Обратная сторона каждого листа"));
    view->setMinimumSize(tabWidget->sizeHint());
    tabWidget->addTab(view,QObject::trUtf8("3-я страница"));

    view = new View(QObject::trUtf8("Обратная сторона последнего листа"));
    view->setMinimumSize(tabWidget->sizeHint());
    tabWidget->addTab(view,QObject::trUtf8("4-я страница"));
    currentPage = tabWidget->currentIndex();

    /*
    QHBoxLayout *lH = new QHBoxLayout;
    lH->addWidget(this->ui.listView);
    this->ui.groupBox->setLayout(lH);

    QGridLayout *gl = new QGridLayout;
    gl->addWidget(this->ui.btnHelp, 0, 0);
    gl->addWidget(this->ui.btnCancel, 0, 2);
    gl->addWidget(this->ui.btnOk, 0, 3);
    gl->setColumnStretch(1, 1);
    this->ui.widget->setLayout(gl);
    */
    //mlayout->addWidget(toolsToolBar,0,0);
    mlayout->addWidget(tabWidget,0,0,1,1);
    mlayout->addWidget(CmdButtonBox,0,1,1,1);


    //
    //layout->setMargin(1);
    //layout->setSpacing(1);
    //layout->addStretch(0);
    //

    mlayout->setMenuBar(menuBar);
    this->setLayout(mlayout);


    connect (tabWidget,
             SIGNAL(currentChanged(int)),
             this,
             SLOT(pageSelect(int)));

    createActions();
    createMenus();
    //createToolBars();
    //createStatusBar();

    setWindowTitle(tr("Редактор шаблонов"));


    // Создадим модель
    page_size_model = new QStringListModel(this);

    // Создадим кнопки
    CmdButtonBox->setCmdButton(elemList);
    // свяжем сигналы и слоты
    QObject::connect (TProperDlg,
                      SIGNAL(needCreateEmptyTemplates(QString &)),
                      this,
                      SLOT(do_needCreateEmptyTemplates(QString &))
                      );

    QObject::connect(CmdButtonBox,
                     SIGNAL(clicked(const QString &)),
                     this,
                     SLOT(do_CmdButtonClick(const QString &))
                     );

}

//             private  fuction
void tEditor::setPages(QGraphicsScene *first,
                       QGraphicsScene *second,
                       QGraphicsScene *third,
                       QGraphicsScene *fourth)
{
    View * vPage  = (View *)tabWidget->widget(0);
    if (vPage){
        vPage->gr_view()->setScene(first);
    }
    vPage  = (View *)tabWidget->widget(1);
    if (vPage){
        vPage->gr_view()->setScene(second);
    }
    vPage  = (View *)tabWidget->widget(2);
    if (vPage){
        vPage->gr_view()->setScene(third);
    }
    vPage  = (View *)tabWidget->widget(3);
    if (vPage){
        vPage->gr_view()->setScene(fourth);
    }
    show();

    QPrinter printer;
    QString with_t = ("1.pdf");

    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(with_t);
    printer.setPageSize(QPrinter::A4);
    QPainter painter(&printer);
    first->render(&painter);

}

// ----------------------------- Private slots
void tEditor::showTemplatesInfo()
{
    if (this->templ_load  ){
        // Покажем информацию о шаблоне
        TProperDlg->setTemplatesInfo(tInfo);
        TProperDlg->setPageSize(page_size_model);
        TProperDlg->setEnableGUI(false);
        TProperDlg->exec();
    }
}

void tEditor::toggleAntialiasing()
{
    // Данные режим работает на всех страницах шаблона
    for (int i = 0; i < tabWidget->count();i++){
        View * vPage  = (View *)tabWidget->widget(i);
        if (vPage){
            vPage->gr_view()->setRenderHint(QPainter::Antialiasing,
                                            antialiasAct->isChecked());
        }
    }
}

void tEditor::pageSelect(int page)
{
    currentPage = page;

    View * vPage  = (View *)tabWidget->widget(page);
    //if (vPage){
    //   this->statusBar()->showMessage(vPage->getPageName());
    //}

}

void tEditor::createActions()
{

    showInfoAct = new  QAction (QIcon(":/t_info.png"),
                                tr("Показать свойства шаблона"),this);
    showInfoAct->setShortcut(QKeySequence(tr("Ctrl+I")));
    showInfoAct->setStatusTip(tr("Показать свойства текущего шаблона [CTRL+I]"));
    showInfoAct->setEnabled(templ_load);
    connect(showInfoAct,
            SIGNAL(triggered()),
            this,
            SLOT(showTemplatesInfo())

            );


    newAct = new QAction(QIcon(":/t_new.png"),
                         tr("Создание шаблона ..."),this);
    newAct->setShortcut(QKeySequence::New);
    newAct->setStatusTip(tr("Создание пустого шаблона"));
    connect (newAct,
             SIGNAL(triggered()),
             this,
             SLOT(do_createEmptyTemplate())
             );

    loadAct = new QAction(QIcon(":/t_open.png"),
                          tr("Загрузка шаблона ..."),this);
    loadAct->setShortcut(QKeySequence::Open);
    loadAct->setStatusTip(tr("Загрузка в редактор шаблона для просмотра и редактирования"));
    connect (loadAct,
             SIGNAL(triggered()),
             this,
             SLOT(loadTemplates()));

    saveAsAct = new QAction(QIcon(":/t_save.png"),
                            tr("Сохранение шаблона как ..."),this);
    saveAsAct->setShortcut(QKeySequence::Open);
    saveAsAct->setStatusTip(tr("Сохранение текущего варианта шаблона в ..."));
    connect (saveAsAct,
             SIGNAL(triggered()),
             this,
             SLOT(saveTemplatesAs()));

    antialiasAct = new QAction(tr("Сглаживание"),this);
    antialiasAct->setCheckable(true);
    antialiasAct->setStatusTip(tr("Режим отображения"));
    connect(antialiasAct, SIGNAL(triggered()), this, SLOT(toggleAntialiasing()));

    quitAct = new QAction(tr("Выход"), this);
    quitAct->setShortcuts(QKeySequence::Close);
    quitAct->setStatusTip(tr("Выход из редактора"));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("О программе"), this);
    aboutAct->setStatusTip(tr("Краткие сведения о программе"));
    //connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("О библиотеке Qt"), this);
    aboutQtAct->setStatusTip(tr("Краткие сведения об используемой библиотеке Qt"));
    //connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void tEditor::createMenus()
{

templatesMenu->addAction(newAct);
}

void tEditor::createToolBars()
{
    editToolBar->addAction(newAct);
    editToolBar->addAction(loadAct);
    //editToolBar->addAction(saveAct);
    editToolBar->addAction(saveAsAct);
    toolsToolBar = new QToolBar((tr("Утилиты")),this);
    toolsToolBar->addAction(antialiasAct);
    toolsToolBar->addAction(showInfoAct);
}

