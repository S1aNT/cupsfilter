
#include <QtSingleApplication>
#include <QtCore/QTextCodec>

#include "config.h"
#include "mytypes.h"
#include "printmonitor.h"

using namespace VPrn;

int main(int argc, char *argv[])
{

    Q_INIT_RESOURCE(images);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);
    //qInstallMsgHandler(myMessageOutput);

    QtSingleApplication app(argc, argv, true);


    if (app.sendMessage(QObject::tr("Потребован повторный запуск приложения!"))
        || app.isRunning())
        return 0;

    PrintMonitor monitor;
    app.setActivationWindow(&monitor);

    QObject::connect(&app, SIGNAL(messageReceived(const QString&)),
		     &monitor, SLOT(appendStartMsg(const QString&)));
    monitor.show();
    return app.exec();
}
