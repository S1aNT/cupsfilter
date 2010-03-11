#include "mainwindow.h"
//#include "undoframe.h"

#include <QtCore/QDebug>
#include <QtCore/QUuid>
#include <QtCore/QSettings>
#include <QtCore/QPluginLoader>

#include <QtGui/QDesktopWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QMessageBox>
#include <QtGui/QToolBar>
#include <QtGui/QTabWidget>
#include <QtGui/QFileDialog>

#include <QtSql/QSqlQueryModel>


#include <QtGui/QTableView>

MainWindow::MainWindow()
        : auth_plugin(0)
        , tmpl_plugin(0)
{

    this->readGlobal(qApp->applicationDirPath());

    /// @todo увеличение на максимум экрана!!!
    this->resize(800,600);
    //setCentralWidget(tabWidget);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    createDockWindows();

    setWindowTitle(tr("Редактор шаблонов"));
    setUnifiedTitleAndToolBarOnMac(true);
}


void MainWindow::loadPlugins()
{
    QDir pluginsDir(qApp->applicationDirPath());
/*
    Itmpl_sql_plugin *tmpl_plugin_Interface;
    Auth_plugin *auth_plugin_Interface;

#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" ||
        pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#endif
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
                auth_plugin->init(ticket_name);
#elif defined(Q_OS_WIN)
                auth_plugin->init();
#endif
                //emit StateChanged (authPluginInit);

            }

            tmpl_plugin_Interface = qobject_cast<Itmpl_sql_plugin  *> (plugin);
            if (tmpl_plugin_Interface){
                tmpl_plugin = tmpl_plugin_Interface;

                QUuid uSID=QUuid::createUuid () ;  //generate SID
                QString sid=uSID.toString().replace("{","").replace("}","");

                connect(plugin,
                        SIGNAL(error(pluginsError,QString )),
                        this,
                        SLOT (errorInfo(pluginsError,QString ))
                        );

                connect(this,
                        SIGNAL(addBaseElementToPage(int,const QString &)),
                        plugin,
                        SLOT(doAddBaseElementToPage(int,const QString &))
                        );
                connect (plugin,
                         SIGNAL(allTemplatesPagesParsed()),
                         this,
                         SLOT(PagesUpdate())
                         );

                connect(saveAct, SIGNAL(triggered()),
                        plugin,  SLOT(saveTemplates())
                        );
                connect(printAct, SIGNAL(triggered()),
                        plugin, SLOT(convert2Pdf())
                       );
                tmpl_plugin->init(spoolDir,sid);
                // Получим названия стандартных кнопок для шаблона
                elemList = tmpl_plugin->getBaseElemNameList();
                // Получим набор стеков Undo  и свяжем его с окном Undo
                this->m_undoGroup = tmpl_plugin->getUndoGrp();

                myUndoFrame->setUndoViewGroup(m_undoGroup);

                undoAct = m_undoGroup->createUndoAction( this,tr("Отменить действие") );
                redoAct = m_undoGroup->createRedoAction( this,tr("Повторить действие") );

                undoAct->setIcon(QIcon(":/undo.png"));
                redoAct->setIcon(QIcon(":/redo.png"));
                undoAct->setShortcut( QKeySequence::Undo );
                redoAct->setShortcut( QKeySequence::Redo );

                editMenu->addAction(undoAct);
                editMenu->addAction(redoAct);
                editMenu->addSeparator();                

                // Получим набор сцен и соопоставим его страницам отображения

                this->m_Scenes = tmpl_plugin->getScenesGroup();
                View * vPage(0);
                int page;
                QGraphicsScene *scene(0);
                QMapIterator<int, myScene *>  i(m_Scenes);
                while (i.hasNext()) {
                    i.next();
                    page  = i.key();
                    if (page >= 0 && page < 8 ){
                        scene = i.value();
                        vPage = m_View.value(page);
                        vPage->gr_view()->setScene(scene);
                        vPage->setUndoStack(i.value()->undoStack());                        
                    }else{
                        emit error(tr("Ошибочное число страниц [%1] в шаблоне").arg(i.key(),0,10)
                                   ,true);
                    }
                }
            }
        }
    }
*/
}



