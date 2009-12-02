#include "gs_plugin.h"



GS_plugin::GS_plugin(QObject *parent)
{

}
bool GS_plugin::init(const QString &gs_bin, const QString &pdftk_bin, const QString &temp_folder,const QString &gs_rcp_file,const QString &sid)
{
    QString error_msg;
    QDir dir;
    const QString startnow = QDir::currentPath();

    if (QFile::exists(gs_bin)) {
        // Проверим факт существования по указанному пути бинарника ghostscript
        gsBin = gs_bin;
        if (!sid.isEmpty()) {
            Sid = sid;// Запомним уникальный номер
            if (dir.cd(temp_folder) && !temp_folder.isEmpty()) {
                // Проверим факт существования временного каталога
                tempPath = temp_folder;
                // Формируем пути для файлов
                mainPDF = QObject::trUtf8("%1/%2_main.pdf").arg(this->tempPath, this->Sid);
                firstPage_fn = QObject::trUtf8("%1/%2_first.pdf").arg(this->tempPath, this->Sid);
                otherPages_fn = QObject::trUtf8("%1/%2_other.pdf").arg(this->tempPath, this->Sid);
                pdf2png_page = QObject::trUtf8("%1/%2_page.png").arg(this->tempPath, this->Sid);
                if (QFile::exists(pdftk_bin)) {
                    // Проверим факт существования по указанному пути бинарника grep
                    pdftkBin = pdftk_bin;
                    if (!QFile::exists(gs_rcp_file)){
                        // Файл не существует но он мне нужен значит создаем его
                        QTemporaryFile file_rcp;
                        if (file_rcp.open()) {
                            QTextStream out(&file_rcp);
                            out << QObject::trUtf8("-dQUIET\n")
                                    << QObject::trUtf8("-dNOPAUSE\n")
                                    << QObject::trUtf8("-dPARANOIDSAFER\n")
                                    << QObject::trUtf8("-dBATCH\n")
                                    << QObject::trUtf8("-r600\n")
                                    << QObject::trUtf8("-dPDFSETTINGS=/prepress\n")
                                    << QObject::trUtf8("-dEmbedAllFonts=true\n")
                                    << QObject::trUtf8("-sDEVICE=pdfwrite\n");
                            file_rcp.close();
                            gs_rcp=QString("%1//%2.rcp").arg(temp_folder,sid);
                            file_rcp.rename(file_rcp.fileName(),gs_rcp);
                        }
                    }else{
                        gs_rcp=gs_rcp_file;
                    }
                }
                else {
                    error_msg = QObject::trUtf8("ERROR : Файл %1 не найден\n").arg(pdftk_bin);
                }
            }
            else {
                error_msg = QObject::trUtf8("ERROR : каталог %1 не найден\n").arg(temp_folder);
            }
        }
        else {
            error_msg = QObject::trUtf8("ERROR : Уникальный номер не может быть пустым\n");
        }
    }
    else {
        error_msg = QObject::trUtf8("ERROR : Файл %1 не найден\n").arg(gs_bin);
    }

    QDir::setCurrent(startnow);

    if (!error_msg.isEmpty()) {
        emit error(error_msg);
        return false;
    }
    else {
        return true;
    }
}
void GS_plugin::convertPs2Pdf(const QString &input_fn)
{
    /*
      Конвертация происходит в 3 этапа:
      1 - перевод файла в pdf
      2 - определение количества страниц в полученном файле
      3 - разбиение полученного файла на первую и последующие страницы
    */
    // Проверим фактик наличия входного файла
    qDebug() << Q_FUNC_INFO << input_fn;
    if (QFile::exists(input_fn)) {
        ProcessT *proc = new ProcessT();
        // Обработчик сообщений файлового потока
        connect(proc,SIGNAL(commandOutput(int,QString)),this,SLOT(parseCnvThread(int,QString)));

        // Формируем строку команды для первого этапа
        args.clear();
        args.append("-q" );
        args.append(QString("@%1").arg(gs_rcp));
        args.append(QString("-sOutputFile=%1").arg(mainPDF));
        args.append("-c");
        args.append(".setpdfwrite");
        args.append("-f");
        args.append(input_fn);
        proc->execute(gsBin, args,QProcess::MergedChannels);
    }else {
        QString e_msg = QObject::trUtf8("ERROR : Файл %1 не найден\n").arg(input_fn);
        emit error(e_msg);
    }
}
void GS_plugin::splitPdf(const QString &source_fn, const QString &firts_page_fn,const QString &other_page_fn)
{
    //pdftk %1 cat %2 output page_%2.pdf
    // Первую страницу
    args.clear();
    args.append(source_fn);
    args.append("cat");
    args.append("1");
    args.append("output");
    args.append(firts_page_fn);
    ProcessT *proc_first = new ProcessT();
    // Обработчик сообщений файлового потока
    connect(proc_first,SIGNAL(commandOutput(int,QString)),this,SLOT(parseFirstPageThread(int,QString)));
    proc_first->execute(pdftkBin, args,QProcess::MergedChannels);

    if (pagesCount>1){
        args.clear();
        args.append(source_fn);
        args.append("cat");
        args.append("2-end");
        args.append("output");
        args.append(other_page_fn);
        ProcessT *proc_other = new ProcessT();
        // Обработчик сообщений файлового потока
        connect(proc_other,SIGNAL(commandOutput(int,QString)),this,SLOT(parseOtherPageThread(int,QString)));
        proc_other->execute(pdftkBin, args,QProcess::MergedChannels);
    }
}
void GS_plugin::getPageCount(const QString &input_fn)
{
    //pdfTk input_file dump_data
    args.clear();
    args.append(input_fn);
    args.append("dump_data");

    ProcessT *proc = new ProcessT();
    // Обработчик сообщений файлового потока
    connect(proc,SIGNAL(commandOutput(int,QString)),this,SLOT(parsePageCountThread(int,QString)));
    proc->execute(pdftkBin, args,QProcess::MergedChannels);
}
void GS_plugin::merge2Pdf(const QString &input_fn, const QString &background_fn,const QString &output_fn)
{//pdftk in.pdf background back.pdf output out.pdf
    QString e_msg;
    if (!QFile::exists(input_fn)) {
        e_msg =QObject::trUtf8("ERROR : Файл %1 не найден или отсутствуют права доступа").arg(input_fn);

    }else if (!QFile::exists(background_fn)) {
        e_msg=QObject::trUtf8("ERROR : Файл %1 не найден или отсутствуют права доступа").arg(background_fn);
    }else {
        args.clear();
        args.append(input_fn);
        args.append("background");
        args.append(background_fn);
        args.append("output");
        args.append(output_fn);
        ProcessT *proc = new ProcessT();
        // Обработчик сообщений файлового потока
        connect(proc,SIGNAL(commandOutput(int,QString)),this,SLOT(parseMergeThread(int,QString)));
        proc->execute(pdftkBin, args,QProcess::MergedChannels);
    }

    if (!e_msg.isNull()){
        emit error (e_msg);
    }
}
void GS_plugin::addPdfMark(const QString &input_fn,const QString &user_name,const QString &host_name,quint16 host_ip )
{
    QString e_msg;
    QTemporaryFile file_tmp;
    QTemporaryFile file_pdfMark;
    QFile file_in;
    if (!QFile::exists(input_fn)){
       e_msg =QObject::trUtf8("ERROR : Файл % не существует\n").arg(input_fn);
    }else{
        if (user_name.isEmpty()){
            e_msg=QObject::trUtf8("ERROR :Имя пользователя не может быть пустым");
        }else{
            // host_name  и IP адрес не проверяем или стоит все таки ?

            file_in.rename(input_fn,file_tmp.fileName());
            // создаем файл pdfMark

            if (file_pdfMark.open()) {
                QTextStream out(&file_pdfMark);
                out << QObject::trUtf8("[ /Title (Title)\n")
                    << QObject::trUtf8("/Author (%1)\n").arg(user_name)
                    << QObject::trUtf8("/Subject (%1)\n").arg(Sid)
                    << QObject::trUtf8("/Keywords (comma, separated, keywords)\n")
                    << QObject::trUtf8("/ModDate (D:%1)\n").arg(QDateTime::currentDateTime().toString("dd.mm.yyyy hh:mm:ss"))
                    << QObject::trUtf8("/CreationDate (D:%1)\n").arg(QDateTime::currentDateTime().toString("dd.mm.yyyy hh:mm:ss"))
                    << QObject::trUtf8("/Creator (Virtual Safe Printer)\n")
                    << QObject::trUtf8("/Producer (Hostname %1, IP:").arg(host_name) << host_ip << ")\n"
                    << QObject::trUtf8("/DOCINFO pdfmark\n");
                file_pdfMark.close();

                // объединим два документа
                args.clear();
                args.append(file_tmp.fileName());
                args.append("update_info");
                args.append(file_pdfMark.fileName());
                args.append("output");
                args.append(input_fn);

                ProcessT *proc = new ProcessT();
                // Обработчик сообщений файлового потока
                connect(proc,SIGNAL(commandOutput(int,QString)),this,SLOT(parseAddPdfMarkThread(int,QString)));
                proc->execute(pdftkBin, args,QProcess::MergedChannels);
            }else{
                e_msg=QObject::trUtf8("Не могу создать файл %1 мета данных").arg(file_pdfMark.fileName());
            }
        }
    }
    if (!e_msg.isNull()){
        emit error (e_msg);
    }
}
void GS_plugin::printPdf(const QString &print_fn, const QString &printer_name)
{ //-q -dQUIET -dNOPAUSE -dPARANOIDSAFER -dBATCH -r300 -sDEVICE=pdfwrite -sOutputFile="%%printer%%pdfcreator" -c .setpdfwrite -f %1

    args.clear();
    args.append("-q");
    args.append("-dQUIET");
    args.append("-dNOPAUSE");
    args.append("-dBATCH");
    args.append("-r600");
    args.append("-sDEVICE=pdfwrite");
    args.append(QString("-sOutputFile=\"\%printer\%%1\"").arg(printer_name));
    args.append("-c");
    args.append(".setpdfwrite");
    args.append("-f");
    args.append(print_fn);
    ProcessT *proc = new ProcessT();
    // Обработчик сообщений файлового потока
    //connect(proc,SIGNAL(commandOutput(int,QString)),this,SLOT(parsePrintThread(int,QString)));
    proc->execute(gsBin, args,QProcess::MergedChannels);

}

