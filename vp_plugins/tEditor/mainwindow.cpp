#include <QDebug>
#include <QtGui>
#include <QUuid>
#include <QDesktopWidget>
#include <QTabWidget>
#include <QFileDialog>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QSettings>

#include "mainwindow.h"
#include "view.h"
#include "cmdFrame.h"

MainWindow::MainWindow()
{
    this->readGlobal(qApp->applicationDirPath());
    // Создаем модель
    doc_model     = new QStandardItemModel(this);

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

                auth_plugin->init(ticket_name);
#elif defined(Q_OS_WIN)
                auth_plugin->init();
#endif
                //emit StateChanged (authPluginInit);

            }

            tmpl_plugin_Interface = qobject_cast<Itmpl_plugin *> (plugin);
            if (tmpl_plugin_Interface){
                tmpl_plugin = tmpl_plugin_Interface;


                QUuid uSID=QUuid::createUuid () ;  //generate SID
                QString sid=uSID.toString().replace("{","").replace("}","");

                tmpl_plugin->init(spoolDir,sid);

                // Получим названия стандартных кнопок для шаблона
                elemList = tmpl_plugin->getElemNameList();

                doc_model->setHorizontalHeaderLabels(elemList);
                insertDocToModel();

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

            }
        }
    }

}


// ----------------------------- Private slots
void MainWindow::showTemplatesInfo()
{
    if (this->templ_load && tmpl_plugin ){
        // Покажем информацию о шаблоне
        //TODO функцию очистки
        TProperDlg->setPageSize(page_size_model);
        TProperDlg->setTemplatesInfo(tInfo);
        TProperDlg->setEnableGUI(false);
        TProperDlg->exec();
    }
}

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
        this->errorB(e_msg);
    }

}

void MainWindow::saveUserName(QString & u_name)
{
    //FIXME запись в tInfo.setT_author()!!!
    userName = u_name;
}

void MainWindow::do_needCreateEmptyTemplates(QString &file_name)
{
    if (tmpl_plugin ){
        tmpl_plugin->setTemplInfo(tInfo);

        tmpl_plugin->createEmptyTemplate(file_name);
        this->statusBar()->showMessage(QObject::tr("Шаблон [%1] создан")
                                       .arg(file_name),1000);
        // Теперь загрузим этот же шаблон
        templ_load = loadFromFile(file_name);
        if (templ_load){
            this->statusBar()->showMessage(QObject::tr("Шаблон [%1] загружен")
                                           .arg(file_name),1000);
            this->currentTemplates = file_name;
             showInfoAct->setEnabled(true);

        }else{
            this->statusBar()->showMessage(QObject::tr("Ошибка загрузки шаблона [%1]")
                                           .arg(file_name),1000);

        }

    }
}

void MainWindow::do_createEmptyTemplate()
{
    // Покажем дилоговое окошко с вводом параметров шаблона
    if (tmpl_plugin ){

        // Получим информацию о шаблоне
        tInfo = tmpl_plugin->getTemplInfo();

        tInfo.setT_author(userName);// Сохраним имя пользователя

        TProperDlg->setTemplatesInfo(tInfo);
        TProperDlg->setPageSize(page_size_model);


        TProperDlg->setLocalTemplatesDir(local_t_path);
        TProperDlg->setEnableGUI(true);
        // Покажем информацию о шаблоне
        TProperDlg->exec();
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
        //vPage->gr_view()->fitInView(first->sceneRect());
    }
    vPage  = (View *)tabWidget->widget(1);
    if (vPage){
        vPage->gr_view()->setScene(second);
        //vPage->gr_view()->fitInView(second->sceneRect());

    }
    vPage  = (View *)tabWidget->widget(2);
    if (vPage){
        vPage->gr_view()->setScene(third);
        //vPage->gr_view()->fitInView(third->sceneRect());
    }
    vPage  = (View *)tabWidget->widget(3);
    if (vPage){
        vPage->gr_view()->setScene(fourth);
        //vPage->gr_view()->fitInView(fourth->sceneRect());
    }

}

