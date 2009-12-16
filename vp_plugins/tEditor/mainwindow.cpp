#include <QDebug>
#include <QtGui>
#include <QtGui/QFrame>
#include <QDesktopWidget>
#include <QTabWidget>
#include <QFileDialog>
#include <QStringListModel>

#include "mainwindow.h"
#include "view.h"
#include "cmdFrame.h"

MainWindow::MainWindow()
{
    templ_load = false;
    // Создаем нужные окошки
    CmdButtonBox = new cmdFrame(this);

    TProperDlg  = new AddTemplate(this);
    //TProperDlg->setAttribute(Qt::WA_DeleteOnClose);
    TProperDlg->setWindowFlags(Qt::Dialog |
                               Qt::CustomizeWindowHint |
                               Qt::WindowTitleHint |
                               Qt::WindowCloseButtonHint |
                               Qt::WindowSystemMenuHint);
    //
    this->resize(800,600);

    tabWidget = new QTabWidget;
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
    connect (tabWidget,
             SIGNAL(currentChanged(int)),
             this,
             SLOT(pageSelect(int)));
    setCentralWidget(tabWidget);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    createDockWindows();
    setWindowTitle(tr("Редактор шаблонов"));
    setUnifiedTitleAndToolBarOnMac(true);

    // Создадим модель
    page_size_model = new QStringListModel(this);

    //Все структуры созданы - грузим апельсины бочками
    loadPlugins();

    // Создадим кнопки
    CmdButtonBox->setCmdButton(elemList);
    // свяжем сигналы и слоты
    QObject::connect (TProperDlg,
                      SIGNAL(needCreateEmptyTemplates(QString,QString,QString,QString,
                                                      QString,bool,QString,
                                                      qreal,qreal,qreal,qreal)),
                      this,
                      SLOT(createEmptyTemplate(QString,QString,QString,QString,
                                               QString,bool,QString,
                                               qreal,qreal,qreal,qreal))
                      );
    QObject::connect(CmdButtonBox,
                     SIGNAL(clicked(const QString &)),
                     this,
                     SLOT(do_CmdButtonClick(const QString &))
                     );


}


void MainWindow::test_slot()
{
    QMessageBox msgBox;
    msgBox.setText("The  1111.");
    msgBox.exec();

}

void MainWindow::loadPlugins()
{
    QDir pluginsDir(qApp->applicationDirPath());
    /*
    Inet_plugin *net_plugin_Interface;
    Igs_plugin *gs_plugin_Interface;
    Auth_plugin *auth_plugin_Interface;
*/
    Itmpl_plugin *tmpl_plugin_Interface;
    Auth_plugin *auth_plugin_Interface;
#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#endif
    //this->readGlobal(app_dir); // Читаем глобальные настройки

    pluginsDir.cd("plugins");
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)){
        QPluginLoader pluginMessageer(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginMessageer.instance();
        if (plugin) {
            connect(plugin,
                    SIGNAL(error(QString )),
                    this,
                    SLOT (errorA(QString ))
                    );
            // Загрузка плагина авторизации
            auth_plugin_Interface = qobject_cast<Auth_plugin *>(plugin);
            if (auth_plugin_Interface){
                auth_plugin =auth_plugin_Interface;
                connect(plugin,
                        SIGNAL(get_User_name_mandat(QString &,QString &)),
                        this,
                        SLOT(saveUserName(QString&)) // Мне нужно только имя
                        );

#if defined(Q_OS_UNIX)
                //FIXME
                QString ticket_name = "/tmp/session_ticket";

                auth_plugin->init(ticket_name);
#elif defined(Q_OS_WIN)
                auth_plugin->init();
#endif
                //emit StateChanged (authPluginInit);

            }

            tmpl_plugin_Interface = qobject_cast<Itmpl_plugin *> (plugin);
            if (tmpl_plugin_Interface){
                tmpl_plugin = tmpl_plugin_Interface;
                //FIXME
                QString spool ="c:/spool/";
                QString sid = "ewqfrieie";
                tmpl_plugin->init(spool,sid);

                // Получим названия стандартных кнопок для шаблона
                elemList = tmpl_plugin->getElemNameList();

                // Получим список размеров страниц
                page_size_model->setStringList(tmpl_plugin->getPageSizeList());
                connect (plugin,
                         SIGNAL(allTemplatesPagesParsed(QGraphicsScene *,
                                                        QGraphicsScene *,
                                                        QGraphicsScene *,
                                                        QGraphicsScene *)),
                         this,
                         SLOT(setPages(QGraphicsScene *,
                                       QGraphicsScene *,
                                       QGraphicsScene *,
                                       QGraphicsScene *))
                         );
                connect(this,
                        SIGNAL(addBaseElementToPage(int,QStringList &)),
                        plugin,
                        SLOT(doAddBaseElementToPage(int,QStringList &))
                        );
                connect(this,
                        SIGNAL(addImgElementToPage(int,QString &)),
                        plugin,
                        SLOT(doAddImgElementToPage(int,QString &))
                        );

                /*
                connect (plugin,
                         SIGNAL(allPagesConverted(QString &,QString &,
                                                  QString &,QString &)),
                         this,
                         SLOT(mergeDocWithTemplate(QString &,QString &,
                                                   QString &,QString &))
                         );

                connect (this,SIGNAL(needUpdatePage(int)),plugin,SLOT(update_scene(int)));

                connect (teditorDlg,
                         SIGNAL(saveTemplates()),
                         plugin,
                         SLOT(doSaveTemplates())
                         );
                         */
            }
        }
    }

}


