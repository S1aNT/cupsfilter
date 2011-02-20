#include "gs_plugin.h"
#include "proc_thread.h"
#include "clientdata.h"

#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QFileInfoList>
#include <QtCore/QRegExp>
#include <QtCore/QTemporaryFile>
#include <QtCore/QTextStream>
#include <QtCore/QDateTime>
#include <QtCore/QUuid>

#include <QtGui/QPixmap>

GS_plugin::GS_plugin(QObject *parent)
        :QObject (parent)
{
    //    if (!QMetaType::isRegistered(QMetaType::type("JobsType"))){
    //        qRegisterMetaType<VPrn::Jobs>("JobsType");
    //    }
}


void GS_plugin::final_clear()
{
    //Самый важнецкий деструктор, удаляет временный файл  gs_rcp;
    // В дальнейшем туточки будет выгружаться библиотека gs_lib
    QFile::remove(gs_rcp);
}

void GS_plugin::init(const QString &gs_bin, const QString &pdftk_bin,
                     const QString &gsprint_bin,const QString &temp_folder)
{
    QDir dir;
    const QString startnow = QDir::currentPath();    
    // Проверка существования и доступности бинарников
    if (!QFile::exists(gs_bin)){
        emit error(VPrn::FileIOError,
                   QObject::trUtf8("Файл %1 не существует или недостачно прав доступа\n%2")
                   .arg(gs_bin)
                   .arg(QString(Q_FUNC_INFO)));
        emit pluginNotReady();
        return;
    }else{
        gsBin = gs_bin;
    }
    // Проверка существования и доступности бинарников
    if (!QFile::exists(pdftk_bin)){
        emit error(VPrn::FileIOError,
                   QObject::trUtf8("Файл %1 не существует или недостачно прав доступа\n%2")
                   .arg(pdftk_bin)
                   .arg(QString(Q_FUNC_INFO)));
        emit pluginNotReady();
        return;
    }else{
        pdftkBin = pdftk_bin;
    }
    if (!QFile::exists(gsprint_bin)){
        emit error(VPrn::FileIOError,
                   QObject::trUtf8("Файл %1 не существует или недостачно прав доступа\n%2")
                   .arg(gsprint_bin)
                   .arg(QString(Q_FUNC_INFO)));
        emit pluginNotReady();
        return;
    }else{
        gsprintBin = gsprint_bin;
    }
    if (dir.cd(temp_folder)){// Проверим факт существования временного каталога
        spoolDir = temp_folder;
        myEnv << QObject::trUtf8("TMPDIR=%1").arg(temp_folder);
        createRCPfile(spoolDir);
    }else{
        emit error(VPrn::FileIOError,
                   QObject::trUtf8("Временный каталог %1 не найден\n%2")
                   .arg(temp_folder).arg(QString(Q_FUNC_INFO)));
        emit pluginNotReady();
    }
    QDir::setCurrent(startnow);
}

void GS_plugin::convertPs2Pdf(const QString &client_uuid,const QString &input_fn)
{
    // Проверим фактик наличия входного файла
    qDebug() << Q_FUNC_INFO << input_fn;
    if (QFile::exists(input_fn)) {
        // Поиск данных для клиента
        ClientData *c_data = findClientData(client_uuid);

        // Формируем основной выходной файл для клиента
        c_data->setMainFile(QString("%1/%2/main.pdf")
                            .arg( spoolDir, client_uuid ) );

        // Формируем строку команды для первого этапа
        QStringList args;

        args.append("-q" );
        args.append(QString("@%1").arg(gs_rcp));
        args.append(QString("-sOutputFile=%1").arg(c_data->getMainFile()));
        args.append("-c");
        args.append(".setpdfwrite");
        args.append("-f");
        args.append(input_fn);
        start_proc(client_uuid,gsBin,args,VPrn::job_ConvertPs2Pdf);

    }else {
        emit error(VPrn::FileIOError,
                   QObject::trUtf8("ERROR : Файл %1 не найден\n%2")
					.arg(input_fn)
					.arg(QString(Q_FUNC_INFO)));
    }
}