void MainWindow::loadTemplates()
{
    QString title_str;
    title_str =QObject::trUtf8("Выберите ранее сохраненные индивидуальные шаблоны");
    /*
    if (local_t_pathh.isEmpty()){
        local_t_path = qApp->applicationDirPath();
    }
    */
    QString file_name =QFileDialog::getOpenFileName(this,
                                                    title_str,
                                                    local_t_path,
                                                    tr("Шаблоны (*.tmpl *.TMPL)")
                                                    );

    templ_load = loadFromFile(file_name);
    if (templ_load){
        this->statusBar()->showMessage(QObject::tr("Шаблон [%1] загружен")
                                       .arg(file_name),1000);
        this->currentTemplates = file_name;
        showInfoAct->setEnabled(true);

    }else{
        this->statusBar()->showMessage(QObject::tr("Ошибка загрузки шаблона [%1]")
                                       .arg(file_name),1000);

    }
}
void MainWindow::saveTemplatesAs()
{
    QString title_str;
    title_str =QObject::trUtf8("Выберите место для сохраненнения шаблона");
    /*
    if (local_path.isEmpty()){
        local_path = qApp->applicationDirPath();
    }
    */
    QString save_file = QFileDialog::getSaveFileName(this,
                                                     title_str,
                                                     local_t_path,
                                                     tr("Шаблоны (*.tmpl *.TMPL)")
                                                     );
    if (!save_file.isEmpty()){
        tmpl_plugin->setTemplInfo(tInfo);
        tmpl_plugin->saveTemplatesAs(save_file);
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
void MainWindow::do_angle_direct()
{
    curPageOrient = tInfo.page_orient();
    if (QAction *action = qobject_cast<QAction*>(sender())) {
        QVariant v = action->data();
        if (v.canConvert<QString>()) {
            QString who_click = qvariant_cast<QString>(v);
            if (curPageOrient && who_click =="Land"){
                flipPage(true);
            }
            if (!curPageOrient && who_click =="Port"){
                flipPage(false);
            }
        }
    }
}


void MainWindow::flipPage(bool angle_direct)
{
    QMatrix  mat;
    qreal angle;
    if (angle_direct){
        angle = -90;

    }else{
        angle = 90;
    }
    mat.rotate(angle);

    View * vPage  = (View *)tabWidget->widget(0);
    if (vPage){
        vPage->gr_view()->setMatrix(mat,true);
        vPage->setAngle(angle);
    }
    vPage  = (View *)tabWidget->widget(1);
    if (vPage){
       vPage->gr_view()->setMatrix(mat,true);
        vPage->setAngle(angle);
    }
    vPage  = (View *)tabWidget->widget(2);
    if (vPage){
       vPage->gr_view()->setMatrix(mat,true);
         vPage->setAngle(angle);
    }
    vPage  = (View *)tabWidget->widget(3);
    if (vPage){
        vPage->gr_view()->setMatrix(mat,true);
         vPage->setAngle(angle);
    }
}

//******************************************************************************
//------------------------------- Private function
void MainWindow::insertDocToModel()
{
    // add empty row to model
    doc_model->insertRow(doc_model->rowCount());
}



void MainWindow::insertDocToModel(QString &item)
{
    if (!item.isEmpty() && item.contains(";:;", Qt::CaseInsensitive)){
        QStringList itemList= item.split(";:;");
        if (itemList.size()>0){
            QList<QStandardItem *> cells;


            for (int i = 0; i <itemList.size() ; ++i) {
                QStandardItem * cell_item= new QStandardItem();
                cells.append(cell_item);
            }
            /*
            QStringList list_2;
            for (int i = 0; i <itemList.size() ; ++i) {
                QString tmp_str=itemList.at(i);
                qDebug() << tmp_str;
                if (!tmp_str.isEmpty() && tmp_str.contains("=")){
                    list_2.clear();
                    list_2 = tmp_str.split("=");
                    if (list_2.size() == 1){
                        QStandardItem *cell_item =cells.at(this->getElemIdByName(list_2.at(0)));
                        cell_item->setData(QVariant(list_2.at(1)),Qt::EditRole);
                        qDebug() << "Key" << list_2.at(0) << " Value " << list_2.at(1);
                    }
                }
            }
            */
            doc_model->appendRow (cells);
        }

    }

}


bool MainWindow::loadFromFile(QString &file_name)
{
    bool flag = false;
    if (!file_name.isEmpty()){
        if (tmpl_plugin ){
            //tmpl_plugin->setTemplInfo(tInfo);
            tmpl_plugin->loadTemplates(file_name);

            tInfo = tmpl_plugin->getTemplInfo();
            flag = true;
        }
    }
    return flag;
}

void MainWindow::error(QString e_msg,bool admin)
{
    QMessageBox msgBox;
    QPushButton *abortButton;
    QString info_txt;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle(QObject::trUtf8("Информационное сообщение"));
    info_txt =QObject::trUtf8("Для решения этой проблемы обратитесь к системному администратору!");
    if (admin){
        QObject::connect(&msgBox,SIGNAL(rejected()),qApp,SLOT(quit()));
    }
    msgBox.setInformativeText(info_txt);
    abortButton=msgBox.addButton(QObject::trUtf8("Выход"), QMessageBox::RejectRole);
    msgBox.setText(e_msg);

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

    printAct = new QAction(QIcon(":/t_print.png"),
                           tr("Пробная печать шаблона"),this);
    connect(printAct,
            SIGNAL(triggered()),
            this,
            SLOT(printTempl())

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
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("О библиотеке Qt"), this);
    aboutQtAct->setStatusTip(tr("Краткие сведения об используемой библиотеке Qt"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    portretAct = new QAction(QIcon(":/portret.png"),
                             tr("Книжная ориентация страниц"),this);
    portretAct->setStatusTip(tr("Выбор книжной ориентации страниц"));
    portretAct->setData(QString("Port"));
    connect (
            portretAct,
            SIGNAL(triggered()),
            this, SLOT(do_angle_direct())
            );
    landscapeAct = new QAction(QIcon(":/landscape.png"),
                               tr("Альбомная ориентация страниц"),this);
    landscapeAct->setStatusTip(tr("Выбор альбомной ориентации страниц"));
    landscapeAct->setData(QString("Land"));
    connect (
            landscapeAct,
            SIGNAL(triggered()),
            this, SLOT(do_angle_direct())
            );
    QActionGroup * orientGroup = new QActionGroup(this);
    orientGroup->addAction(portretAct);
    orientGroup->addAction(landscapeAct);
}

void MainWindow::createMenus()
{
    templatesMenu = menuBar()->addMenu(tr("&Шаблоны"));
    templatesMenu->addAction(newAct);
    templatesMenu->addAction(loadAct);
    templatesMenu->addAction(saveAsAct);
    templatesMenu->addAction(showInfoAct);
    templatesMenu->addAction(printAct);

    templatesMenu->addSeparator();

    templatesMenu->addAction(quitAct);
    editMenu = menuBar()->addMenu(tr("Правка"));

    viewMenu = menuBar()->addMenu(tr("Панели"));

    toolsMenu = menuBar()->addMenu(tr("Утилиты"));
    toolsMenu->addAction(antialiasAct);
    toolsMenu->addAction(portretAct);
    toolsMenu->addAction(landscapeAct);

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
    editToolBar->addAction(saveAsAct);
    toolsToolBar = addToolBar(tr("Утилиты"));
    toolsToolBar->addAction(antialiasAct);
    toolsToolBar->addAction(showInfoAct);
    toolsToolBar->addAction(portretAct);
    toolsToolBar->addAction(landscapeAct);

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

void MainWindow::printTempl()
{
    if (tmpl_plugin ){
        tmpl_plugin->convertTemplatesToPdf(currentTemplates);
    }

}

void MainWindow::readGlobal(const QString &app_dir)
{
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
        e_msg = QObject::trUtf8("Файл %1 не найден!").arg(ini_path);
        this->error(e_msg,false);
    }
}

//******************************************************************************
