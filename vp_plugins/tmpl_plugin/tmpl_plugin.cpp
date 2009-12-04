#include "tmpl_plugin.h"
#include "tech_global.h"

using namespace SafeVirtualPrinter;


void Tmpl_plugin::init(const QString &spool,const QString &sid,const QStandardItemModel *model)
{
    QString error_msg;
    QDir dir;

    if (!sid.isEmpty()) {
        current_sid = sid;// Запомним уникальный номер
        if (model){
            if (dir.cd(spool) && !spool.isEmpty()) {
                // Проверим факт существования временного каталога
                spool_dir = spool;
                // Формируем пути для файлов
                firstPage_tmpl_fn = QObject::trUtf8("%1/%2_first.pdf").arg(spool, sid);
                secondPage_tmpl_fn = QObject::trUtf8("%1/%2_second.pdf").arg(spool, sid);
                thirdPage_tmpl_fn = QObject::trUtf8("%1/%2_third.pdf").arg(spool, sid);
                fourthPage_tmpl_fn = QObject::trUtf8("%1/%2_fourth.pdf").arg(spool, sid);
                // создаем сцены
                firstPage_tmpl  = new QGraphicsScene(this);
                secondPage_tmpl = new QGraphicsScene(this);
                thirdPage_tmpl  = new QGraphicsScene(this);
                fourthPage_tmpl = new QGraphicsScene(this);

            }else{
                error_msg = QObject::trUtf8("ERROR: каталог %1 не существует\n").arg(spool);
            }
        }else{
            error_msg = QObject::trUtf8("ERROR: Модель [карточки документа] не существует\n");
        }
    }else{
        error_msg = QObject::trUtf8("ERROR: Неверный SID для документа\n").arg(sid);
    }
    if (!error_msg.isEmpty()) {
        emit error(error_msg);
    }
}

void Tmpl_plugin::setTemplates(const QString & templates_in_file)
{
    QString error_msg;
    if (QFile::exists(templates_in_file)) {
        firstPage_tmpl->clear();
        secondPage_tmpl->clear();
        thirdPage_tmpl->clear();
        fourthPage_tmpl->clear();
        if (QFile::exists(firstPage_tmpl_fn)){
            QFile::remove(firstPage_tmpl_fn);
        }
        if (QFile::exists(secondPage_tmpl_fn)){
            QFile::remove(secondPage_tmpl_fn);
        }
        if (QFile::exists(thirdPage_tmpl_fn)){
            QFile::remove(thirdPage_tmpl_fn);
        }
        if (QFile::exists(fourthPage_tmpl_fn )){
            QFile::remove(fourthPage_tmpl_fn );
        }
        currentTemplates = templates_in_file;
    }else{
        error_msg = QObject::trUtf8("Файл [%1] шаблона не найден.").arg(templates_in_file);
    }
    if (!error_msg.isEmpty()) {
        emit error(error_msg);
    }
}

void Tmpl_plugin::printFormatingPageToFile(int pageNum)
{
    QString error_msg;
    // Печатает выбранную страницу текущего шаблона в pdf файл
    // страница формируется исходя из данных модели
    if (!currentTemplates.isEmpty() && pageNum <= 4 && pageNum >=1){
        QPrinter pdfprinter;
        if (page_orient){
            pdfprinter.setOrientation(QPrinter::Portrait);
        }else{
            pdfprinter.setOrientation(QPrinter::Landscape);
        }
        pdfprinter.setOutputFormat(QPrinter::PdfFormat);
        pdfprinter.setPageSize(paper_size);
        QPainter painter(&pdfprinter);

        switch (pageNum){
        case 1:
            if (parse_templates(pageNum,firstPage_tmpl)){
                pdfprinter.setOutputFileName(firstPage_tmpl_fn);
                firstPage_tmpl->render(&painter);
            }else{
                error_msg = QObject::trUtf8("ERROR: Ошибка формирования PDF [%1] из %2 страницы шаблона\n")
                            .arg(firstPage_tmpl_fn).arg(pageNum,0,10);
            }

            break;
        case 2:
            if (parse_templates(pageNum,secondPage_tmpl)){
                pdfprinter.setOutputFileName(secondPage_tmpl_fn);
                secondPage_tmpl->render(&painter);
            }else{
                error_msg = QObject::trUtf8("ERROR: Ошибка формирования PDF [%1] из %2 страницы шаблона\n")
                            .arg(secondPage_tmpl_fn).arg(pageNum,0,10);
            }
            break;
        case 3:
            if (parse_templates(pageNum,thirdPage_tmpl)){
                pdfprinter.setOutputFileName(thirdPage_tmpl_fn);
                thirdPage_tmpl->render(&painter);
            }else{
                error_msg = QObject::trUtf8("ERROR: Ошибка формирования PDF [%1] из %2 страницы шаблона\n")
                            .arg(thirdPage_tmpl_fn).arg(pageNum,0,10);
            }
            break;
        case 4:
            if (parse_templates(pageNum,fourthPage_tmpl)){
                pdfprinter.setOutputFileName(fourthPage_tmpl_fn);
                fourthPage_tmpl->render(&painter);
            }else{
                error_msg = QObject::trUtf8("ERROR: Ошибка формирования PDF [%1] из %2 страницы шаблона\n")
                            .arg(fourthPage_tmpl_fn).arg(pageNum,0,10);
            }
            break;
        }
    }
}


bool Tmpl_plugin::parse_templates(int page,QGraphicsScene *scene)
{
    bool flag;
    int ver;

    if (!currentTemplates.isEmpty()){
        scene->clear();
        QFile file(currentTemplates);
        file.open(QIODevice::ReadOnly);
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_4_5);
        in >> ver;
        if (ver <= version ){
            // Читаем общую часть шаблона


        }else{
            flag =  false; // Мой плагин работает с шаблоном только версии <= version
        }
        file.close();
    }

    return flag;
}

Q_EXPORT_PLUGIN2(Itmpl_plugin, Tmpl_plugin)
;


