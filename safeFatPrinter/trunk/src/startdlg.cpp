#include "startdlg.h"
#include "ui_startdlg.h"



StartDlg::StartDlg(QWidget *parent,QStringList &list)
    : QDialog(parent), ui(new Ui::StartDlg)
{
    ui->setupUi(this);
  //generate SID
    SID=QUuid::createUuid () ;
    connect( &proc,
		    SIGNAL(proc_finished(int, QProcess::ExitStatus)),
		  this,
		    SLOT(processDone(int, QProcess::ExitStatus) )
    );
}

void StartDlg::convertToPDF(QSring &input_file)
{
    	QStringList args;
	args.append( "-q" );
	args.append( "-dQUIET" );
	args.append( "-dNOPAUSE" );
	args.append( "-dPARANOIDSAFER" );
	args.append( "-dBATCH" );
	args.append( "-sDEVICE=pdfwrite" );
#if defined(Q_OS_UNIX)
	QString cmd ="//usr//...";
	mainPDF= QString("-sOutputFile=//var/log/spool/cups/tmp//%1").arg(SID.toString());
#elif defined(Q_OS_WIN)
	QString cmd ="C:\\Program Files\\gs\\gs8.70\\bin\\gswin32c.exe";
	mainPDF =QString("-sOutputFile=D:\\Temp\\%1.pdf").arg(SID.toString());
	//gsFile ="c:\\tools\\e.bat";
#endif
	args.append(mainPDF);
	args.append("-c");
	args.append("setpdfwrite");
	args.append("-f");
	args.append(input_file);
	qDebug() << args;
	proc.execute(cmd, args );
}

void StartDlg::mergePDF(QString &main_file,QString &background_file)
{
    	if ( !main_file.isEmpty() ) {
		if ( QFile::exists(main_file ) ) {
		    if ( !main_file.isEmpty() ) {
			if ( QFile::exists(background_file ) ) {
			    //Merge PDF files

			}
			else {
				qCritical() << Q_FUNC_INFO << "File I/O error " << background_file << "as the file does not exist.";
			}
		    }
		    else {
			qCritical() << Q_FUNC_INFO <<  "Empty background filename.";
		    }
		}
		else {
			qCritical() << Q_FUNC_INFO << "File I/O error " << main_file << "as the file does not exist.";
		}
	}
	else {
		qCritical() << Q_FUNC_INFO <<  "Empty main_pdf filename.";
	}
}
void StartDlg::setDocTitle(QString &doc_title)
{
     dTitle=doc_title;
}
StartDlg::~StartDlg()
{
    delete ui;
}

 void StartDlg::processDone(int Code, QProcess::ExitStatus Status)
 {

    if (Code != QProcess::NormalExit) {
	QMessageBox::warning(this, QObject::trUtf8("Обратитесь к системному администратору"),
			     QObject::trUtf8("Ошибка предпечатной подготовки. %1:\nКод %2").arg(Status,Code));
    }
 }
