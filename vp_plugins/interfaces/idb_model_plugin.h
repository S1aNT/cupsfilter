#ifndef IDB_MODEL_PLUGIN_H
#define IDB_MODEL_PLUGIN_H

#include <QObject>

class QStandardItemModel;

class Idb_model_plugin{
public:
    virtual ~Idb_model_plugin() {}    
    virtual void init(const QString &spool) = 0;
    virtual void readTemplate(const QString &cuid, const QString &fileName,
                               QStandardItemModel *tInfo,
                               QStandardItemModel *pages,
                               QStandardItemModel *elements) =0;
};

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(Idb_model_plugin,"com.technoserv.Plugin.Idb_model_plugin/1.0");
QT_END_NAMESPACE

#endif
