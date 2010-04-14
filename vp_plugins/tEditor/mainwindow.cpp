#include "mainwindow.h"
#include "myscene.h"
#include "view.h"
#include "cmdframe.h"
#include "undoframe.h"
#include "editpagesmodel.h"

#include <QtCore/QDebug>
#include <QtCore/QUuid>
#include <QtCore/QSettings>
#include <QtCore/QPluginLoader>
#include <QtCore/QProcess>

#include <QtGui/QDesktopWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QMessageBox>
#include <QtGui/QToolBar>
#include <QtGui/QTabWidget>
#include <QtGui/QFileDialog>

#include <QtSql/QSqlQueryModel>

MainWindow::MainWindow():
        auth_plugin(0)
        , tmpl_plugin(0)
        , secondChance(false)
{

    this->readGlobal(qApp->applicationDirPath());
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

    /// @todo увеличение на максимум экрана!!!

    this->resize(800,600);
    tabWidget = new QTabWidget;

    // Создаем 8 View они существуют все время
    for (int i=0;i<8;i++){
        View * view = new View(this);
        view->setTabNumber(i);
        view->setMinimumSize(tabWidget->sizeHint());
        m_View.insert(i,view);
        // Если их не создавать тут то появляется артефакт отображения,
        // мелочь, но раздражает
        tabWidget->addTab(view,"page");
    }
    tabWidget->clear();
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

void MainWindow::loadPlugins()
{
    QDir pluginsDir(qApp->applicationDirPath());

    Itmpl_sql_plugin *tmpl_plugin_Interface;
    Auth_plugin *auth_plugin_Interface;

    if (pluginsDir.dirName().toLower() == "debug" ||
        pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();

#if defined(Q_OS_MAC)
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
                //auth_plugin->init(ticket_fname);
                auth_plugin->init();
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
}

// ----------------------------- Public slots

// ----------------------------- Private slots
void MainWindow::updateUndoLimit(int limit)
{
    View *vPage  = (View *)tabWidget->currentWidget();
    if (vPage) {
        int index = tabWidget->indexOf(vPage);
        Q_ASSERT(index != -1);
        static const QIcon unsavedIcon(":/t_needsave.png");
        tabWidget->setTabIcon(index, vPage->undoStack()->isClean() ? QIcon() : unsavedIcon);

        if (vPage->undoStack()->count() == 0)
            vPage->undoStack()->setUndoLimit(limit);
    }
}

void MainWindow::showTemplatesInfo()
{
    if (this->templ_load && tmpl_plugin ){
        // Покажем информацию о шаблоне
        TProperDlg->setInfoModel(tmpl_plugin->getInfoModel());
        TProperDlg->setPageSizeModel(tmpl_plugin->getPSizeModel());
        TProperDlg->setPagesModel(tmpl_plugin->getPagesModel());
        TProperDlg->default_init();
        TProperDlg->setEnableGUI(false);
        TProperDlg->exec();
    }
}

void MainWindow::do_CmdButtonClick(const QString &line)
{
    if (templ_load){
        // Арифметика Пупкина с картинками :)
        // Получим номер страницы отображаемой на текущей вкладке
        View *vPage  = (View *)tabWidget->currentWidget();
        int page = vPage->getTabNumber();

        if (line==tr("Изображение")){
            QString templ_fn = QFileDialog::getOpenFileName(this,
                                                            tr("Выберите картинку"),
                                                            ".",
                                                            tr("Картинки (*.jpg *.png *.bmp *.JPG *.PNG *.BMP)")
                                                            );
            if (!templ_fn.isEmpty()){
                //добавим картинку
                emit addImgElementToPage(page,templ_fn);
            }
        }else{
            emit addBaseElementToPage(page,line);
        }
    }else{
        QString e_msg = tr("Необходимо предварительно загрузить шаблон!");
        this->errorB(e_msg);
    }

}

void MainWindow::saveUserName(QString & u_name)
{
    userName = u_name;
}

void MainWindow::do_needCreateEmptyTemplates(QString &file_name)
{
    if (tmpl_plugin ){
        tmpl_plugin->saveTemplatesAs(file_name);

        if (tmpl_plugin->isDBOpened()){
            statusBar()->showMessage(QObject::trUtf8("Шаблон [%1] создан").arg(file_name),1000);
            // Теперь загрузим этот же шаблон
            loadFromFile(file_name);
        }else{
            statusBar()->showMessage(
                    QObject::trUtf8("Не возможно создать или загрузить шаблон [%1]")
                    .arg(file_name),1000);
        }

    }
}

void MainWindow::createNewTemplate()
{
    // Покажем дилоговое окошко с вводом параметров шаблона
    if (tmpl_plugin){
        // Запомним в плагине имя пользователя
        tmpl_plugin->setUserName (this->userName);
        /** @brief Создаем пустой шаблон в во временном каталоге
          * заданном при инициализации плагина
          * @todo сейчас создается во  временом системном каталоге изменить!
          */
        tmpl_plugin->createEmptyTemplate();
        if (tmpl_plugin->isDBOpened()){
            // Покажем информацию о шаблоне
            TProperDlg->setInfoModel(tmpl_plugin->getInfoModel());
            TProperDlg->setPageSizeModel(tmpl_plugin->getPSizeModel());
            TProperDlg->setPagesModel(tmpl_plugin->getPagesModel());
            TProperDlg->default_init();
            TProperDlg->setEnableGUI(true);
            TProperDlg->exec();
        }
    }
}

void MainWindow::loadTemplates()
{
    QString title_str;
    title_str =QObject::trUtf8("Выберите ранее сохраненные индивидуальные шаблоны");
    QString file_name =QFileDialog::getOpenFileName(this,
                                                    title_str,
                                                    local_t_path,
                                                    tr("Шаблоны (*.tmpl *.TMPL)")
                                                    );
    loadFromFile(file_name);
}

void MainWindow::saveTemplatesAs()
{
    QString title_str;
    title_str =QObject::trUtf8("Выберите место для сохранения шаблона");
    QString save_file = QFileDialog::getSaveFileName(this,
                                                     title_str,
                                                     local_t_path,
                                                     tr("Шаблоны (*.tmpl *.TMPL)")
                                                     );
    if (!save_file.isEmpty()){
        // Проверка на расширение
        QString name = save_file.section(".",0,0);
        QString ext  = save_file.section(".",1,1);
        if (ext.isEmpty() || ext.compare("tmpl",Qt::CaseInsensitive) != 0 ){
            save_file = name.append(".tmpl");
        }
        tmpl_plugin->saveTemplatesAs(save_file);
    }
}

void MainWindow::errorA(QString e_msg)
{
    error(e_msg,true);
}

void MainWindow::errorInfo(pluginsError eCode,QString e_msg)
{
    error(e_msg,true);
}

void MainWindow::errorB(QString e_msg)
{
    error(e_msg,false);
}

void MainWindow::do_viewCode()
{
    if (this->templ_load && tmpl_plugin ){
        // Данные режим работает на всех страницах шаблона
        for (int i = 0; i < tabWidget->count();i++){
            View * vPage  = (View *)tabWidget->widget(i);
            if (vPage){
                // Получим номер страницы отображаемой на i вкладке
                int page = vPage->getTabNumber();
                this->m_Scenes.value(page)->setViewMode();
            }
        }
    }else{
        QString e_msg = tr("Плагин для работы с шаблонами не загружен!");
        errorA(e_msg);

    }
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
void MainWindow::loadFromFile(const QString &file_name)
{
    //createView("Page size Model",tmpl_plugin->getPSizeModel());
    bool Ok =true;
    {
        Ok &= !file_name.isEmpty() && tmpl_plugin;
        if (Ok){
            tmpl_plugin->openTemplates(file_name);
            Ok &= tmpl_plugin->isDBOpened();
            if (Ok){
                // Если уже был показан шаблон, то были созданы вкладки, удалим их
                tabWidget->clear();
                // Теперь получим число страниц в шаблоне и покаже те страницы
                // которые не помечены как скрытые (удаленные) в шаблоне
                int p_number;
                int p_visible;
                QString p_name;
                View * vPage(0);
                pagesModel = tmpl_plugin->getPagesModel();
                for (int i=0;i<pagesModel->rowCount();i++){
                    p_visible = pagesModel->data(pagesModel->index(i,VPrn::PD_p_visible)).toInt();
                    if ( p_visible == 1 ){
                        p_name = pagesModel->data(pagesModel->index(i,VPrn::PD_p_name)).toString();
                        p_number  = pagesModel->data(pagesModel->index(i,VPrn::PD_p_number)).toInt();
                        /// Поиск View соответсвующего странице с номером
                        vPage = m_View.value( p_number );

                        vPage->setAngle(m_Scenes.value(p_number)->getAngle());
                        if (vPage){
                            tabWidget->addTab( vPage, p_name );
                        }else{
                            Ok &= false;
                        }
                    }
                }
                pageSelect(0);
                this->statusBar()->showMessage(tr("Шаблон [%1] загружен").arg(file_name));
                this->currentTemplates = file_name;
                showInfoAct->setEnabled(true);
            }
        }
    }
    if (!Ok){
        this->statusBar()->showMessage(tr("Ошибка загрузки шаблона [%1]")
                                       .arg(file_name),1000);
    }
    templ_load = Ok;
}

void MainWindow::error(QString e_msg,bool admin)
{
    QMessageBox msgBox;
    QPushButton *abortButton;
    QString info_txt;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle(QObject::trUtf8("Информационное сообщение"));
    info_txt =QObject::trUtf8("Для решения этой проблемы обратитесь к системному администратору!");
    msgBox.setInformativeText(info_txt);
    abortButton=msgBox.addButton(QObject::trUtf8("Выход"), QMessageBox::RejectRole);
    if (admin){
        /// @todo Не работает выход
        QObject::connect(&msgBox,SIGNAL(rejected()),qApp,SLOT(quit()));
    }
    msgBox.setText(e_msg);
    msgBox.exec();
}

void MainWindow::pageSelect(int page)
{

    View * vPage  = (View *)tabWidget->widget(page);
    m_undoGroup->setActiveStack(vPage == 0 ? 0 : vPage->undoStack());

    this->statusBar()->showMessage(tr("Выбранна страница шаблона - %1")
                                   .arg(tabWidget->tabText(page)),2500);
}

void MainWindow::createActions()
{
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

}

void MainWindow::createMenus()
{
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

}

void MainWindow::createToolBars()
{
    editToolBar = addToolBar(tr("Шаблоны"));
    editToolBar->addAction(newAct);
    editToolBar->addAction(loadAct);
    editToolBar->addAction(saveAct);

    toolsToolBar = addToolBar(tr("Утилиты"));
    toolsToolBar->addAction(antialiasAct);
    toolsToolBar->addAction(showInfoAct);
    toolsToolBar->addAction(viewCodeAct);
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
    addDockWidget(Qt::LeftDockWidgetArea,  dock);

    viewMenu->addAction(dock->toggleViewAction());

    myUndoFrame  = new UndoFrame(this);
    myUndoFrame->setMinimumWidth(100);
    addDockWidget(Qt::RightDockWidgetArea,myUndoFrame);

    viewMenu->addAction(myUndoFrame->toggleViewAction());
    connect(myUndoFrame, SIGNAL(undoLimitChange(int)),
            this,        SLOT  (updateUndoLimit(int)));
}

void MainWindow::printTempl()
{
    if (tmpl_plugin ){
        //TODO привести к новому виду
        /*
        tmpl_plugin->convertTemplatesToPdf(currentTemplates);
        */
    }

}

void MainWindow::readGlobal(const QString &app_dir)
{
    // Читаем файл настроек
    // TODO add emit log_message
    // QString l_msg = QString("[%1]").arg(QString::fromAscii(Q_FUNC_INFO));
    QString e_msg;
    QString ini_path =QString("%1/Technoserv/safe_printer.ini").arg(app_dir);

    if (QFile::exists(ini_path)){
        QSettings settings (ini_path,QSettings::IniFormat);
        settings.setIniCodec("UTF-8");
        settings.beginGroup("USED_DIR_FILE");
        spoolDir       = settings.value("spool_dir").toString();

#if defined(Q_OS_UNIX)
        ticket_fname   = settings.value("session_ticket").toString();
#endif
        settings.endGroup();

        settings.beginGroup("TEMPLATES");
        local_t_path  = settings.value("local_templates").toString();
        global_t_path = settings.value("global_templates").toString();
        settings.endGroup();

    }else{
        e_msg = QObject::trUtf8("Файл с настройками программы %1 не найден!").arg(ini_path);
        errorA(e_msg);
    }
}


//******************************************************************************
