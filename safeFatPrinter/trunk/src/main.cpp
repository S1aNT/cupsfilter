#include <QDebug>
#include <QtGui/QApplication>
#include <QTextCodec>
#include <QMessageBox>
#include <QStringList>
#include <QPrintDialog>

#include "workreport.h"
#include "dController.h"
#include "config.h"
#include "startdlg.h"


#include <QTableView>



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
    if (aList.size()!=3){
	QMessageBox::critical(0,QObject::trUtf8("Обратитесь к системному администратору"),
					    QObject::trUtf8("Ошибка запуска приложения, не достаточно параметров %1").arg(aList.size()));

	qDebug() << aList<<QString("Need more command line arguments.Current arguments %1\nExample safe_printer input.file doc_name").arg(aList.size());
	app.exit(1);
    }else {
/*
	QSettings mandat_set("d:\\safeprinter.ini",QSettings::IniFormat);
	mandat_set.beginGroup("RULES");
	QString mandat =mandat_set.value("mandat","UNDEF").toString();
	mandat_set.endGroup();
*/
	QString mandat ="SYSPROG";
	QString in_file =aList.at(1);
	StartDlg w;

	dController control;

	QTableView tableView;
	tableView.setModel(control.document_model());
	tableView.show();

	w.setController(&control);
	w.convertToPDF(in_file);
	w.show();
	return app.exec();
    }

}

