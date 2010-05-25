#include "mainwindow.h"
#include "igs_plugin.h"
#include "itmpl_sql_plugin.h"

#include <QtCore/QDebug>
#include <QtCore/QUuid>
#include <QtCore/QHash>
#include <QtCore/QSettings>
#include <QtCore/QPluginLoader>
#include <QtCore/QProcess>
#include <QtCore/QDir>
#include <QtCore/QStringList>
#include <QtCore/QUuid>
#include <QtCore/QFileInfoList>

#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMessageBox>


#include <QtSql/QSqlQueryModel>

#include <QPair>

MainWindow::MainWindow()
    :myGs_plugin(0)
    ,myTmpl_plugin(0)
    ,m_Ready(false)
{
    readGlobal(qApp->applicationDirPath());
    m_Ready = loadPlugins();

}

bool MainWindow::loadPlugins()
{
    QDir pluginsDir(qApp->applicationDirPath());


    Igs_plugin       *gs_plugin_Interface;
    Itmpl_sql_plugin *tmpl_plugin_Interface;

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
        QPluginLoader pluginManager(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginManager.instance();
        if (plugin) {
            bool needUnloadPlugin = true;
            {
                if (!myTmpl_plugin){
                    tmpl_plugin_Interface = qobject_cast<Itmpl_sql_plugin *> (plugin);
                    if ( tmpl_plugin_Interface ){
                        needUnloadPlugin = false;
                        myTmpl_plugin = tmpl_plugin_Interface;

                        connect(plugin, SIGNAL (error(pluginsError,QString )),
                                this,   SLOT   (errorInfo(pluginsError,QString ))
                                );
                    }
                }
                if (!myGs_plugin){
                    /// Загрузка плагина работы с ghostscript
                    gs_plugin_Interface = qobject_cast<Igs_plugin *> (plugin);
                    if (gs_plugin_Interface){
                        needUnloadPlugin = false;
                        myGs_plugin = gs_plugin_Interface;

                        connect(plugin, SIGNAL (error(pluginsError,QString )),
                                this,   SLOT   (errorInfo(pluginsError,QString ))
                                );

                        connect (plugin,
                                 SIGNAL( docReady4work(const QString &,int) ),
                                 SLOT ( do_docReady4work (const QString &,int) )
                                 );
                        connect (plugin,
                                 SIGNAL( docReady4print(const QString &) ),
                                 SLOT ( do_docReady4print (const QString &) )
                                 );
                        connect (plugin,
                                 SIGNAL( docReady4preview(const QString &) ),
                                 SLOT ( do_docReady4preview (const QString &) )
                                 );
                    }
                }
            }
            if ( needUnloadPlugin ){
                // Выгрузим его нафиг не наш он плагин, сто пудово :)
                qDebug() << QObject::trUtf8("Plugin's [%1] unload").arg(pluginsDir.absoluteFilePath(fileName));
                pluginManager.unload();
            }
        }
    }
    return (myGs_plugin && myTmpl_plugin);

}

//----------------------------------------------------------------------------------------------------------------------------------------
void MainWindow::test()
{
    QString main = QString("/opt/vprn/spool/main_29.ps");
    if ( m_Ready ){
        c_uuid = QUuid::createUuid().toString().replace("{","").replace("}","");
        myGs_plugin->init( gsBin, pdftkBin,spoolDir);
        myTmpl_plugin->init( spoolDir,c_uuid ); /// @todo c_uuid тут лишний УБРАТЬ!!!!

        if (myGs_plugin){
            // Выполняет Хранитель при коннекте нового клиента
            myGs_plugin->createClientData(c_uuid);
            // От клиента пришел запрос на конвертацию ps -> pdf
            myGs_plugin->convertPs2Pdf(c_uuid, main); // Завершение работы сигнал docReady4work
        }
    }
    this->show();
}

//----------------------------------------- PRIVATE SLOTS ----------------------------------------------------------------------
void MainWindow::do_docReady4work (const QString &client_uuid,int pCount)
{
    QMessageBox msgbox;
    msgbox.setText(QString("Число страниц в документе %1").arg(pCount,0,10));
    msgbox.exec();

    // Документ готов к дальнейшей обработке
    QByteArray data = test_prepareUserData();
    if (data.size() != 0){
        // Мета задача объединить исходный документ с каждым вариантом шаблона
        // Сигнал о завершении docReady4preview, после объединения всех
        test_GK_mergeWithTemplate(client_uuid,data);
    }
}

