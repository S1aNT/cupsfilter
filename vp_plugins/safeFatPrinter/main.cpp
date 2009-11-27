#include <QtGui/QApplication>
#include <QDebug>
#include <QTextCodec>
#include <QErrorMessage>

#include "mediator.h"
#include "mainwindow.h"
#include "firstask.h"
#include "getusernamemandatdlg.h"
#include "config.h"
#include "tech_global.h"


int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(images);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    //QTextCodec::setCodecForTr( QTextCodec::codecForName( "cp866" ) );
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);
    qInstallMsgHandler(myMessageOutput);

    QApplication app(argc, argv);


    QStringList aList=app.arguments();
    if (aList.size()< 1){
            QMessageBox::critical(0,QObject::trUtf8("Обратитесь к системному администратору"),
                  QObject::trUtf8("Ошибка запуска приложения, не достаточно параметров запуска"));
            app.exit(1);
    }else {
            QString in_file =aList.at(1);
            QPixmap pixmap(":images/splash.png");

            Mediator *SpiderInTheMiddle = new Mediator();
            getUserNameMandatDlg *UMDlg = new getUserNameMandatDlg();
            firstAsk *askDlg = new firstAsk();

            QSplashScreen *splash  = new QSplashScreen(pixmap);
            QErrorMessage *errorMessage = new QErrorMessage();

            QObject::connect (SpiderInTheMiddle,SIGNAL(error (QString )),errorMessage,SLOT(showMessage(QString)));
            QObject::connect (SpiderInTheMiddle,SIGNAL(needShowAuthWindow(QString &)),UMDlg,SLOT(setUserName(QString &)));
            QObject::connect (SpiderInTheMiddle,SIGNAL(pluginLoad(QString &,int, QColor &)),splash,SLOT(showMessage(QString &,int, QColor &)));

            splash->show();
            splash->showMessage(QObject::trUtf8("Загрузка плагинов"),Qt::AlignLeft| Qt::AlignBottom, QColor::fromRgb(170,255,0));
            QTimer::singleShot(1000, splash, SLOT(clearMessage()));
            SpiderInTheMiddle->loadPlugin(qApp->applicationDirPath());

            app.processEvents();
            splash->showMessage(QObject::trUtf8("Инициализация"),Qt::AlignLeft| Qt::AlignBottom, QColor::fromRgb(170,255,0));
            QTimer::singleShot(1000, splash, SLOT(clearMessage()));
            SpiderInTheMiddle->plugin_init();

            app.processEvents();
            QTimer::singleShot(1000, splash, SLOT(close()));

            // Определяемся с режимом работы
            if (SpiderInTheMiddle->isValid()){
                MainWindow window;
                window.setSpider(SpiderInTheMiddle);

                // Связь с могучим демоном установленна
                if (!SpiderInTheMiddle->isAuth()){
                   // Требуется показать Диалоговое окошко запроса USER|Mandat
                    // К этому моменту я уже получил от демона список всех мандатов
                    if (SpiderInTheMiddle->isHaveMandatList()){
                        UMDlg->setMandatModel(SpiderInTheMiddle->mandat_model());
                        if (UMDlg->exec() == QDialog::Accepted){
                            SpiderInTheMiddle->setUserMandat(UMDlg->getCurrentMandat());
                        }
                    }
                }else{
                    // Пользоваталь авторизовался через агент безопастности
                    // или существует удачно распрасенный файл session_ticket
                    // Покажем чудесатое окошко с выбором режима работы
                    if (askDlg->exec() == QDialog::Accepted){
                        SpiderInTheMiddle->setWorkMode(askDlg->getCurrentMode());
                        window.show();
                    }
                }
            }else{
                // Была ошибка загрузки плагинов или начальной инициализации
                QTimer::singleShot(1000, qApp, SLOT(quit()));
            }
            return app.exec();
    }
}