// ----------------------------- Private slots
void MainWindow::do_CmdButtonClick(const QString &line)
{
    if (templ_load){
        if (line==tr("Изображение")){
            QString templ_fn = QFileDialog::getOpenFileName(this,
                                                            tr("Выберите картинку"),
                                                            ".",
                                                            tr("Картинки (*.jpg *.png *.bmp *.JPG *.PNG *.BMP)")
                                                            );
            if (!templ_fn.isEmpty()){
                //добавим картинку
                emit addImgElementToPage(this->currentPage+1,templ_fn);
            }
        }else{
            QStringList list;
            list << line;
            emit addBaseElementToPage(this->currentPage+1,list);
        }
    }else{
        QString e_msg = tr("Необходимо предварительно загрузить шаблон!");
        this->errorA(e_msg);
    }

}

void MainWindow::saveUserName(QString & u_name)
{
    userName = u_name;
}

void MainWindow::createEmptyTemplate(const QString & file_name,
                                     const QString & t_author,
                                     const QString & t_name,
                                     const QString & t_desc,
                                     const QString & p_size,

                                     bool pages_orient,
                                     const QString & c_date,
                                     qreal m_top,
                                     qreal m_bottom,
                                     qreal m_right,
                                     qreal m_left)
{
    if (tmpl_plugin){
        tmpl_plugin->createEmptyTemplate(file_name,t_author,t_name,
                                         t_desc, p_size, pages_orient,c_date,
                                         m_top,m_bottom,m_right,m_left);
        this->statusBar()->showMessage(QObject::tr("Шаблон [%1] создан")
                                       .arg(file_name)
                                       );
    }
}

void MainWindow::createEmptyTemplates()
{
    // Покажем дилоговое окошко с вводом параметров шаблона

    TProperDlg->setUserName(userName);
    TProperDlg->setPageSize(page_size_model);
    //FIXME
    QString local_templ_dir ="c:/local_templ_dir/";
    TProperDlg->setLocalTemplatesDir(local_templ_dir);

    int ret = TProperDlg->exec();
    if (ret == QDialog::Accepted){
        //Создадим новый шаблончик

    }
}

void MainWindow::setPages(QGraphicsScene *first,
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

}