void GS_plugin::directPrint(const QString &client_uuid,const QString &file_name,
                            const QString &printer_name,int copies)
{
    // Поиск данных для клиента
    QStringList args;
    QProcess proc;

#if defined(Q_OS_UNIX)                
    args.append(QString("-P %1").arg(printer_name));
    args.append(tr("-#%1").arg(copies,0,10));
    args.append(QString("\"%1\"").arg(file_name).toLatin1());

    proc.start( gsprintBin, args );
#elif defined(Q_OS_WIN) //-color -noquery -all -printer %1 -copies %2 %3
    args.append("-color");
    args.append("-noquery");
    args.append("-all");
    args.append(QString("-printer \"%1\"").arg(printer_name).toLatin1());
    args.append(QString("-copies %1").arg(copies,0,10));
    args.append(file_name);

    /// Запуск под окошками требует извращений
    QFile t_file;
    QString tmp_cmd;
    {
        // Генерация уникального имени файла
        QTemporaryFile tmp_file;// Создадим временный файл
        tmp_file.setFileTemplate("XXXXXXXX.bat");
        if (tmp_file.open()){
            QFileInfo f_info(tmp_file);
            tmp_cmd = f_info.absoluteFilePath();
            tmp_file.close();
            t_file.setFileName(tmp_cmd);
        }else{
          emit error(VPrn::FileIOError,
                           QObject::trUtf8("Ошибка создания временного файла %1\n%2")
                           .arg( tmp_cmd )
                           .arg(QString(Q_FUNC_INFO))
                           );
            return;
        }
    }

    QTextStream out(&t_file);
    if (t_file.open(QIODevice::WriteOnly)){
        out << QObject::tr("@echo off\n");
        if (gsprintBin.contains(" ") ){
            out << QObject::tr("\"%1\" ").arg(gsprintBin);
        }else{
            out << QObject::tr("%1 ").arg(gsprintBin);
        }
        for (int i=0;i<args.size();i++){
            out << args.at(i) << " ";
        }
        t_file.close();

        QFile::copy( tmp_cmd, "c:/gsprint.bat");

        proc.start( tmp_cmd );
    }else{
        emit error(VPrn::FileIOError,
                       QObject::trUtf8("Ошибка записи данных во временный файл %1\n%2")
                       .arg( tmp_cmd )
                       .arg(QString(Q_FUNC_INFO))
                       );
        return;
    }
#endif
    // Дождемся развязки славного запуска

    if (!proc.waitForStarted(5000)){
        emit error (VPrn::InternalAppError,
                    QObject::trUtf8("Ошибка gsprint (Процесс не стартовал) при печати файла %1!\n%2")
                    .arg(file_name)
                    .arg(QString(Q_FUNC_INFO))
                    );
        return;
    }
    proc.waitForFinished(-1);
    proc.closeWriteChannel();
    qDebug() << "Direct print output: " << proc.readAll();//.trimmed();
    emit filePrinted(client_uuid);
#if defined(Q_OS_WIN)
    t_file.remove();
#endif

}

void GS_plugin::catPdf(const QString &client_uuid,const QString &file_nameA,
                       const QString &file_nameB, const QString &output_name)
{

    QStringList args;
    args.append(QString("A=%1").arg(file_nameA));
    args.append(QString("B=%1").arg(file_nameB));
    args.append("cat A B output ");
    args.append(output_name);

    start_proc(client_uuid,pdftkBin,args,VPrn::job_CatPages);
}