void MainWindow::test_GK_Print(const QString &client_uuid)
{
    // Документ готов к  печати
    // Обработали все файлы надо пройти по всем каталогам и собрать *_out.pdf
    QStringList files;


    files =  findFiles(client_uuid,QStringList() << "*out.pdf" << "*OUT.PDF");

    for (int i=1;i<5;i++){
        QString firstpage;
        QString otherpage;
        QString overside;
        QString lastpage;
        QString filename;
        int copy_num(0);

        //Формируем группу файлов относящуюся к одному эземпляру
        for (int j=0;j<files.size();j++ ){
            filename = files.at( j );
            QRegExp rx("/(.+)/(.+)/(.)-copy/(.+_out).pdf");
            if(rx.indexIn( filename ) != -1){
                // Наш файлик можно обрабатывать
                copy_num = rx.cap(3).toInt();
                QString page_type  = rx.cap(4);
                if (copy_num == i){
                    if ( page_type.compare("firstpage_out",Qt::CaseInsensitive) == 0){
                        firstpage = filename;
                    }
                    if ( page_type.compare("otherpage_out",Qt::CaseInsensitive) == 0) {
                        // Лицевая сторона второго и последующих листов
                        otherpage = filename;
                    }
                    if ( page_type.compare("oversidepage_out",Qt::CaseInsensitive) == 0){
                        overside = filename;
                    }
                    if ( page_type.compare("lastpage_out",Qt::CaseInsensitive) == 0){
                        lastpage = filename;
                    }
                }
            }
        }
        // Получили группу файлов теперь отправляем на печать
        if (! firstpage.isEmpty() && !overside.isEmpty() && !lastpage.isEmpty()){
            myGs_plugin->print2devide(client_uuid,firstpage,QString("PDF"),false);
            if (!otherpage.isEmpty()){
                myGs_plugin->print2devide(client_uuid,otherpage,QString("PDF"),false);
            }

            QMessageBox msgBox;
            QPushButton *nextButton = msgBox.addButton(QObject::trUtf8("Дальше"), QMessageBox::ActionRole);
            msgBox.setText(QString("После окончания печати лицевой стороны %1 экземпляра документа,\nПереверните листы и нажмите кнопку Дальше.")
                           .arg(i,0,10 ));
            msgBox.exec();
            if (msgBox.clickedButton() == nextButton) {
                myGs_plugin->print2devide(client_uuid,overside,QString("PDF"),true);
                myGs_plugin->print2devide(client_uuid,lastpage,QString("PDF"),false);
            }
        }
    }
}

void MainWindow::do_docReady4print (const QString &client_uuid)
{
    // Документ готов к конвертированию в png  или печати
    // Обработали все файлы надо пройти по всем каталогам и собрать *_out.pdf
    QStringList files;
    files =  findFiles(client_uuid,QStringList() << "*out.pdf" << "*OUT.PDF");

    PreviewMode mode = VPrn::FullPreviewMode;
    //PreviewMode mode =VPrn::PartPreviewMode;
    //PreviewMode mode = VPrn::PrintWithoutPreviewMode:
    if (myGs_plugin){
        myGs_plugin->convertPdfToPng(client_uuid,files, mode );                
        // Отправили файлы клиенту он их посмотрел и дал добро печатаем!
        test_GK_Print( client_uuid);
    }
}

void MainWindow::do_docReady4preview (const QString &client_uuid)
{
    // Документ готов к предпросмотру
    // Обработали все файлы надо пройти по всем каталогам и собрать *.png
    QStringList files;
    files =  findFiles(client_uuid,QStringList() << "*.png" << "*.PNG");    
}

//---------------------------------------- PRIVATE SECTIONS -------------------------------------------------------------------
void MainWindow::test_GK_mergeWithTemplate(const QString &client_uuid,QByteArray data)
{
    QString t_fileName;
    QPair <int,int> copy_count;
    QHash <QString, QString> m_tagValue;
    // Получили данные переданные пользователем, разбор данных
    QDataStream in(&data, QIODevice::ReadOnly );
    in.setVersion(QDataStream::Qt_3_0);
    in >> copy_count;
    in >> t_fileName;
    // читаем значения
    in >> m_tagValue;

    bool Ok = true;
    {
        if (copy_count.first == 0  && copy_count.second == 5){
            for (int i=1; i<5;i++){
                m_tagValue[QObject::trUtf8("Номер экз.")] = QString("%1").arg(i,0,10);
                // Формируем страницы шаблона в pdf
                Ok &= myTmpl_plugin->prepare_template(client_uuid,t_fileName, m_tagValue,i);
            }
        }else{
            m_tagValue[QObject::trUtf8("Номер экз.")] ==QString("%1").arg(copy_count.first,0,10);
            // Формируем страницы шаблона в pdf
            Ok &=  myTmpl_plugin->prepare_template(client_uuid,t_fileName, m_tagValue,copy_count.first) ;
        }
    }
    if (Ok &&  myGs_plugin ){
        QStringList files =  findFiles(client_uuid,QStringList()
                                       << "t_firstpage.pdf"        << "T_FIRSTPAGE.PDF"
                                       << "t_otherpage.pdf"      << "T_OTHERPAGE.PDF"
                                       << "t_oversidepage.pdf" << "T_OVERSIDEPAGE.PDF"
                                       << "t_lastpage.pdf"         << "T_LASTPAGE.PDF"   );

        // Сформируем документ подготовленный к печати  документ + шаблон
        myGs_plugin->mergeWithTemplate(client_uuid, files ) ;
    }
}