void MainWindow::createActions()
{
/*
    viewCodeAct = new QAction(QIcon(":/view_code.png"),
                              tr("Показать [коды] / значения полей реквизитов"),this);
    viewCodeAct->setStatusTip(tr("Режим отображения [код] / значение реквизита"));
    connect(viewCodeAct, SIGNAL(triggered()),
            this,        SLOT(do_viewCode()) );

    showInfoAct = new  QAction (QIcon(":/t_info.png"),
                                tr("Показать свойства шаблона"),this);
    showInfoAct->setShortcut(QKeySequence(tr("Ctrl+I")));
    showInfoAct->setStatusTip(tr("Показать свойства текущего шаблона [CTRL+I]"));
    showInfoAct->setEnabled(templ_load);
    connect(showInfoAct, SIGNAL(triggered()),
            this,        SLOT(showTemplatesInfo())  );

    printAct = new QAction(QIcon(":/t_print.png"),
                           tr("Пробная печать шаблона"),this);

    newAct = new QAction(QIcon(":/t_new.png"),
                         tr("Создание шаблона ..."),this);
    newAct->setShortcut(QKeySequence::New);
    newAct->setStatusTip(tr("Создание пустого шаблона"));
    connect(newAct, SIGNAL(triggered()),
            this,  SLOT(createNewTemplate()));

    loadAct = new QAction(QIcon(":/t_open.png"),
                          tr("Загрузка шаблона ..."),this);
    loadAct->setShortcut(QKeySequence::Open);
    loadAct->setStatusTip(tr("Загрузка в редактор шаблона для просмотра и редактирования"));
    connect (loadAct,SIGNAL(triggered()),
             this,   SLOT(loadTemplates()));

    saveAsAct = new QAction(QIcon(":/t_save.png"),
                            tr("Сохранение шаблона как ..."),this);
    saveAsAct->setShortcut(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Сохранение текущего варианта шаблона в ..."));
    connect(saveAsAct, SIGNAL(triggered()),
            this,     SLOT(saveTemplatesAs()));

    saveAct = new QAction(QIcon(":/t_save.png"),
                            tr("Сохранение шаблона"),this);
    saveAct->setShortcut(QKeySequence::Save);
    saveAct->setStatusTip(tr("Сохранение текущего варианта шаблона"));

    antialiasAct = new QAction(tr("Сглаживание"),this);
    antialiasAct->setCheckable(true);
    antialiasAct->setStatusTip(tr("Режим отображения"));
    connect(antialiasAct, SIGNAL(triggered()),
            this, SLOT(toggleAntialiasing()));

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
*/
}

void MainWindow::createMenus()
{
/*
    templatesMenu = menuBar()->addMenu(tr("&Шаблоны"));
    templatesMenu->addAction(newAct);
    templatesMenu->addAction(loadAct);
    templatesMenu->addAction(saveAct);
    templatesMenu->addAction(saveAsAct);
    templatesMenu->addAction(showInfoAct);
    templatesMenu->addAction(printAct);
    templatesMenu->addSeparator();
    templatesMenu->addAction(quitAct);

    viewMenu = menuBar()->addMenu(tr("Панели"));

    toolsMenu = menuBar()->addMenu(tr("Утилиты"));
    toolsMenu->addAction(antialiasAct);
    toolsMenu->addAction(viewCodeAct);

    editMenu = menuBar()->addMenu(tr("Правка"));

    helpMenu = menuBar()->addMenu(tr("&Справка"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
 */
}

void MainWindow::createToolBars()
{
/*
    editToolBar = addToolBar(tr("Шаблоны"));
    editToolBar->addAction(newAct);
    editToolBar->addAction(loadAct);
    editToolBar->addAction(saveAct);

    toolsToolBar = addToolBar(tr("Утилиты"));
    toolsToolBar->addAction(antialiasAct);
    toolsToolBar->addAction(showInfoAct);
    toolsToolBar->addAction(viewCodeAct);
*/
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Готов"));
}

void MainWindow::createDockWindows()
{ /*

    QDockWidget *dock = new QDockWidget(tr("Элементы"), this);
    dock->setMinimumWidth(100);
    //dock->setMinimumWidth(200);
    //FIX проблема зафиксировать размер кнопок
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setWidget(CmdButtonBox);
    addDockWidget(Qt::LeftDockWidgetArea,  dock);

    viewMenu->addAction(dock->toggleViewAction());

    myUndoFrame  = new UndoFrame(this);
    myUndoFrame->setMinimumWidth(100);
    addDockWidget(Qt::RightDockWidgetArea,myUndoFrame);

    viewMenu->addAction(myUndoFrame->toggleViewAction());
    connect(myUndoFrame, SIGNAL(undoLimitChange(int)),
            this,        SLOT  (updateUndoLimit(int)));
*/
}


void MainWindow::readGlobal(const QString &app_dir)
{
/*
    // Читаем файл настроек
    // TODO add emit log_message
    QString l_msg = QString("[%1]").arg(QString::fromAscii(Q_FUNC_INFO));
    QString e_msg;
    QString ini_path =QString("%1/Technoserv/safe_printer.ini").arg(app_dir);

    if (QFile::exists(ini_path)){
        QSettings settings (ini_path,QSettings::IniFormat);
        settings.setIniCodec("UTF-8");
        settings.beginGroup("SERVICE");
        serverHostName = settings.value("server").toString();
        serverPort = settings.value("port").toInt();
        settings.endGroup();
#if defined(Q_OS_UNIX)
        settings.beginGroup("USED_DIR_FILE");
        spoolDIR = settings.value("spool_dir").toString();
        ticket_fname=settings.value("session_ticket").toString();
        settings.endGroup();

        settings.beginGroup("TEMPLATES");
        local_t_path=settings.value("local_templates").toString();
        global_t_path=settings.value("global_templates").toString();
        ftpTemplatesDir=settings.value("ftp_templates_dir",).toString();
        settings.endGroup();
#elif defined(Q_OS_WIN)
        settings.beginGroup("USED_DIR_FILE");
        spoolDir= settings.value("spool_dir").toString();
        settings.endGroup();

        settings.beginGroup("TEMPLATES");
        local_t_path=settings.value("local_templates").toString();
        global_t_path=settings.value("global_templates").toString();
        ftpTemplatesDir=settings.value("ftp_templates_dir").toString();
        settings.endGroup();
#endif

    }else{
        e_msg = QObject::trUtf8("Файл с настройками программы %1 не найден!").arg(ini_path);
        errorA(e_msg);
    }
*/
}

//******************************************************************************