void GS_plugin::mergeWithTemplate(const QString &client_uuid,
                                  const QStringList &t_files,
                                  VPrn::PreviewMode prn_mode)
{

    // Поиск данных для клиента
    ClientData *c_data = findClientData(client_uuid);
    if (!QFile::exists(c_data->getFirstPage() ) ){
        emit error (VPrn::FileIOError,
                    QObject::trUtf8("Файл %1 не найден!\n%2")
                    .arg(c_data->getFirstPage()).arg(QString(Q_FUNC_INFO)) );
        return;
    }

    if (t_files.size() == 0){
        emit error (VPrn::InternalAppError,
                    QObject::trUtf8("Ошибка при подготовке шаблона!\n%1").arg(QString(Q_FUNC_INFO))
                    );

        return;
    }
    // Цикл по всем элементам шаблона, для каждого экземпляра
    QString backPdf;
    QString outPdf;
    for (int i=0; i< t_files.size();i++){
        backPdf = t_files.at(i);
        // spoolDir/client_uuid/t_firstpage.pdf
        QRegExp rx("/(.+)/(.+)/(.-copy)/(t_.+).pdf");
        if(rx.indexIn( backPdf ) != -1){
            // Наш файлик можно обрабатывать
            QString copy_num = rx.cap(3);
            QString page_type  = rx.cap(4);

            if ( page_type.compare("t_firstpage",Qt::CaseInsensitive) == 0){
                // Лицевая сторона i экз
                outPdf = QString("%1/%2/%3/firstpage_out.pdf")
                         .arg(spoolDir,client_uuid,copy_num);
                c_data->startMerge( i );
                mergePdf(client_uuid,c_data->getFirstPage(),backPdf,outPdf);
            }
            if ( page_type.compare("t_otherpage",Qt::CaseInsensitive) == 0  &&
                 c_data->getPageCount() >1    ){
                outPdf = QString("%1/%2/%3/otherpage_out.pdf")
                         .arg(spoolDir,client_uuid,copy_num);
                c_data->startMerge( i );
                // Лицевая сторона второго и последующих листов
                mergePdf(client_uuid,c_data->getOtherPage(),backPdf,outPdf);
            }
            if ( page_type.compare("t_oversidepage",Qt::CaseInsensitive) == 0){
                outPdf = QString("%1/%2/%3/oversidepage_out.pdf")
                         .arg(spoolDir,client_uuid,copy_num);
                QFile::copy(backPdf,outPdf);
            }
            if ( page_type.compare("t_lastpage",Qt::CaseInsensitive) == 0){
                outPdf = QString("%1/%2/%3/lastpage_out.pdf")
                         .arg(spoolDir,client_uuid,copy_num);
                QFile::copy(backPdf,outPdf);
            }

        }
    }

    if (prn_mode == VPrn::pre_ClearPrintMode ){
        c_data->setConvertMode(true);
    }else{
        c_data->setConvertMode(false);
    }    
}

void GS_plugin::convertPdfToPng(const QString &client_uuid,
                                const QStringList &files)
{
    // Поиск данных для клиента
    ClientData *c_data = findClientData(client_uuid);

    if (files.size() == 0){
        emit error (VPrn::FileNotFound,
                    QObject::trUtf8("Ошибка при поиске файлов для преобразования, ничего не найденно !\n%1")
                    .arg(QString(Q_FUNC_INFO))
                    );
        return;
    }
    if (c_data){
        QString file_name;
        for (int i=0; i<files.size();i++){
            //gs -q -sDEVICE=png16m -dBATCH -dNOPAUSE -dFirstPage=1 -dLastPage=5 -r300 -sOutputFile=out_%d.png in.pdf
            file_name = files.at(i);

            QRegExp rx("/(.+)/(.+)/(.-copy)/(.+)_out.pdf");
            if (rx.indexIn(file_name) != -1){
                // Наш файлик можно обрабатывать
                QString copy_num = rx.cap(3);
                QString page_type  = rx.cap(4);

                c_data->startConverPdf2Png( i );
                QStringList args;
                args.append("-q");
                args.append("-dQUIET");
                args.append("-dNOPAUSE");
                args.append("-dBATCH");
                args.append("-dPARANOIDSAFER");
                ///@todo Добавить вычисление разрешения max (106*72/max_x,106*72/max_y)

                args.append("-r150");
                args.append("-sDEVICE=png16m");
                args.append("-dFirstPage=1");
                //Формируем имя файлов для предпросмотра
                if (!c_data->getConvertMode()){
                    args.append("-dLastPage=1");
                    args.append(QString("-sOutputFile=%1/%2/%3/%4_0.png")
                                .arg(spoolDir,client_uuid,copy_num,page_type));
                }else{
                    args.append(QString("-sOutputFile=%1/%2/%3/%4_\%d.png")
                                .arg(spoolDir,client_uuid,copy_num,page_type));
                }
                args.append(file_name);
                start_proc(client_uuid,gsBin,args,VPrn::job_ConvertToPng);
            }else{
                emit error (VPrn::FileIOError,
                            QObject::trUtf8("Ошибка в имени файла, при конвертации в pdf -> png!\n%1").arg(QString(Q_FUNC_INFO))
                            );

            }
        }
    }
}