QByteArray MainWindow::test_prepareUserData()
{
    /**
      * @brief  Формируем набор данных, тот что передает клиент
      */
    QByteArray fields_data;
    QHash <QString, QString> m_tagValue;
    QString t_fileName = QString("/opt/vprn/local_templates/a4.tmpl");

    //формируем хеш значений
    m_tagValue.insert( QObject::trUtf8("Название док-та"),QObject::trUtf8("Приказ о назначении л-т Иванова И.а на должность командира 1 взвода 3 мср от 12.01.10."));
    m_tagValue.insert( QObject::trUtf8("Гриф"), QObject::trUtf8("Сов.Секретно") );
    m_tagValue.insert( QObject::trUtf8("МБ"),QObject::trUtf8("МБ12/13-1"));
    m_tagValue.insert( QObject::trUtf8("Пункт перечня"),QObject::trUtf8("п.3") );
    m_tagValue.insert( QObject::trUtf8("Кол-во листов"), QObject::trUtf8("77") );
    m_tagValue.insert( QObject::trUtf8("Номер экз."),  QObject::trUtf8("0") );
    m_tagValue.insert( QObject::trUtf8("Кол-во экз."),  QObject::trUtf8("5") );
    m_tagValue.insert( QObject::trUtf8("Исполнитель"),  QObject::trUtf8("Иванов С.А."));
    m_tagValue.insert( QObject::trUtf8("Отпечатал"),  QObject::trUtf8("Иванов С.А."));
    m_tagValue.insert( QObject::trUtf8("Телефон"),   QObject::trUtf8("77-33-12"));
    m_tagValue.insert( QObject::trUtf8("Инв. N"),  QObject::trUtf8("Инв.№77"));
    m_tagValue.insert( QObject::trUtf8("Дата распечатки"), QObject::trUtf8("15.05.2010"));
    m_tagValue.insert(QObject::trUtf8("Получатель N1"), QObject::trUtf8("Получатель N1 - в дело1"));
    m_tagValue.insert(QObject::trUtf8("Получатель N2"), QObject::trUtf8("Получатель N2 - в дело2"));
    m_tagValue.insert(QObject::trUtf8("Получатель N3"), QObject::trUtf8("Получатель N3 - в дело3"));
    m_tagValue.insert(QObject::trUtf8("Получатель N4"), QObject::trUtf8("Получатель N4 - в дело4"));
    m_tagValue.insert(QObject::trUtf8("Получатель N5"), QObject::trUtf8("Получатель N5 - в дело5"));

    // Заполним его в формате ключ-значение
    QDataStream out(&fields_data, QIODevice::WriteOnly );
    out.setVersion(QDataStream::Qt_3_0);
    //Запишем выбранный пользователем экземпляр
    out << qMakePair(0,5); // №экз, всего экз
    out << t_fileName;
    // save hash
    out << m_tagValue;
    return fields_data;
}

QStringList MainWindow::findFiles(const QString &client_uuid,const QStringList &filters)
{
    QStringList files;
    QDir dir = QDir::current();

    // Формируем списк файлов которые надо подвергнуть преобразованию в png
    for (int i=1;i<5;i++){

        dir = QDir(QString ("%1/%2/%3-copy").arg(spoolDir,client_uuid).arg(i,0,10) );
        dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
        dir.setNameFilters(filters);
        QFileInfoList list = dir.entryInfoList();
        for (int i = 0; i < list.size(); ++i)
        {
            QFileInfo fileInfo = list.at(i);
            files.append(dir.absoluteFilePath(fileInfo.fileName()));
        }
    }
    return files;
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

        settings.beginGroup("POSTSCRIPT");
        gsBin = settings.value("gs_bin").toString();
        settings.endGroup();
        settings.beginGroup("PDF");
        pdftkBin = settings.value("pdfTK").toString();
        settings.endGroup();

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
    }
}

void MainWindow::errorInfo(pluginsError errCode,QString error_message)
{
    QMessageBox mbox;
    mbox.setText(error_message);
    mbox.exec();
}

//******************************************************************************
