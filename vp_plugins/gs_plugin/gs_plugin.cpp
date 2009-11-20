#include "gs_plugin.h"


void gs_plugin::init(const QString &gs_bin,const QString &grep_bin,const QString &temp_folder,const QString &sid)
{
    QString error_msg;
    QDir dir;
    const QString startnow = QDir::currentPath();

    if (QFile::exists(gs_bin)){
        // Проверим факт существования по указанному пути бинарника ghostscript
        this->gsBin=gs_bin;
        if (!sid.isEmpty()){
            this->Sid=sid;// Запомним уникальный номер
        }else{
            error_msg=QObject::trUtf8("Error:Ошибка генерации уникального номера\n");
        }
        if (dir.cd(temp_folder) && !temp_folder.isEmpty() ){
            // Проверим факт существования временного каталога
            this->tempPath=temp_folder;
            if (QFile::exists(grep_bin)){
                // Проверим факт существования по указанному пути бинарника grep
                this->grepBin=grep_bin;
            }else{
                error_msg=QObject::trUtf8("Error:Файл %1 не найден\n").arg(grep_bin);
            }
        }else{
            error_msg=QObject::trUtf8("Error:каталог %1 не найден\n").arg(temp_folder);
        }
    }else{
        error_msg=QObject::trUtf8("Error:Файл %1 не найден\n").arg(gs_bin);
    }
    if (!error_msg.isEmpty()){
        emit error(error_msg);
    }else{
        emit StateChanged(gsPluginLoaded);
    }
    QDir::setCurrent(startnow);
}

int gs_plugin::pageCount(QString &input_fn)
{
   QString error_msg;
   int count=0;
   if (QFile::exists(input_fn)){// Проверим фактик наличия входного файла
       this->args.clear();
       this->args.append("-c");
       this->args.append(QString("\%\%Page: "));
       this->args.append(input_fn);

       QProcess *process = new QProcess(NULL);
       process->setReadChannelMode(QProcess::MergedChannels);
       process->start(this->grepBin,  args , QIODevice::ReadOnly );
       if (process->waitForFinished()) {
           QString proc_output = process->readAll().trimmed();
           bool ok;
           count =proc_output.toInt(&ok,10);
           if (!ok){
              error_msg=QObject::trUtf8("Error:В исходном документе количество страниц = 0\n");
           }
       }else{
           error_msg=QObject::trUtf8("Error:Ошибка получения количества страниц в исходном документе %1\n").arg(input_fn);
       }
   }else{
       error_msg=QObject::trUtf8("Error:Файл %1 не найден\n").arg(input_fn);
   }
   if (!error_msg.isEmpty()){
       emit error(error_msg);
   }

   return count;
}

void gs_plugin::convertPs2Pdf(QString &input_fn)
{
    QString error_msg;
    firstPage_fn=QObject::trUtf8("%1/%2_first.pdf").arg(this->tempPath,this->Sid);
    otherPages_fn=QObject::trUtf8("%1/%2_other.pdf").arg(this->tempPath,this->Sid);

    if (QFile::exists(input_fn)){// Проверим фактик наличия входного файла
        this->PageCountInDoc=this->pageCount(input_fn);
        if(this->PageCountInDoc>0){
            // Формируем pdf первой страницы
            this->args.clear();
            args.append("-q");
            args.append("-dQUIET");
            args.append("-dNOPAUSE");
            args.append("-dPARANOIDSAFER");
            args.append("-dBATCH");
            args.append("-r600");
            args.append("-dPDFSETTINGS=/prepress");
            args.append("-dEmbedAllFonts=true");
            args.append("-sDEVICE=pdfwrite");
            args.append("-dFirstPage=1");
            args.append("-dLastPage=1");
            args.append(QString("-sOutputFile=%1").arg(firstPage_fn));
            args.append("-c");
            args.append(".setpdfwrite");
            args.append("-f");
            args.append(input_fn);

            if (this->callGS(args)==0){
                if (this->PageCountInDoc>1){
                    // Формируем pdf последующих страниц
                    this->args.clear();
                    args.append("-q");
                    args.append("-dQUIET");
                    args.append("-dNOPAUSE");
                    args.append("-dPARANOIDSAFER");
                    args.append("-dBATCH");
                    args.append("-r600");
                    args.append("-dPDFSETTINGS=/prepress");
                    args.append("-dEmbedAllFonts=true");
                    args.append("-sDEVICE=pdfwrite");
                    args.append("-dFirstPage=2");
                    args.append(QString("-dLastPage=%1").arg(this->PageCountInDoc));
                    args.append(QString("-sOutputFile=%1").arg(otherPages_fn));
                    args.append("-c");
                    args.append(".setpdfwrite");
                    args.append("-f");
                    args.append(input_fn);
                    if (this->callGS(args)!=0){
                        error_msg=QObject::trUtf8("Error:Ошибка преобразования последующих страниц документа в PDF\n");
                    }
                }
            }else{
                error_msg=QObject::trUtf8("Error:Ошибка преобразования первой страницы документа в PDF\n");
            }
        }
    }else{
        error_msg=QObject::trUtf8("Error:Файл %1 не найден\n").arg(input_fn);
    }
    if (!error_msg.isEmpty()){
        emit error(error_msg);
    }else{
        emit StateChanged(converted);
    }
}

QPixmap gs_plugin::convertPdf2Png(QString &fn, int beginPage,int lastPage, int Width)
{
    return pageSnapShot;
}

void gs_plugin::merge2Pdf(QString &input_fn,QString &background_fn)
{

}

void gs_plugin::printPdf(QString &print_fn,QString &printer_name)
{

}

int gs_plugin::callGS(const QStringList &gs_args)
{
   const QString startnow = QDir::currentPath();
   int res;
   QDir::setCurrent(this->tempPath);
   QProcess *process = new QProcess(NULL);
   process->setReadChannelMode(QProcess::MergedChannels);
   process->start(this->gsBin,  args , QIODevice::ReadOnly );
   if (!process->waitForFinished()) {
       res = -1;
   }else{
       QString ghostcomment = process->readAll().trimmed();
       //qDebug() << "ghostcomment-> " << ghostcomment;
       res = 0;
   }

   QDir::setCurrent(startnow);
   return res;
}




Q_EXPORT_PLUGIN2(Igs_plugin, gs_plugin);