void GS_plugin::setConvertToPngMode(const QString &client_uuid, bool full_doc )
{
    ClientData *c_d(0);
    c_d = findClientData (client_uuid);
    if (c_d){
        c_d->setConvertMode(full_doc);
    }
}

QString GS_plugin::getUuid() const
{
    return QUuid::createUuid().toString().replace("{","").replace("}","");
}

void GS_plugin::createClientData(const QString &client_uuid)
{
    /**
    * @todo Кто будет реагировать на ошибку и когда ?
    * проверить что вызвающая функция сумеет обработать ситуацию когда не удалось создать
    * каталоги для клиента
    **/

    ClientData *c_d(0);
    c_d = findClientData (client_uuid);
    QDir w_dir;

    QString wrk_dir =QString("%1/%2").arg(this->spoolDir,client_uuid);
    bool Ok = true;
    {
        if (c_d){
            //w_dir = QDir(wrk_dir);

            //if ( w_dir.exists() ){
            //  recursiveDeletion( wrk_dir );
            //}

            // Формируем требуемые каталоги
            Ok &= w_dir.mkpath(wrk_dir);
            for (int i=1; i<6;i++){
                wrk_dir = QString("%1/%2/%3-copy")
                          .arg(this->spoolDir,client_uuid)
                          .arg(i,0,10);

                if ( QDir(wrk_dir).exists() ){
                    recursiveDeletion( wrk_dir );
                }

                Ok &= w_dir.mkpath(wrk_dir);
                if (!Ok){
                    emit error(VPrn::FileIOError,
                               QObject::trUtf8("Ошибка создания рабочего каталога для клиента! %1\n%2")
                               .arg(wrk_dir).arg(QString(Q_FUNC_INFO))
                               );
                    return;
                }
            }
        }else{
            Ok &=false;
            emit error(VPrn::InternalAppError,
                       QObject::trUtf8("Ошибка создания внутренней учетной записи для клиента!\n%1")
                       .arg(QString(Q_FUNC_INFO))
                       );

        }
    }
}

void GS_plugin::deleteClientData(const QString &client_uuid)
{
    clients_data.remove(client_uuid);
}


