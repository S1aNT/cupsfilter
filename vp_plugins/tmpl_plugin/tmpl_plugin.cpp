#include "tmpl_plugin.h"


void Tmpl_plugin::init(const QString &spool,const QString &sid,const QStandardItemModel *model)
{
    /*
    QString error_msg;
    if (!sid.isEmpty()) {
        Sid = sid;// Запомним уникальный номер
        if (dir.cd(spool) && !spool.isEmpty()) {
            // Проверим факт существования временного каталога
            tempPath = temp_folder;
            // Формируем пути для файлов
            firstPage_tmpl_fn = QObject::trUtf8("%1/%2_first.pdf").arg(spool, sid);
            secondPage_tmpl_fn = QObject::trUtf8("%1/%2_second.pdf").arg(spool, sid);
            thirdPage_tmpl_fn = QObject::trUtf8("%1/%2_third.pdf").arg(spool, sid);
            fourthPage_tmpl_fn = QObject::trUtf8("%1/%2_fourth.pdf").arg(spool, sid);

        }else{
            error_msg = QObject::trUtf8("ERROR: каталог %1 не существует\n").arg(spool);
        }
    }else{
        error_msg = QObject::trUtf8("ERROR: Неверный SID для документа\n").arg(sid);
    }

    if (!error_msg.isEmpty()) {
        emit error(error_msg);
    }
*/
}

// Возвращает полный путь к сформированному на основании шаблона и данных модели файлу
QString Tmpl_plugin::getFormatingPageFile(const QString &templates_in_file,int pageNum)
{

}

Q_EXPORT_PLUGIN2(Itmpl_plugin, Tmpl_plugin)
;


