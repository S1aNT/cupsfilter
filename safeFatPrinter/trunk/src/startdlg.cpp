#include "startdlg.h"
#include "ui_startdlg.h"

StartDlg::StartDlg(QWidget *parent)
    : QDialog(parent), ui(new Ui::StartDlg)
{
    ui->setupUi(this);
    init();
    makeConnection();
    read_settings();
}
StartDlg::~StartDlg()
{
    delete ui;
}
void StartDlg::read_settings()
{
     QSettings settings(QSettings::IniFormat, QSettings::UserScope,"Техносервъ","Защищенный принтер");
      settings.setIniCodec("UTF-8");
      settings.beginGroup( "SERVICE" );

#if defined(Q_OS_UNIX)
	gsBin = settings.value("gs_bin","/usr/local/bin/gs").toString();
	mainPDF = settings.value("output_fg_PDF","-sOutputFile=//var/log/spool/cups/tmp//%1.pdf").toString();
	pdfTk = settings.value("pdfTK","/usr/local/bin/pdftk").toString();
#elif defined(Q_OS_WIN)
	gsBin = settings.value("gs_bin","C:\\Program Files\\gs\\gs8.70\\bin\\gswin32c.exe").toString();
	mainPDF = settings.value("output_fg_PDF","-sOutputFile=D:\\safePrinter\\%1.pdf").toString();
	pdftkBin = settings.value("pdfTK","D:\\Tools\\pdfTK\\pdftk.exe").toString();
#endif
      settings.endGroup();
}
void StartDlg::closeEvent(QCloseEvent *event)
{
    write_settings();
    event->accept();
}
void StartDlg::write_settings()
{
   QSettings settings(QSettings::IniFormat, QSettings::UserScope,"Техносервъ","Защищенный принтер");
   settings.setIniCodec("UTF-8");
    settings.beginGroup( "SERVICE" );
    settings.setValue("gs_bin",gsBin);
    settings.setValue("output_fg_PDF",mainPDF);
    settings.setValue("pdfTK",pdftkBin);
    settings.endGroup();
}

/*								Function				    */
void StartDlg::init()
{

    SID=QUuid::createUuid () ;  //generate SID

 }

void StartDlg::makeConnection()
{
      connect (&proc,
		    SIGNAL(commandOutput(int,QString)),
		 this,
		    SLOT(processDone(int,QString))
		    );
    connect (ui->markPaperBtn,
		    SIGNAL(clicked()),
		 this,
		    SLOT(markPaper())
		    );
}

void StartDlg::convertToPDF(const QString &input_file)
{
	args.clear();
	args.append( "-q" );
	args.append( "-dQUIET" );
	args.append( "-dNOPAUSE" );
	args.append( "-dPARANOIDSAFER" );
	args.append( "-dBATCH" );
	args.append("-r300");
	args.append( "-sDEVICE=pdfwrite" );
	args.append(mainPDF.arg(SID.toString()));
	args.append("-c");
	args.append(".setpdfwrite");
	args.append("-f");
	args.append(input_file);
	proc.execute(gsBin, args,QProcess::MergedChannels);
}

/*								SLOTS				    */
void StartDlg::displayDebug(const QString  &line)
{
    QMessageBox msgBox;
    msgBox.setText(line);
    msgBox.exec();
}

void StartDlg::mergePDF(QString &main_file,QString &background_file)
{
    	if ( !main_file.isEmpty() ) {
		if ( QFile::exists(main_file ) ) {
		    if ( !background_file.isEmpty() ) {
			if ( QFile::exists(background_file ) ) {
			    //Merge PDF files
			    args.clear();

			    proc.execute(pdftkBin, args,QProcess::MergedChannels);
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

 void StartDlg::processDone(int Code, const QString &s_output)
 {

    if (Code != QProcess::NormalExit) {
	QMessageBox::warning(this, QObject::trUtf8("Обратитесь к системному администратору"),
			     QObject::trUtf8("Ошибка предпечатной подготовки. %1:\nКод %2").arg(s_output,Code));
    }else{
	QMessageBox msgBox;
	msgBox.setText("Исходный файл успешно конвертирован в pdf");
	msgBox.exec();
    }
 }

 void StartDlg::setDocName(const QString &docName)
 {
     dTitle=docName;
 }

void StartDlg::setMandat(const QString &userName,const QString &mandatName)
{
    currentUserName=userName;
    mandat=mandatName;

/*
    QList<QPrinterInfo> plist;
     plist = QPrinterInfo::availablePrinters () ;
     int def;
     for (int i = 0; i < plist.size(); ++i) {
	if (plist.at(i).isDefault () ){
	    def=i;
	}
	ui->printerCBox->addItem(plist.at(i).printerName());
     }
     ui->printerCBox->setCurrentIndex(def);// индекс принтера по умолчанию
   */
}

 void StartDlg::markPaper()
 {
     this->hide();
     askTheUser askDlg;
     askDlg.setWindowTitle("Предварительный учет листов");

     askDlg.exec();
 }
