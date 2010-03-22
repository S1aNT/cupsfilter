#include "printmonitor.h"
#include "message.h"

#include "ui_printmonitor.h"

#include <QtCore/QSettings>
#include <QtCore/QPluginLoader>
#include <QtCore/QProcess>
#include <QtCore/QFile>
#include <QtNetwork/QLocalSocket>
#include <QtGui/QApplication>

PrintMonitor::PrintMonitor(QWidget *parent) :
        QWizard(parent),
        ui(new Ui::PrintMonitor)
{
    ui->setupUi(this);

    core_app = new Engine(0,qApp->applicationDirPath());

    connect(core_app,SIGNAL( infoMessage(QString) ),
            this,    SLOT  ( showInfoMessage(QString) )
            );
    core_app->init();
}

PrintMonitor::~PrintMonitor()
{
    delete ui;
}

void PrintMonitor::changeEvent(QEvent *e)
{
    QWizard::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

//------------------------- PRIVATE SLOTS --------------------------------------
 void PrintMonitor::showInfoMessage(const QString &info)
 {
    new QListWidgetItem(info,  ui->infoListWidget);
 }