// ********************* Private SLOTS *******************************************************

void GS_plugin::parseCnvThread(int Code,QString output )
{
    QString msg;
    if (Code != QProcess::NormalExit) {
        msg =QObject::trUtf8("Ошибка предпечатной подготовки.\n%1:\nКод %2").arg(output).arg(Code,0,10);
        emit error(msg);
    }else{
        // Файл преобразован в pdf
        emit taskStateChanged(converted);
        //вызовем pdfTk для получения числа страниц в документе ...
        this->getPageCount(mainPDF);
    }
}

void GS_plugin::parseFirstPageThread(int Code,QString output )
{
    QString msg;
    if (Code != QProcess::NormalExit) {
        msg =QObject::trUtf8("ERROR : Первую страницу документа не удалось создать\n%1:\nКод %2").arg(output).arg(Code,0,10);
        emit error(msg);
    }else{
        emit taskStateChanged(splitted_first);
    }
}

void GS_plugin::parseOtherPageThread(int Code,QString output )
{
    QString msg;
    if (Code != QProcess::NormalExit) {
        msg =QObject::trUtf8("ERROR : Вторую и последующие страницы документа не удалось создать\n%1:\nКод %2").arg(output).arg(Code,0,10);
        emit error(msg);
    }else{
        emit taskStateChanged(splitted_other);
    }
}

