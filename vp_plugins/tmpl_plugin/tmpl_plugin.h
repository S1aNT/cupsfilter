#ifndef TMPL_PLUGIN_H
#define TMPL_PLUGIN_H

#include <QObject>
#include <QDebug>
#include <QPixmap>
#include <QtPlugin>
#include <QPainter>
#include <QFile>
#include <QDir>
#include <QTemporaryFile>
#include <QDataStream>


#include "itmpl_plugin.h"
#include "tech_global.h"

using namespace SafeVirtualPrinter;

class Tmpl_plugin :public QObject, Itmpl_plugin
{
    Q_OBJECT
    Q_INTERFACES(Itmpl_plugin)
public:

    Tmpl_plugin(QObject *parent=0);
    void init(const QString &spool,const QString &sid,const QStandardItemModel *model);
    // Возвращает полный путь к сформированному на основании шаблона и данных модели файлу
    QString getFormatingPageFile(const QString &templates_in_file,int pageNum);

signals:
    void error(QString error_message);
protected:
     QStandardItemModel *work_model;
     QString spool_dir;
     QString current_sid;

     QString firstPage_tmpl_fn;
     QString secondPage_tmpl_fn;
     QString thirdPage_tmpl_fn;
     QString fourthPage_tmpl_fn;
};

#endif
