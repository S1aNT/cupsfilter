#include "previewwnd.h"

#include <QtCore/QTextCodec>
#include <QtCore/QTranslator>
#include <QtCore/QLibraryInfo>

#include <QtGui/QApplication>
#include <QtGui/QMessageBox>


int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);
    //qInstallMsgHandler(myMessageOutput);

    QApplication app(argc, argv);
    Q_INIT_RESOURCE(images);

    QString translatorFileName = QLatin1String("qt_");
    translatorFileName += QLocale::system().name();
    QTranslator *translator = new QTranslator(&app);
    if (translator->load(translatorFileName,
                         QLibraryInfo::location(QLibraryInfo::TranslationsPath))){
        app.installTranslator(translator);
    }

    PreviewWnd wnd;
    wnd.show();
    wnd.loadPixmapFromFolder("/home/slant/src/vprn_wrk/images");

    return app.exec();
}
