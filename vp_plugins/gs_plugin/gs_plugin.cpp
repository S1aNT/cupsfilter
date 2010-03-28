#include "gs_plugin.h"
#include "proc_thread.h"

#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QRegExp>
#include <QtCore/QTemporaryFile>
#include <QtCore/QTextStream>
#include <QtCore/QDateTime>


#include <QtGui/QPixmap>

GS_plugin::GS_plugin(QObject *parent)
{

}

void GS_plugin::init(const QString &gs_bin, const QString &pdftk_bin, const QString &temp_folder,const QString &sid)
{
    QString error_msg;
    QDir dir;
    QFile new_file;
    const QString startnow = QDir::currentPath();

    bool Ok = true;
    {
        Ok &= QFile::exists(gs_bin);
        if (Ok){// Проверим факт существования по указанному пути бинарника ghostscript
            gsBin = gs_bin;
            Ok &= !sid.isEmpty();
            if (Ok){
                Sid = sid;// Запомним уникальный номер
                Ok &= dir.cd(temp_folder) && !temp_folder.isEmpty();
                if (Ok){// Проверим факт существования временного каталога
                    tempPath = temp_folder;
                    myEnv << QObject::trUtf8("TMPDIR=%1").arg(temp_folder);
                    // Формируем пути для файлов
                    mainPDF       = QObject::trUtf8("%1/%2_main.pdf")
                                    .arg( this->tempPath, this->Sid );
                    firstPage_fn  = QObject::trUtf8("%1/%2_first.pdf")
                                    .arg( this->tempPath, this->Sid );
                    otherPages_fn = QObject::trUtf8("%1/%2_other.pdf")
                                    .arg( this->tempPath, this->Sid );
                    pdf2png_page  = QObject::trUtf8("%1/%2_page.png")
                                    .arg( this->tempPath, this->Sid );
                    Ok &=QFile::exists(pdftk_bin);
                    if (Ok){// Проверим факт существования по указанному пути бинарника
                        pdftkBin = pdftk_bin;
                        // Файл не существует но он мне нужен значит создаем его
                        gs_rcp=QString("%1/%2.rcp").arg(temp_folder,sid);
                        // Надо больше спать из за отсутсвия этой строчки 20 минут искал ошибку
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
                                       QObject::trUtf8("Ошибка создания файла %1\nОшибка %2")
                                       .arg(gs_rcp).arg(new_file.errorString()));
                        }
                    }else{
                        emit error(VPrn::FileIOError,
                                   QObject::trUtf8("Файл %1 не найден\n")
                                   .arg(pdftk_bin));
                    }
                }else{
                    emit error(VPrn::FileIOError,
                               QObject::trUtf8("Каталог %1 не найден\n")
                               .arg(temp_folder));
                }
            }else {
                emit error(VPrn::InternalPluginError,
                           QObject::trUtf8("Уникальный номер не может быть пустым!"));
            }
        }else {
            emit error(VPrn::FileIOError, QObject::trUtf8("Файл %1 не найден\n").arg(gs_bin));
        }
    }
    QDir::setCurrent(startnow);
}

void GS_plugin::convertPs2Pdf(const QString &client_uuid,const QString &input_fn)
{
    // Проверим фактик наличия входного файла
    qDebug() << Q_FUNC_INFO << input_fn;
    if (QFile::exists(input_fn)) {
        ProcessT *proc = new ProcessT(this,VPrn::job_ConvertPs2Pdf,client_uuid);
        // Обработчик сообщений файлового потока
        connect(proc, SIGNAL(jobFinish   (const QString &,VPrn::Jobs,int,const QString &)),
                this, SIGNAL(threadFinish(const QString &,VPrn::Jobs,int,const QString &))
                );

        // Формируем строку команды для первого этапа
        args.clear();
        args.append("-q" );
        args.append(QString("@%1").arg(gs_rcp));
        args.append(QString("-sOutputFile=%1").arg(mainPDF));
        args.append("-c");
        args.append(".setpdfwrite");
        args.append("-f");
        args.append(input_fn);
        proc->addToEnv(myEnv);
        proc->execute(gsBin, args,QProcess::MergedChannels);

    }else {
        emit error(VPrn::FileIOError,
                   QObject::trUtf8("ERROR : Файл %1 не найден\n").arg(input_fn));
    }
}

//---------------------------- PRIVATE SLOTS --------------------------------------------------
void GS_plugin::threadFinish(const QString &client_uuid,VPrn::Jobs job_id,int code,
                             const QString &output_message)
{
    if ( code != 0 ){
        // Ошибка так и скажем клиенту
        emit JobFinish(client_uuid, job_id,code,output_message);
    }else{
        QString e_msg;
        int     e_code;
        QRegExp rx;
        switch (job_id){
        case VPrn::job_ConvertPs2Pdf: // Завершилась задача конвертирования ps в
            e_code = code;
            e_msg  = output_message;
            getPageCount(client_uuid,mainPDF);
            break;
        case VPrn::job_CalcPageCount: // Завершилась задача получения dump_info из pdf документа
            int pagesCount; // Число страниц в конвертируемом документе
            if (!output_message.isEmpty()) {
                rx.setPattern("NumberOfPages:\\s+(\\d{1,5}).*");
                rx.setMinimal(true);
                if (rx.indexIn(output_message) != -1) {
                    bool ok;
                    qDebug() << Q_FUNC_INFO << rx.cap(0) << rx.cap(1);
                    pagesCount = rx.cap(1).toInt(&ok, 10);

                    switch (pagesCount) {
                    case 0:
                        e_code = 1;
                        e_msg  = QObject::trUtf8("Ошибка разбора документа PDF %1")
                                 .arg(mainPDF);
                        break;
                    case 1:
                        if (QFile::copy(mainPDF, firstPage_fn)){
                            e_code = 0;
                            e_msg  = QString("%1").arg(pagesCount,0,10);
                        }else{
                            e_code = 1;
                            e_msg = QObject::trUtf8("Ошибка копиования документа PDF из %1 в %2")
                                           .arg(mainPDF,firstPage_fn);
                        }
                        break;
                    default:
                        e_code = 0;
                        e_msg  = QString("%1").arg(pagesCount,0,10);
                        // Делим файлы на 2 части
                        splitPdf(client_uuid,mainPDF, firstPage_fn, otherPages_fn);
                        break;
                    }
                }
            }
            break;
        }

        emit JobFinish(client_uuid, job_id,e_code,e_msg);
    }
}

//----------------------------------- PRIVATE ---------------------------------------------
void GS_plugin::getPageCount(const QString &client_uuid,const QString &input_fn)
{
    //pdfTk input_file dump_data
    args.clear();
    args.append(input_fn);
    args.append("dump_data");

    ProcessT *proc = new ProcessT(this,VPrn::job_CalcPageCount,client_uuid);
    // Обработчик сообщений файлового потока
    connect(proc, SIGNAL(jobFinish   (const QString &,VPrn::Jobs,int,const QString &)),
            this, SIGNAL(threadFinish(const QString &,VPrn::Jobs,int,const QString &))
            );
    proc->addToEnv(myEnv);
    proc->execute(pdftkBin, args,QProcess::MergedChannels);
}

void GS_plugin::splitPdf(const QString &client_uuid,const QString &main_pdf,
                         const QString &first_page, const QString &other_pages)
{

}

Q_EXPORT_PLUGIN2(Igs_plugin, GS_plugin)
;