//---------------------------- PRIVATE SLOTS --------------------------------------------------
void GS_plugin::threadFinish(const QString &jobKey,int code,
                             const QString &output_message)
{

    QString m_client_uuid = clients_list.take(jobKey);
    VPrn::Jobs m_job = jobs_list.take(jobKey);
    // Поиск данных для клиента
    ClientData *c_data = findClientData(m_client_uuid);

    if (code != 0 ){
        //Задача завершена с ошибкой, доложим наверх
        ///@todo переделать на сообщение об ошибке
        emit jobFinish(m_client_uuid, m_job,code,output_message);
        return;
    }

    // Формируем пути для файлов
    QString mainPDF       = c_data->getMainFile();

    // Посмотрим чем же таким важным был занят клиент
    QRegExp rx;
    switch (m_job){
    case VPrn::job_ConvertToPng:
        if ( c_data->isFinishedConvertPdf2Png() ){
			//Исходные файлы преобразовались в png оъединим лицевые страницы
			catFirstPages( m_client_uuid );
            emit docReady4preview( m_client_uuid );
        }
        break;
    case VPrn::job_CatPages:
        if (c_data->isFinishedCat() ){
            emit docReady4print( m_client_uuid );
        }
        break;
    case VPrn::job_MergePdf:
        if (c_data->isFinishedMerge() ){
            // Все варианты документа объеденнены с шаблоном отправим сигнал
            QStringList out_list = this->findFiles(m_client_uuid, QStringList()
                                                   << "*_out.pdf"
                                                   );
            convertPdfToPng(m_client_uuid,out_list);            
        }
        break;
    case VPrn::job_PrintFile:
        emit filePrinted(m_client_uuid);
        break;
    case VPrn::job_ConvertPs2Pdf: // Завершилась задача конвертирования ps в
        emit docConvertedToPdf(m_client_uuid);
        calcPageCount(m_client_uuid,mainPDF);
        break;
    case VPrn::job_SplitPageFirst:
        break;
    case VPrn::job_SplitPageOther:
        if ( !c_data->getOtherPage().isEmpty()  && c_data->getPageCount() > 1  ){
            emit docReady4work(m_client_uuid,c_data->getPageCount());
        }
        break;
    case VPrn::job_CalcPageCount: // Завершилась задача получения dump_info из pdf документа
        int pagesCount; // Число страниц в конвертируемом документе
        if (!output_message.isEmpty()) {
            rx.setPattern("NumberOfPages:\\s+(\\d{1,}).*");
            //rx.setMinimal(true);
            if (rx.indexIn(output_message) != -1) {
                bool ok;
                qDebug() << Q_FUNC_INFO << rx.cap(0) << rx.cap(1);
                pagesCount = rx.cap(1).toInt(&ok, 10);

                if (ok && pagesCount >0 ){
                    c_data->setPageCount(pagesCount);

                    c_data->setFirstPage( QObject::trUtf8("%1/%2/firstpage.pdf")
                                          .arg( spoolDir, m_client_uuid ) );
                    if ( pagesCount ==1){
                        if ( QFile::exists(c_data->getFirstPage()) ){
                            QFile::remove( c_data->getFirstPage() );
                        }
                        if ( !QFile::copy(mainPDF,c_data->getFirstPage() )){
                            emit error(VPrn::FileIOError,
                                       QObject::trUtf8("Ошибка копирования документа PDF из %1 в %2\n%3")
                                       .arg(mainPDF,c_data->getFirstPage().arg(QString(Q_FUNC_INFO)) )
                                       );
                        }else{
                            emit docReady4work(m_client_uuid,pagesCount);
                        }
                    }else{
                        c_data->setOtherPage( QObject::trUtf8("%1/%2/otherpage.pdf")
                                              .arg( spoolDir, m_client_uuid) );
                        // Делим файлы на 2 части
                        splitPdf(m_client_uuid,c_data->getMainFile(),
                                 c_data->getFirstPage(),
                                 c_data->getOtherPage());
                    }
                }else{
                    emit error(VPrn::InternalAppError,
                               QObject::trUtf8("Ошибка разбора документа PDF %1\n%2")
                               .arg(mainPDF).arg(QString(Q_FUNC_INFO))
                               );
                }

            }
        }
        break;
    }

}

//----------------------------------- PRIVATE ---------------------------------------------

void GS_plugin::start_proc(const QString &client_uuid,const QString &bin,
                           QStringList &arg_list,VPrn::Jobs job_id)
{
    // Формируем ключ и запишем данные в списки
    QString j_key = this->getUuid();
    clients_list.insert(j_key,client_uuid);
    jobs_list.insert(j_key,job_id);

    ProcessT *proc = new ProcessT(this,j_key);
    // Обработчик сообщений файлового потока
    connect(proc, SIGNAL(jobFinish     (const QString &,int,const QString &)),
            this, SLOT  (threadFinish  (const QString &,int,const QString &))
            );
    proc->addToEnv(myEnv);
    proc->execute(bin, arg_list,QProcess::MergedChannels);
}

