#include "startdlg.h"
#include "ui_startdlg.h"

StartDlg::StartDlg(QWidget *parent)
    : QDialog(parent), ui(new Ui::StartDlg)
{
    ui->setupUi(this);
    setPrinterList();

    cnv=false;
    merge=false;

    connect (ui->markPaperBtn,SIGNAL(clicked()),this,SLOT(markPaper()));
    connect (ui->printOnMarkPaperBtn,SIGNAL(clicked()),this,SLOT(printOnMarkPaper()));
    connect (ui->printerCBox,SIGNAL(currentIndexChanged(QString)),this,SIGNAL(printerSelected(QString)));
}


void StartDlg::setPrinterList()
{
 QList<QPrinterInfo> plist;
 plist = QPrinterInfo::availablePrinters () ;
 for (int i = 0; i < plist.size(); ++i) {

     if (plist.at(i).printerName()!="Защищенный принтер"){
	 ui->printerCBox->addItem(plist.at(i).printerName());
     }
 }
 ui->printerCBox->setCurrentIndex(-1);
 /*
 if (plist.size() !=-1){
     QPrinterInfo printer=plist.at(0);
     ui->printerCBox->setCurrentIndex(0);
     qDebug() << "emit printerSelected (printer.printerName())" <<printer.printerName();
     emit printerSelected (printer.printerName());
 }
 */
}

void StartDlg::printOnMarkPaper()
{
    qDebug() << Q_FUNC_INFO;

    askDlg.setWindowTitle("Печать на предварительно учтенных листах");
    askDlg.setViewMode(2); // предварительный учет листов печатаем только заднюю сторону листа

    askDlg.setModel(d_ctrl->document_model());
    //askDlg->setAttribute(Qt::WA_DeleteOnClose);
    qDebug()<< "After setModel";
    //askDlg.show();
    if (askDlg.exec()==QDialog::Accepted){
	 //askDlg->close();
    }
}

void StartDlg::markPaper()
{

    //askDlg.show();
     //TODO remove after debug
     //askDlg.setMBCompleter(QStringList() <<tr("МБ 12/12-1")<<tr("МБ 132/12-122")<<tr("МБ 32/12-1"));

     askDlg.setWindowTitle("Предварительный учет листов");
     askDlg.setViewMode(1); // предварительный учет листов печатаем только заднюю сторону листа
    // d_ctrl->insertDocToModel();

     askDlg.setModel(d_ctrl->document_model());
     if (askDlg.exec()==QDialog::Accepted){
	 //askDlg->close();
    }

}
void StartDlg::setController(dController *ctrl)
{
    d_ctrl= ctrl;
    connect (d_ctrl,SIGNAL(error(int,QString)),this,SLOT(error(int,QString)));
    connect (d_ctrl,SIGNAL(inputFileConverted()),this,SLOT(convertDone()));
    connect (d_ctrl,SIGNAL(inputFileMerged()),this,SLOT(mergeDone()));
    connect (d_ctrl,SIGNAL(connect2Demon()),this,SLOT(connectToDemon()));
    connect (d_ctrl,SIGNAL(sayMeGood()),&reportDlg,SLOT(show()));
    connect (d_ctrl,SIGNAL(mbNumberExist(int)),&askDlg,SLOT(extenedAnswerMB(int)));
    connect (&askDlg,SIGNAL(isMBExist(QString)),d_ctrl,SLOT(checkMB(QString)));
    connect (d_ctrl,SIGNAL(mbNumberChecked(QString &)),&askDlg,SLOT(confirmMB(QString &)));
    connect (this,SIGNAL(printerSelected(QString)),d_ctrl,SLOT(setPrinter(QString)));
    //connect (askDlg,SIGNAL(printOverSidePage(int)),d_ctrl,SLOT(printOverSide(int)));
    //connect (askDlg,SIGNAL(printWithTemplate(int)),d_ctrl,SLOT(printWithTemplate(int)));
    /*

    connect (askDlg,SIGNAL(save2base(QString)),d_ctrl,SLOT(saveBase(QString)));

    connect (d_ctrl,SIGNAL(exchangeData2MB(QString)),askDlg,SLOT(parserDocData(QString)));
    */

}

void StartDlg::enableGUI()
{
    ui->printerCBox->setEnabled(true);
    ui->printOnMarkPaperBtn->setEnabled(true);
    ui->printWithAutoMarkPaperBtn->setEnabled(true);
    ui->markPaperBtn->setEnabled(true);
}

 void StartDlg::convertToPDF(QString &in_file)
 {
    d_ctrl->convert2PDF(in_file);
 }

StartDlg::~StartDlg()
{
    d_ctrl->write_settings();
    delete ui;
}

//	    SLOTS
void StartDlg::error(int ErrorCode,QString ErrorString)
 {
    if (ErrorCode != QProcess::NormalExit) {
	    QMessageBox::critical(this, QObject::trUtf8("Обратитесь к системному администратору"),
			     QObject::trUtf8("Ошибка предпечатной подготовки.\n%1:\nКод %2").arg(ErrorString).arg(ErrorCode,0,10));
	    this->close(); // TODO как правильно завершить приложение?
	}
 }
void StartDlg::convertDone()
{
    cnv=true;
    ui->logList->addItem(QString("%1 Успешно завершена конвертация исходного файла в pdf.").arg(QDateTime::currentDateTime ().toString("dd.MM.yyyy hh:mm:ss")));
}
void StartDlg::mergeDone()
{
    merge=true;
    ui->logList->addItem(QString("%1 Успешно завершено наложение шаблона.").arg(QDateTime::currentDateTime ().toString("dd.MM.yyyy hh:mm:ss")));
}

void StartDlg::connectToDemon()
{
    ui->logList->addItem(QString("%1 Регистрация на сервере безопасности произведена.").arg(QDateTime::currentDateTime ().toString("dd.MM.yyyy hh:mm:ss")));
    this->enableGUI();
}