void GS_plugin::parsePageCountThread(int Code,QString output)
{
    QString msg;
    QRegExp rx;

    if (Code != QProcess::NormalExit) {
        msg =QObject::trUtf8("Ошибка предпечатной подготовки.\n%1:\nКод %2").arg(output).arg(Code,0,10);
        emit error(msg);
    }else{
        // Определим количество страниц в документе
        if (!output.isEmpty()) {
            // разберем по кирпичику
            rx.setPattern("NumberOfPages:\\s+(\\d{1,5}).*");
            rx.setMinimal(true);
            if (rx.indexIn(output) != -1) {
                bool ok;
                this->pagesCount = rx.cap(1).toInt(&ok, 10);
                if (!ok) {
                    this->pagesCount =0;
                    emit error (msg);

                }else{
                    emit pagesInDoc(this->pagesCount);

                    switch (this->pagesCount) {
                    case 0:
                        msg = QObject::trUtf8("ERROR :  Ошибка разбора документа PDF %1").arg(mainPDF);
                        emit error (msg);
                        break;
                    case 1:
                        QFile::copy(mainPDF, firstPage_fn);
                        break;
                    default:
                        // Делим файлы на 2 части
                        this->splitPdf(mainPDF, firstPage_fn, otherPages_fn);
                        break;
                    }
                }
            }
        }
    }
}

