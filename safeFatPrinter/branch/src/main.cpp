#include <QDebug>
#include <QtGui/QApplication>
#include "startdlg.h"
#include <QTextCodec>
#include<QMessageBox>

int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    //QTextCodec::setCodecForTr( QTextCodec::codecForName( "cp866" ) );
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);

    QApplication app(argc, argv);
    QStringList aList=app.arguments();
    qDebug() << aList.size();
    if (aList.size()< 1){
	QMessageBox::critical(0,QObject::trUtf8("Обратитесь к системному администратору"),
			      QObject::trUtf8("Ошибка запуска приложения, не достаточно параметров запуска"));

	qDebug() << aList<<QString("Need more command line arguments.Current arguments %1\nExample safe_printer input.file doc_name").arg(aList.size());
	app.exit(1);
    }else {
	QString in_file =aList.at(1);

	StartDlg w;
	w.convertToPDF(in_file);
	w.show();
	return app.exec();
    }
}
