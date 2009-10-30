#include "startdlg.h"
#include "ui_startdlg.h"

StartDlg::StartDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartDlg)
{
    ui->setupUi(this);
    connectStep=false;
    convertStep=false;
    control = new dController();
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

void StartDlg::readPrinterList()
{

}

//************************************      SLOTS     *********************************************

void StartDlg::fill_docCard4Print(int Mode)
{
    QString title;
    switch (Mode){
    case 0:
	title=QObject::trUtf8("Предварительный учет документа");

	break;
    case 1:
	break;
    case 2:
	break;
    }
    // покажем диалог ввода или выбора данных

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
