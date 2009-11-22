#include "gs_plugin.h"

bool gs_plugin::init(const QString &gs_bin, const QString &pdftk_bin, const QString &temp_folder,const QString &sid)
{
    QString error_msg;
    QDir dir;
    const QString startnow = QDir::currentPath();

    if (QFile::exists(gs_bin)) {
        // Проверим факт существования по указанному пути бинарника ghostscript
        gsBin = gs_bin;
        if (!sid.isEmpty()) {
            Sid = sid;// Запомним уникальный номер
        }
        else {
            error_msg = QObject::trUtf8("ERROR : Ошибка генерации уникального номера\n");
        }
        if (dir.cd(temp_folder) && !temp_folder.isEmpty()) {
            // Проверим факт существования временного каталога
            tempPath = temp_folder;
            if (QFile::exists(pdftk_bin)) {
                // Проверим факт существования по указанному пути бинарника grep
                pdftkBin = pdftk_bin;
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

QString gs_plugin::startProc(const QString &bin, const QStringList &proc_args)
{
    QString proc_output;
    QString error_msg;

    QProcess *process = new QProcess(NULL);
    process->setReadChannelMode(QProcess::MergedChannels);
    process->start(bin, proc_args, QIODevice::ReadOnly);
    if (process->waitForFinished()) {
        proc_output = process->readAll().trimmed();
    }
    else {
        error_msg=QObject::trUtf8("ERROR :  Ошибка запуска дочернего процесса %1\nс аргументами %2\n").arg(bin).arg(proc_args.join(" "));
        emit error(error_msg);
    }
    return proc_output;
}

void gs_plugin::convertPs2Pdf(const QString &input_fn)
{
    QString error_msg;
    QString mainPDF = QObject::trUtf8("%1/%2_main.pdf").arg(this->tempPath, this->Sid);
    firstPage_fn = QObject::trUtf8("%1/%2_first.pdf").arg(this->tempPath, this->Sid);
    otherPages_fn = QObject::trUtf8("%1/%2_other.pdf").arg(this->tempPath, this->Sid);

    // Проверим фактик наличия входного файла
    if (QFile::exists(input_fn)) {
        args.clear();
        args.append("-q");
        args.append("-dQUIET");
        args.append("-dNOPAUSE");
        args.append("-dPARANOIDSAFER");
        args.append("-dBATCH");
        args.append("-r600");
        args.append("-dPDFSETTINGS=/prepress");
        args.append("-dEmbedAllFonts=true");
        args.append("-sDEVICE=pdfwrite");
        args.append(QString("-sOutputFile=%1").arg(mainPDF));
        args.append("-c");
        args.append(".setpdfwrite");
        args.append("-f");
        args.append(input_fn);

        //Start convert process
        this->startProc(gsBin, args);

        if (QFile::exists(mainPDF)) {
            //вызовем pdfTk для получения числа страниц в документе ...
            pagesCount = this->pageCount(mainPDF);
            switch (pagesCount) {
            case 0:
                error_msg= QObject::trUtf8("ERROR :  Ошибка разбора документа PDF %1").arg(mainPDF);
                break;
            case 1:
                QFile::rename(mainPDF, firstPage_fn);
                break;
            default:
                // Делим файлы на 2 части
                this->splitPdf(mainPDF, this->firstPage_fn, this->otherPages_fn);
                break;
            }
        }
        else {
            error_msg = QObject::trUtf8("ERROR : Ошибка преобразования исходного документа в PDF\n");
        }
    }else {
        error_msg = QObject::trUtf8("ERROR : Файл %1 не найден\n").arg(input_fn);
    }
    if (!error_msg.isEmpty()) {
        emit error(error_msg);
    }else {
        emit StateChanged(converted);
    }
}

void gs_plugin::splitPdf(const QString &source_fn, const QString &firts_page_fn,const QString &other_page_fn)
{
    QString error_msg;

    //pdftk %1 cat %2 output page_%2.pdf
    // Первую страницу
    args.clear();
    args.append(source_fn);
    args.append("cat");
    args.append("1");
    args.append("output");
    args.append(firts_page_fn);
    this->startProc(pdftkBin, args);
    if (!QFile::exists(firts_page_fn)){
        error_msg=QObject::trUtf8("ERROR : Первую страницу документа не удалось создать\n");
    }
    args.clear();
    args.append(source_fn);
    args.append("cat");
    args.append(QString("%1-%2").arg(2, this->pagesCount));
    args.append("output");
    args.append(other_page_fn);
    this->startProc(pdftkBin, args);
    if (!QFile::exists(other_page_fn)){
           error_msg=QObject::trUtf8("ERROR : Вторую и последующие страницы документа не удалось создать\n");
    }
    if (!error_msg.isEmpty()) {
           emit error(error_msg);
    }else {
           emit StateChanged(splitted);
    }
}

int gs_plugin::pageCount(const QString &input_fn)
{
    QString error_msg;
    QRegExp rx;
    int page_count;
    //pdfTk input_file dump_data
    args.clear();
    args.append(input_fn);
    args.append("dump_data");

    QString res = this->startProc(this->pdftkBin, args);
    if (!res.isEmpty()) {
        // разберем по кирпичику
        //rx.setPattern(".*\nNumberOfPages:\s+(\d{1,}).*$");
        rx.setMinimal(true);
        if (rx.indexIn(res) != -1) {
            bool ok;
            int page_count = rx.cap(1).toInt(&ok, 10);
            if (!ok) {
                 page_count =0;
            }

        }
    }
    return page_count;
}

void gs_plugin::merge2Pdf(const QString &input_fn, const QString &background_fn,const QString &output_fn)
{
    //pdftk in.pdf background back.pdf output out.pdf
    QString error_msg;
    if (!QFile::exists(input_fn)) {
        error_msg = QObject::trUtf8("ERROR : Первую страницу документа не удалось создать\n");
    }
    else if (!QFile::exists(background_fn)) {
        error_msg = QObject::trUtf8("ERROR : Первую страницу документа не удалось создать\n");
    }
    else {
        args.clear();
        args.append(input_fn);
        args.append("background");
        args.append(background_fn);
        args.append("output");
        args.append(output_fn);
        this->startProc(pdftkBin, args);
        if (!QFile::exists(output_fn)) {
            error_msg = QObject::trUtf8("ERROR : Ошибка наложения шаблона %1\n на документ %2\n").arg(background_fn,input_fn);
        }
    }
    if (!error_msg.isEmpty()) {
        emit error(error_msg);
    }
    else {
        emit StateChanged(merged);
    }
}

void gs_plugin::addPdfMark(const QString &input_fn,const QStringList mark)
{

}
void gs_plugin::printPdf(const QString &print_fn, const QString &printer_name)
{

}
QPixmap gs_plugin::convertPdf2Png(const QString &fn, int beginPage, int Width)
{
    return pageSnapShot;
}

Q_EXPORT_PLUGIN2(Igs_plugin, gs_plugin)
;
