#include "gsworker.h"

gsWorker::gsWorker(QStringList &argList)
{

    if (argList.size()==3)
    {
#if defined(Q_OS_UNIX)
	gsFile ="//usr//...";
	rspFile = "//usr//local//safeprinter//pdfwrite.rsp";
	tempPDF=QString("-o//var/log/spool/cups/tmp//%1.pdf").arg(SID.toString());
#elif defined(Q_OS_WIN)
	rspFile ="@c:\\gs\\pdfwrite.rsp";
	gsFile ="C:\\Program Files\\gs\\gs8.70\\bin\\gswin32c.exe";
	//tempPDF=QString("-oD:\\Temp\\%1.pdf").arg(SID.toString());
	gsFile ="c:\\tools\\e.bat";
#endif
	// the first argument, which is the program's name
	userName=argList.at(1);
	docName=argList.at(2);
	//generate SID
	SID=QUuid::createUuid () ;
	process = new QProcess(this);
	connect(process,
			SIGNAL(finished(int, QProcess::ExitStatus)),
		    this,
			SLOT(convertDone(int, QProcess::ExitStatus))
	);

	QStringList aList;
	aList <<rspFile  << "-q" << tempPDF << "-c .setpdfwrite" <<"-f"<<"-";
	qDebug() <<aList;
	 process->start(gsFile,aList);
    }else{
	emit error (exitCode::needMore,
			QObject::trUtf8("Ошибка при запуске программы, отсутствуют необходимые аргументы."));
	qDebug() << QObject::trUtf8("Need more arguments. Argc =%1\n Usage [STDIN] > safe_printer USERNAME DOCNAME ").arg(argList.size());
    }
}

gsWorker::~gsWorker()
{
	if (process->state() == QProcess::Running) {
	    process->terminate();
	    process->waitForFinished(3000);
	}
}


QString  gsWorker::getUserName() const
{
    return userName;
}

QString  gsWorker::getDocName() const
{
    return docName;
}



void gsWorker::converDone(int Code, QProcess::ExitStatus Status)
{
    qDebug() << process->readAllStandardOutput();
    if (Code() != 0) {
	emit error (Code,Object::trUtf8("Ошибка предпечатной подготовки.\nОбратитесь к системмному администратору"));
	qDebug() << process->readAllStandardError();
    }
}

bool gsWorker::convert2PDF()
{


 }


    //Create temp file
/*
  bool gsWorker::read(QTextStream &stream)
{
    //inputData=stream.readAll();
    if (inputData.isEmpty()){
	//errStr =QObject::trUtf8("Ошибка печати, при чтении данных.\nОбратитесь к системному администратору");
	qDebug() << "Read error from stdin. Stream is empty";
	return false;
    }else{
	return convert2PDF(inputData);
    }
}
    QTemporaryFile tFile;
     if (tFile.open()) {
	 // file.fileName() returns the unique file name
	 QTextStream stream;
	 stream.setDevice(&tFile);
	 stream << inBuf;
	 qDebug() << "create temp file: " << tFile.fileName() ;
     }
    QProcess otool;
    //    otool.setStandardInputFile(tFile.fileName());

    QProcess otool;
    QStringList aList;
    aList <<rspFile  << "-q" << tempPDF << "-c .setpdfwrite" <<"-f"<<"-";
    qDebug() <<aList;
    otool.start(gsFile,aList);
    otool.waitForFinished();

    if (otool.exitCode() != 0) {
	errStr=QObject::trUtf8("Ошибка предпечатной подготовки.\nОбратитесь к системмному администратору");
	qDebug() << otool.readAllStandardError();
	return false;
    }else{
	qDebug() << otool.readAllStandardOutput();
	return true;
    }
*/
