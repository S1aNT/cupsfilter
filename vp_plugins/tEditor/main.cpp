#include <QtGui/QApplication>
#include <QDebug>
#include <QTextCodec>
#include <QStringList>

/*! \mainpage OMSp_MBT_ComposTest
 *
 * \section intro_sec Introduction
 *
 * <b>Developer</b>:
 * - ���
 *
 * \section requirements Requirements
 * The application OMSp_MBT_ComposTest.dll requires the following external stuff:
 * - lib1
 * - dll1
 *
 * \section changelog Changelog
 *
 * - February 2009: initial release
 *
 * \section About_sec About this documentation
 *
 * \subsection step1 Location
 *  
 * This documentation can be found on file://dir1/dir2/index.html
 * 
 * \subsection step2 Updating
 * 
 * <b>Requirements</b>
 * - Installed <b><i>doxygen</i></b> package (from http://www.stack.nl/~dimitri/doxygen)
 * - Optional: Installed <b><i>graphviz</i></b> tool for diagrams (from http://www.graphviz.org) 
 *   <br><sup><b>Note:</b> you have add the <b>bin</b> path of the installation to the 
 *   <tt>PATH</tt> environment variable</sup>
 *
 * <b>Generating</b>
 *
 * - go to source location <b> dir1\project_dir </b>
 * - open a cmd window
 * - type <tt>doxygen tEditor.doxygen </tt>
 *
 * <b>Configuration files</b>
 * - <tt> tEditor.doxygen </tt> (text file; general configuration of input files and output)
 * - <tt> main.cpp</tt> (text file for processing this Main Page)
 */

#include "config.h"
//#include "tech_global.h"
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
     * - sizeOf (argv[]) = 1 (без аргументов) режим создания пустого шаблона
     * - sizeOf (argv[]) = 2 (имя_файла_шаблона) редактирование выбранного шаблона
    */
    QString file_name;

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
    }

    return app.exec();
}