void MainWindow::loadTemplates()
{
    QString title_str;
    title_str =QObject::trUtf8("Выберите ранее сохраненные индивидуальные шаблоны");
    if (local_path.isEmpty()){
        local_path = qApp->applicationDirPath();
    }
    QString templ_fn = QFileDialog::getOpenFileName(this,
                                                    title_str,
                                                    local_path,
                                                    tr("Шаблоны (*.tmpl *.TMPL)")
                                                    );
    if (!templ_fn.isEmpty()){
        if (tmpl_plugin){
            tmpl_plugin->loadTemplates(templ_fn);
            this->statusBar()->showMessage(QObject::tr("Шаблон [%1]загружен")
                                           .arg(templ_fn)
                                           );
            this->templ_load = true;
        }


    }
}

void MainWindow::errorA(QString e_msg)
{
    error(e_msg,true);
}

void MainWindow::errorB(QString e_msg)
{
    error(e_msg,false);
}

void MainWindow::toggleAntialiasing()
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
//******************************************************************************
//------------------------------- Private function
void MainWindow::error(QString e_msg,bool admin)
{
    QMessageBox msgBox;
    QPushButton *abortButton;
    QString info_txt;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle(QObject::trUtf8("Информационное сообщение"));
    if (admin){
        info_txt =QObject::trUtf8("Для решения этой проблемы обратитесь к системному администратору!");
    }else{
        info_txt =QObject::trUtf8("Для решения этой проблемы обратитесь к администратору безопасности!");
    }
    msgBox.setInformativeText(info_txt);
    abortButton=msgBox.addButton(QObject::trUtf8("Выход"), QMessageBox::RejectRole);
    msgBox.setText(e_msg);
    QObject::connect(&msgBox,SIGNAL(rejected()),qApp,SLOT(quit()));
    msgBox.exec();

}

void MainWindow::pageSelect(int page)
{
    currentPage = page;

    View * vPage  = (View *)tabWidget->widget(page);
    if (vPage){
        this->statusBar()->showMessage(vPage->getPageName());
    }

}

void MainWindow::createActions()
{
    printAct = new QAction(QIcon(":/t_print.png"),
                           tr("Пробная печать шаблона"),this);

    newAct = new QAction(QIcon(":/t_new.png"),
                         tr("Создание шаблона ..."),this);
    newAct->setShortcut(QKeySequence::New);
    newAct->setStatusTip(tr("Создание пустого шаблона"));
    connect (newAct,
             SIGNAL(triggered()),
             this,
             SLOT(createEmptyTemplates())
             );

    loadAct = new QAction(QIcon(":/t_open.png"),
                          tr("Загрузка шаблона ..."),this);
    loadAct->setShortcut(QKeySequence::Open);
    loadAct->setStatusTip(tr("Загрузка в редактор шаблона для просмотра и редактирования"));
    connect (loadAct,
             SIGNAL(triggered()),
             this,
             SLOT(loadTemplates()));

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
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("О библиотеке Qt"), this);
    aboutQtAct->setStatusTip(tr("Краткие сведения об используемой библиотеке Qt"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus()
{
    templatesMenu = menuBar()->addMenu(tr("&Шаблоны"));
    templatesMenu->addAction(newAct);
    templatesMenu->addAction(loadAct);
    templatesMenu->addSeparator();

    templatesMenu->addAction(quitAct);
    editMenu = menuBar()->addMenu(tr("Правка"));

    viewMenu = menuBar()->addMenu(tr("Панели"));

    toolsMenu = menuBar()->addMenu(tr("Утилиты"));
    toolsMenu->addAction(antialiasAct);

    menuBar()->addSeparator();
    helpMenu = menuBar()->addMenu(tr("&Справка"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

void MainWindow::createToolBars()
{
    editToolBar = addToolBar(tr("Правка"));
    editToolBar->addAction(newAct);
    editToolBar->addAction(loadAct);
    //editToolBar->addAction(saveAct);
    toolsToolBar = addToolBar(tr("Утилиты"));
    toolsToolBar->addAction(antialiasAct);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Готов"));
}

void MainWindow::createDockWindows()
{
    QDockWidget *dock = new QDockWidget(tr("Элементы"), this);
    dock->setMinimumWidth(100);
    //dock->setMinimumWidth(200);
    //FIX проблема зафиксировать размер кнопок
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setWidget(CmdButtonBox);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    viewMenu->addAction(dock->toggleViewAction());
}

//******************************************************************************