void GS_plugin::clearAll()
{// Удаляем за собой все файлы созданные в ходе работы
    if (!QFile::exists(firstPage_fn)) {
        QFile::remove(firstPage_fn);
    }
    if (!QFile::exists(otherPages_fn)) {
        QFile::remove(otherPages_fn);
    }
    if (!QFile::exists(mainPDF)) {
        QFile::remove(mainPDF);
    }
    if (!QFile::exists(pdf2png_page)) {
        QFile::remove(pdf2png_page);
    }
}

void GS_plugin::parseMergeThread(int Code,QString output )
{
    QString msg;
    if (Code != QProcess::NormalExit) {
        msg =QObject::trUtf8("ERROR : Ошибка наложения шаблона на документ\n%1:\nКод %2").arg(output).arg(Code,0,10);
        emit error(msg);
    }else{
        emit taskStateChanged(merged);
    }
}

void GS_plugin::parseAddPdfMarkThread(int Code,QString output )
{
    QString msg;
    if (Code != QProcess::NormalExit) {
        msg =QObject::trUtf8("ERROR : Ошибка добавления meta данных в документ\n%1:\nКод %2").arg(output).arg(Code,0,10);
        emit error(msg);
    }else{
        emit taskStateChanged(pdfMarkAdded);
    }
}
void GS_plugin:: parseCnv2PngThread(int Code,QString output)
{
    QString msg;
    if (Code != QProcess::NormalExit) {
        msg =QObject::trUtf8("ERROR : Ошибка преобразования запрошенной страницы в png\n%1:\nКод %2").arg(output).arg(Code,0,10);
        emit error(msg);
    }else{
        // Читаем файлик

        if (currentPageSnapShot.load(pdf2png_page)){
            emit taskStateChanged(previewedPage);
        }else{
            msg = QObject::trUtf8("ERROR : Загрузка файл %1 не удачна\n").arg(pdf2png_page);
            emit error(msg);
        }

    }
}

//********************************************************************************************
void GS_plugin::convertPdf2Png(const QString &fn, int convertedPage)
{// -q -sDEVICE=png16m -dBATCH -dNOPAUSE -dFirstPage=1 -dLastPage=1 -r300 -sOutputFile=test.png %1
    if (QFile::exists(pdf2png_page)) {
        QFile::remove(pdf2png_page); // Удалим файлик
    }
    args.clear();
    args.append("-q");
    args.append("-sDEVICE=png16m");
    args.append("-dBATCH");
    args.append("-dNOPAUSE");
    args.append("-r300");
    args.append(QString("-dFirstPage=%1").arg(convertedPage));
    args.append(QString("-dLastPage=%1").arg(convertedPage));
    args.append(QString("-sOutputFile=%1").arg(pdf2png_page));
    args.append(fn);

    ProcessT *proc = new ProcessT();
    // Обработчик сообщений файлового потока
    connect(proc,SIGNAL(commandOutput(int,QString)),this,SLOT(parseCnv2PngThread(int,QString)));
    proc->execute(gsBin, args,QProcess::MergedChannels);
}

Q_EXPORT_PLUGIN2(Igs_plugin, GS_plugin)
;