void GS_plugin::calcPageCount(const QString &client_uuid,const QString &input_fn)
{
    ClientData *c_d(0);
    if (input_fn.isEmpty()){
        c_d = findClientData (client_uuid);
        emit docReady4work(client_uuid,c_d->getPageCount());
    }else{
        //pdfTk input_file dump_data
        QStringList args;
        args.append(input_fn);
        args.append("dump_data");
        start_proc(client_uuid,pdftkBin,args,VPrn::job_CalcPageCount);
    }
}



void GS_plugin::splitPdf(const QString &client_uuid,const QString &main_pdf,
                         const QString &first_page, const QString &other_pages)
{
    //pdftk %1 cat %2 output page_%2.pdf
    // Так как сюда попадаем только только если в документе больше 1 страницы,
    // То запускаем 2 потока формирирование 1 стр, и формирование последующих

    // Первую страницу
    QStringList args;
    args.append(main_pdf);
    args.append("cat");
    args.append("1");
    args.append("output");
    args.append(first_page);

    start_proc(client_uuid,pdftkBin,args,VPrn::job_SplitPageFirst);

    args.clear();
    args.append(main_pdf);
    args.append("cat");
    args.append("2-end");
    args.append("output");
    args.append(other_pages);

    start_proc(client_uuid,pdftkBin,args,VPrn::job_SplitPageOther);
}

void GS_plugin::mergePdf(const QString &client_uuid,const QString &in_pdf,
                         const QString &back_pdf,const QString &out_pdf)
{
    //pdftk in.pdf background back.pdf output out.pdf
    if (!QFile::exists(in_pdf)) {
        emit error(VPrn::FileIOError,
                   QObject::trUtf8("ERROR : Файл %1 не найден\n%2").arg(in_pdf).arg(QString(Q_FUNC_INFO)));
        return;
    }

    if (!QFile::exists(back_pdf)) {
        emit error(VPrn::FileIOError,
                   QObject::trUtf8("ERROR : Файл %1 не найден\n%2").arg(back_pdf).arg(QString(Q_FUNC_INFO)));
        return;
    }

    QStringList args;
    args.append(in_pdf);
    args.append("background");
    args.append(back_pdf);
    args.append("output");
    args.append(out_pdf);

    start_proc(client_uuid,pdftkBin,args,VPrn::job_MergePdf);
}

void GS_plugin::catFirstPages(const QString &client_uuid )
{    
    // Поиск данных для клиента
    ClientData *c_data = findClientData(client_uuid);
    if (c_data){

        // Запуск процесса объединения
        for (int i=1;i<6;i++){
            //Список файлов i экземпляра
            QStringList out_list = findFiles4Copy(client_uuid,i,
                                                  QStringList()<<"firstpage_out.pdf"
                                                  <<"otherpage_out.pdf"
                                                  );
            QString fileA;
            QString fileB;
            QString outFile = QString("%1/%2/%3-copy/face_pages_out.pdf")
                              .arg(spoolDir,client_uuid).arg(i,0,10);

            for (int j=0;j<out_list.size();j++){
                QString line = out_list.at(j);
                QRegExp rx("/(.+)/(.+)/(.-copy)/(.+)_out.pdf");
                if (rx.indexIn(line) != -1){
                    // Наш файлик можно обрабатывать
                    QString copy_num = rx.cap(3);
                    QString page_type  = rx.cap(4);
                    if (page_type.compare("firstpage",Qt::CaseInsensitive) == 0){
                        fileA = line;
                    }
                    if (page_type.compare("otherpage",Qt::CaseInsensitive) == 0){
                        fileB = line;
                    }
                }
            }
            if ( c_data->getPageCount() >1  && !fileA.isEmpty() && !fileB.isEmpty()){
                //Объединим файлы
                c_data->startCat( i );
                catPdf(client_uuid,fileA,fileB,outFile);
            }else{
                // Переименуем
                if (!fileA.isEmpty()){
                    QFile::rename(fileA,outFile);
                }
            }
        }
        if ( c_data->getPageCount() == 1){
            emit docReady4print( client_uuid );
        }
    }else{
        emit error(VPrn::InternalAppError,
                   QObject::trUtf8("Ошибка поиска данных клиента!\n%1")
                   .arg(QString(Q_FUNC_INFO)));
    }
}

