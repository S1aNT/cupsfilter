#include "startdlg.h"
#include "ui_startdlg.h"

StartDlg::StartDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartDlg)
{
    ui->setupUi(this);
    askDlg = new AskTheUser(this);
    wrkDlg = new workReport(this);
    connectStep=false;
    convertStep=false;
    control = new dController(this);
    signalMapper = new QSignalMapper(this);
    createConnection();
    control->connect2Demon();
}


void StartDlg::createConnection()
{
    // маппинг сигналов
     connect(ui->markPaperBtn, SIGNAL(clicked()), signalMapper, SLOT(map()));
     connect(ui->printOnMarkPaperBtn, SIGNAL(clicked()), signalMapper, SLOT(map()));
     connect(ui->printWithAutoMarkPaperBtn, SIGNAL(clicked()), signalMapper, SLOT(map()));
     signalMapper->setMapping(ui->markPaperBtn,0 );
     signalMapper->setMapping(ui->printOnMarkPaperBtn,1);
     signalMapper->setMapping(ui->printWithAutoMarkPaperBtn,2 );
     connect(signalMapper, SIGNAL(mapped(int)),  this, SLOT(fill_docCard4Print(int)));
     connect(control, SIGNAL(init_done(int,QString &)), this, SLOT(enableGUI(int,QString &)));
}

void StartDlg::convertToPDF(QString &filename)
{
    control->convert2PDF(filename);
    QString msg;
    msg=QObject::trUtf8("Файл %1 отправлен на конвертацию").arg(filename);
    this->printToLog(msg);
}

void StartDlg::printToLog(QString & log_mes)
{
    ui->logList->addItem(QString("%1: %2").arg(QDateTime::currentDateTime ().toString("dd.MM.yyyy hh:mm:ss")).arg(log_mes));
    ui->logList->update();
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
}

//************************************      SLOTS     *********************************************

void StartDlg::fill_docCard4Print(int Mode)
{  // покажем диалог ввода или выбора данных
    int ret = askDlg->exec();
    if (ret ==QDialog::Accepted){
	switch (Mode){
	    case 0:
		if (askDlg->isCheckedPAO()){
		    //emit saveModelToBase(askDlg->getCurrentModelIndex());

		}else{
		    //emit PrintOutSideMark(askDlg->getCurrentModelIndex());
		}
		break;
	    case 1:

		break;
	    case 2:
		break;
	}
	// Покажем завершающий диалог
	wrkDlg->exec();

    }else{
	// clearModel
    }
}
void StartDlg::enableGUI(int step,QString &message)
{

    qDebug() << step << message;
    if (step==CONN_STEP){
	connectStep=true;
    }else  if (step==CONVERT_STEP){
	convertStep=true;
    }
    this->printToLog(message);
    if (connectStep && convertStep){
	ui->printerCBox->setEnabled(true);
	ui->printOnMarkPaperBtn->setEnabled(true);
	ui->printWithAutoMarkPaperBtn->setEnabled(true);
	ui->markPaperBtn->setEnabled(true);
    }
}

//************************************ END SLOTS *********************************************


StartDlg::~StartDlg()
{
    delete ui;
}

void StartDlg::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
