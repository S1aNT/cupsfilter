#include "startdlg.h"
#include "ui_startdlg.h"

StartDlg::StartDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartDlg)
{
    ui->setupUi(this);

    askDlg = new AskTheUser(this);
    wrkDlg = new workReport(this);
    templViewer = new View(this);
    //templViewer->hide();
    connectStep=false;
    convertStep=false;

    control = new dController(this);
    signalMapper = new QSignalMapper(this);
    createConnection();
    rabbitHole();
    control->connect2Demon();

    QString msg;
    if (control->isUser_Mandat()){
	msg= QObject::trUtf8("Успешно получили имя пользователя и его мандат");
	getMandatUserStep=true;

    }else{
	getMandatUserStep=false;
	msg= QObject::trUtf8("Ошибка получения имени пользователя или его мандата");
    }
    this->printToLog(msg);

}


void StartDlg::convertToPDF(QString &filename)
{
    control->convert2PDF(filename);
    QString msg;
    msg=QObject::trUtf8("Файл %1 отправлен на конвертацию").arg(filename);
    this->printToLog(msg);
}

//************************************      SLOTS     *********************************************




// --------------------------------------------------------------- private slots --------------------------------------------------------------------------------


void StartDlg::fill_docCard4Print(int Mode)
{  // покажем диалог ввода или выбора данных
    askDlg->setWorkMode(Mode);
    askDlg->setModel(control->document_model());
    askDlg->setStampModel(control->stamp_model());
    askDlg->setTemplatesPath(control->glob_templates());

    int ret = askDlg->exec();
    if (ret ==QDialog::Accepted){

	QString in =control->mainPDF;
	QString bg_file="d:\\compl_teml2.pdf";
	QString out_file="d:\\out.pdf";
	 control->mergeTwoPDF(in,bg_file,out_file);

	switch (Mode){
	    case 0:
		if (askDlg->isCheckedPAO()){
		    control->printWithTemplate(1);
		    QString in =control->mainPDF;
		    QString bg_file="d:\\compl_teml2.pdf";
		    QString out_file="d:\\out.pdf";
		     control->mergeTwoPDF(in,bg_file,out_file);
		    //emit saveModelToBase(askDlg->getCurrentModelIndex());

		}else{
		    //emit PrintOutSideMark(askDlg->getCurrentModelIndex());
		    control->printOverSide(1);
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

void StartDlg::enableGUI(steps_t step,QString &message)
{
    if (step==CONN_STEP){

	connectStep=true;
    }else  if (step==CONVERT_STEP){
	convertStep=true;
    }
    if (connectStep && convertStep && getMandatUserStep){
	ui->printerCBox->addItems(control->printers_list());
	ui->printerCBox->setEnabled(true);
	ui->printOnMarkPaperBtn->setEnabled(true);
	ui->printWithAutoMarkPaperBtn->setEnabled(true);
	ui->markPaperBtn->setEnabled(true);
    }
     this->printToLog(message);
}

void StartDlg::showErrorInfo()
{

    QMessageBox msgBox(this);
     QPushButton *abortButton;
    msgBox.setObjectName("crit_msg_box");
    msgBox.setIcon(QMessageBox::Critical);

    QString msg=QObject::trUtf8("Для пользователя %1 нет доступных принтеров. ").arg("sva");
    msgBox.setInformativeText(QObject::trUtf8("Для решения этой проблемы обратитесь к администратору безопасности!"));
    abortButton=msgBox.addButton(QObject::trUtf8("Выход"), QMessageBox::RejectRole);
    msgBox.setText(msg);
    msgBox.exec();
    if (msgBox.clickedButton()==abortButton){
	QCoreApplication::quit();
    }
}

void StartDlg::do_showTemplatesEditor()
{

    askDlg->hide();
    templViewer->exec();
}

StartDlg::~StartDlg()
{
    delete ui;
}
//************************************ END SLOTS *********************************************

// --------------------------------------------------------------- protected function-------------------------------------------------------------------------
void StartDlg::rabbitHole()
{
    // Сигналы от контроллера
    // Машина состояний
    connect (control, SIGNAL(init_done(steps_t,QString &)), this, SLOT(enableGUI(steps_t,QString &)));
    connect (control,SIGNAL(printersNotFound()),this,SLOT(showErrorInfo()));
    connect (control,SIGNAL(mbNumberExist(int)),askDlg,SLOT(do_mbExist(int)));
    connect (control,SIGNAL(mbNumberNotExist()),askDlg,SLOT(do_mbNumberNotExist()));
    // Сигналы от вопросника
    connect (askDlg,SIGNAL(needCheckMB(QString )),control,SLOT(checkMB(QString )));
    connect (askDlg,SIGNAL(showTemplatesEditor()),this,SLOT(do_showTemplatesEditor()));
    // Сигналы от диалога_полследнего слова
    // Сигналы от редактора шаблонов

}

void StartDlg::printToLog(QString & log_mes)
{
    ui->logList->addItem(QString("%1: %2").arg(QDateTime::currentDateTime ().toString("dd.MM.yyyy hh:mm:ss")).arg(log_mes));
    ui->logList->update();
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