ClientData * GS_plugin::findClientData(const QString &client_uuid)
{
    ClientData *c_d(0);

    QMap<QString,ClientData *>::const_iterator i = clients_data.find(client_uuid);
    if ( i == clients_data.end() ){
        clients_data.insert(client_uuid,new ClientData());
    }
    c_d = clients_data.value(client_uuid);
    return c_d;
}

QStringList GS_plugin::findFiles(const QString &client_uuid,
                                 const QStringList &filters)
{
    return QStringList() << findFiles4Copy(client_uuid,1,filters)
            << findFiles4Copy(client_uuid,2,filters)
            << findFiles4Copy(client_uuid,3,filters)
            << findFiles4Copy(client_uuid,4,filters)
            << findFiles4Copy(client_uuid,5,filters);
}

QStringList GS_plugin::findFiles4Copy(const QString &client_uuid, /*ID клиента*/
                                      int copyNum, /*Номер экземпляра документа*/
                                      const QStringList &filters /*Тип файлов*/
                                      )
{
    QStringList files;
    QDir dir;
    if (copyNum >0  && copyNum <6){
        // Формируем списк файлов для конкретного экз. документа
        dir = QDir(QString ("%1/%2/%3-copy")
                   .arg(spoolDir,client_uuid)
                   .arg(copyNum,0,10) );
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

void GS_plugin::recursiveDeletion(QString path)
{
    QDir dir(path);
    QStringList files = dir.entryList(QDir::Files);

    QStringList::Iterator itFile = files.begin();
    while (itFile != files.end())
    {
        QFile file(path + "/" + *itFile);
        if ( !file.remove()){
            emit error(VPrn::FileIOError,
                       QObject::trUtf8("Ошибка удаления файла %1!\n%2").arg(*itFile).arg(QString(Q_FUNC_INFO)));
        }
        ++itFile;
    }

    QStringList dirs = dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot);
    QStringList::Iterator itDir = dirs.begin();
    while (itDir != dirs.end())
    {
        recursiveDeletion(path + "/" + *itDir);
        ++itDir;
    }

    if ( !dir.rmdir(path)){

        emit error(VPrn::FileIOError,
                   QObject::trUtf8("Ошибка удаления каталога %1!\n%2").arg(path).arg(QString(Q_FUNC_INFO)));

    }
}

void GS_plugin::createRCPfile(const QString &tmp_dir)
{
    gs_rcp=QString("%1/%2.rcp").arg(tmp_dir,getUuid());
    QFile new_file;

    new_file.setFileName(gs_rcp);
    if (new_file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&new_file);
        out     << QObject::trUtf8("-dQUIET\n")
                << QObject::trUtf8("-dNOPAUSE\n")
                << QObject::trUtf8("-dPARANOIDSAFER\n")
                << QObject::trUtf8("-dBATCH\n")
                << QObject::trUtf8("-r600\n")
                << QObject::trUtf8("-dPDFSETTINGS=/prepress\n")
                << QObject::trUtf8("-dEmbedAllFonts=true\n")
                << QObject::trUtf8("-sDEVICE=pdfwrite\n");
        new_file.close();
    }else{
        emit error(VPrn::FileIOError,
                   QObject::trUtf8("Ошибка создания файла %1\nОшибка %2\n%3")
                   .arg(gs_rcp)
                   .arg(new_file.errorString())
                   .arg(QString(Q_FUNC_INFO)));
        emit pluginNotReady();
    }
}



Q_EXPORT_PLUGIN2(Igs_plugin, GS_plugin);
