#include "gs_plugin.h"
#include "proc_thread.h"

#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QRegExp>
#include <QtCore/QTemporaryFile>
#include <QtCore/QTextStream>
#include <QtCore/QDateTime>
//#include <QtCore/QMetaType>
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
                     const QString &temp_folder)
{
    QDir dir;
    QFile new_file;
    const QString startnow = QDir::currentPath();

    bool Ok = true;
    {
        Ok &= QFile::exists(gs_bin);
        if (Ok){// Проверим факт существования по указанному пути бинарника ghostscript
            gsBin = gs_bin;

            Ok &= dir.cd(temp_folder) && !temp_folder.isEmpty();
            if (Ok){// Проверим факт существования временного каталога
                tempPath = temp_folder;
                myEnv << QObject::trUtf8("TMPDIR=%1").arg(temp_folder);

                Ok &=QFile::exists(pdftk_bin);
                if (Ok){// Проверим факт существования по указанному пути бинарника
                    pdftkBin = pdftk_bin;
                    // Файл не существует но он мне нужен значит создаем его
                    gs_rcp=QString("%1/%2.rcp").arg(temp_folder,getUuid());
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
        // Формируем основной выходной файл для клиента
        QString mainPDF = QObject::trUtf8("%1/%2_main.pdf")
                          .arg( tempPath, client_uuid );

        // Формируем строку команды для первого этапа
        args.clear();
        args.append("-q" );
        args.append(QString("@%1").arg(gs_rcp));
        args.append(QString("-sOutputFile=%1").arg(mainPDF));
        args.append("-c");
        args.append(".setpdfwrite");
        args.append("-f");
        args.append(input_fn);
        start_proc(client_uuid,gsBin,args,VPrn::job_ConvertPs2Pdf);

    }else {
        emit error(VPrn::FileIOError,
                   QObject::trUtf8("ERROR : Файл %1 не найден\n").arg(input_fn));
    }
}



//---------------------------- PRIVATE SLOTS --------------------------------------------------
void GS_plugin::threadFinish(const QString &jobKey,int code,
                             const QString &output_message)
{

    QString m_client_uuid = clients_list.take(jobKey);
    VPrn::Jobs m_job = jobs_list.take(jobKey);
    qDebug() << "\nthreadFinish:"
            << "\nm_client_uuid:" << m_client_uuid
            << "\njobKey:" <<jobKey
            << "\nm_job:" <<m_job;

    // Формируем пути для файлов
    QString mainPDF       = QObject::trUtf8("%1/%2_main.pdf")
                            .arg( tempPath, m_client_uuid );
    QString firstPage_fn  = QObject::trUtf8("%1/%2_first.pdf")
                            .arg( tempPath, m_client_uuid );
    QString otherPages_fn = QObject::trUtf8("%1/%2_other.pdf")
                            .arg(tempPath, m_client_uuid);

    // Посмотрим чем же таким важным был занят клиент
    QRegExp rx;
    switch (m_job){
    case VPrn::job_ConvertPs2Pdf: // Завершилась задача конвертирования ps в
        //Задача завершена, доложим наверх
        emit jobFinish(m_client_uuid, m_job,code,output_message);
        getPageCount(m_client_uuid,mainPDF);
        break;
    case VPrn::job_SplitPageFirst:
    case VPrn::job_SplitPageOther:
        //Задача завершена, доложим наверх
        emit jobFinish(m_client_uuid, m_job,code,output_message);
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
                // числа листов в документе зависит наш ответ
                if (pagesCount == 0 || !ok){
                    emit jobFinish(m_client_uuid,VPrn::job_CalcPageCount,
                                   1,
                                   QObject::trUtf8("Ошибка разбора документа PDF %1")
                                   .arg(mainPDF)
                                   );
                }else{
                    if (pagesCount ==1 && QFile::copy(mainPDF, firstPage_fn) ){
                        //Так как в документе только 1 страница, то сразу
                        //рапортуем о завершении сразу 2-х задач.
                        emit jobFinish(m_client_uuid,VPrn::job_CalcPageCount,
                                       code,
                                       QString("%1").arg(pagesCount,0,10)
                                       );
                        emit jobFinish(m_client_uuid,VPrn::job_SplitPageFirst,
                                       code,tr(""));
                    }else{
                        emit jobFinish(m_client_uuid,VPrn::job_CalcPageCount,
                                       1,
                                       QObject::trUtf8("Ошибка копирования документа PDF из %1 в %2")
                                       .arg(mainPDF,firstPage_fn)
                                       );
                    }
                    if (pagesCount > 1){
                        emit jobFinish(m_client_uuid,VPrn::job_CalcPageCount,
                                       code,
                                       QString("%1").arg(pagesCount,0,10)
                                       );
                        // Делим файлы на 2 части
                        splitPdf(m_client_uuid,mainPDF,firstPage_fn, otherPages_fn);
                    }
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

    qDebug() << "\nthreadStart:"
            << "\nclient_uuid:" << client_uuid
            << "\njob_key:"     << j_key
            << "\njob_id:"      << job_id;

    ProcessT *proc = new ProcessT(this,j_key);
    // Обработчик сообщений файлового потока
    connect(proc, SIGNAL(jobFinish     (const QString &,int,const QString &)),
            this, SLOT  (threadFinish  (const QString &,int,const QString &))
            );
    proc->addToEnv(myEnv);
    proc->execute(bin, arg_list,QProcess::MergedChannels);
}

void GS_plugin::getPageCount(const QString &client_uuid,const QString &input_fn)
{
    //pdfTk input_file dump_data
    args.clear();
    args.append(input_fn);
    args.append("dump_data");
    start_proc(client_uuid,pdftkBin,args,VPrn::job_CalcPageCount);
}

void GS_plugin::splitPdf(const QString &client_uuid,const QString &main_pdf,
                         const QString &first_page, const QString &other_pages)
{
    //pdftk %1 cat %2 output page_%2.pdf
    // Так как сюда попадаем только только если в документе больше 1 страницы,
    // То запускаем 2 потока формирирование 1 стр, и формирование последующих

    // Первую страницу
    args.clear();
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

QString GS_plugin::getUuid() const
{
    return QUuid::createUuid().toString().replace("{","").replace("}","");
}


Q_EXPORT_PLUGIN2(Igs_plugin, GS_plugin)
;
