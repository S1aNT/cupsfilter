#include <QDebug>
#include <QtGui>
#include <QSqlQueryModel>
#include <QTextCodec>

#include "plugin_worker.h"
#include "addtemplate.h"

#include "tinfoeditmodel.h"

void myMessageOutput(QtMsgType type, const char *msg)
{
    QFile logFile("safe_printer.log");

    if (!logFile.open(QFile::Append| QFile::Text)){
        logFile.open(stderr, QIODevice::WriteOnly);
    }

     QTextStream out;
     out.setDevice(&logFile);
     out.setCodec("UTF-8");

     out << "\nDateTime: " << QDateTime::currentDateTime ().toString("dd.MM.yyyy hh:mm:ss");
     switch (type) {
     case QtDebugMsg:
        out << QObject::trUtf8("Debug: %1\n").arg(QString(msg)) <<"\n";
         break;
     case QtWarningMsg:
         out << QObject::trUtf8("Warning: %1\n").arg(QString(msg))<<"\n";
         break;
     case QtCriticalMsg:
         out << QObject::trUtf8("Critical: %1\n").arg(QString(msg))<<"\n";
         break;
     case QtFatalMsg:
         out << QObject::trUtf8("Fatal: %1\n").arg(QString(msg))<<"\n";
         abort();
     }
     logFile.close();
}

void createView(const QString &title, QAbstractItemModel * model )
{
    static int offset = 0;

    QTableView *view = new QTableView;
    view->setModel(model);
    view->setWindowTitle(title);
    view->move(100 + offset, 100 + offset);
    offset += 20;
    view->show();
}

int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);
    qInstallMsgHandler(myMessageOutput);

    QApplication app(argc, argv);
    AddTemplate * addTempl = new AddTemplate();

    pluginWorker *testObj = new pluginWorker();

    if (testObj->loadPlugin()){
       testObj->createEmptyTemplate("d://test_templ.tmpl");
       /*
       MyFilterProxyModel *proxyModel = new MyFilterProxyModel;
       proxyModel->setSourceModel(testObj->getPSizeModel());
       createView(QObject::tr("Proxy Model"), proxyModel);
       */

       createView(QObject::tr("Info Query Model"), testObj->getInfoModel());
       createView(QObject::tr("PageSize Query Model"), testObj->getPSizeModel());

       addTempl->setInfoModel(testObj->getInfoModel());
       addTempl->setPageSizeModel(testObj->getPSizeModel());

       addTempl->show();
    }

    return app.exec();
}
