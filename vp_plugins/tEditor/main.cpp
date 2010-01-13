#include <QtGui/QApplication>
#include <QDebug>
#include <QTextCodec>
#include <QStringList>

#include "config.h"
#include "tech_global.h"
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);
    qInstallMsgHandler(myMessageOutput);

    QApplication app(argc, argv);
    Q_INIT_RESOURCE(images);

    QStringList aList=app.arguments();
    /*
     * @brief По количеству аргументов определяем режим работы
     * - sizeOf (argv[]) = 0 (без аргументов) режим создания пустого шаблона
     * - sizeOf (argv[]) = 1 (имя_файла_шаблона) редактирование выбранного шаблона
     * - sizeOf (argv[]) = 2 (имя_файла_шаблона) (имя файла с данными модели)
     *                       показ шаблона с установленными значениями аргументов
    */
    QString file_name;
    QString file_name_dat;
    MainWindow window;
    window.show();

    switch (aList.size()){
     case 1:
       window.createNewTemplate();
       break;
     case 2:
       file_name = aList.at(1);
       window.loadFromFile(file_name);
       break;
     case 3:
       file_name = aList.at(1);
       file_name_dat = aList.at(2);
       window.loadFromFileWithDat(file_name,file_name_dat);
       break;
    }

    return app.exec();
}
