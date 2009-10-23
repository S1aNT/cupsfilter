#include "asktheuser.h"
#include "ui_asktheuser.h"

askTheUser::askTheUser(QWidget *parent) :
    QDialog(parent),
m_ui(new Ui::askTheUser)
{
    m_ui->setupUi(this);
    MBcompleter = new QCompleter(this);
    connect (m_ui->docName_plainTextEdit,SIGNAL(textChanged()),this,SLOT(docNameChanged()));
    connect (m_ui->secretCBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(secretStampChanged(QString)));
    connect (m_ui->copyNumberSpinBox,SIGNAL(valueChanged(int)),this,SLOT(copyNumberChanged(int)));
    connect (m_ui->punktLineEd,SIGNAL(textChanged(QString)),this,SLOT(punktChanged(QString)));
    connect (m_ui->reciverOne_lineEd,SIGNAL(textChanged(QString)),this,SLOT(reciverOneChanged(QString)));
    connect (m_ui->reciverTwo_lineEd,SIGNAL(textChanged(QString)),this,SLOT(reciverTwoChanged(QString)));
    connect (m_ui->reciverThree_lineEd,SIGNAL(textChanged(QString)),this,SLOT(reciverThreeChanged(QString)));
    connect (m_ui->reciverFour_lineEd,SIGNAL(textChanged(QString)),this,SLOT(reciverFourChanged(QString)));
    connect (m_ui->reciverFive_lineEd,SIGNAL(textChanged(QString)),this,SLOT(reciverFiveChanged(QString)));
    connect (m_ui->executor_lineEd,SIGNAL(textChanged(QString)),this,SLOT(executorChanged(QString)));
    connect (m_ui->pressman_lineEd,SIGNAL(textChanged(QString)),this,SLOT(pressmanChanged(QString)));
    connect (m_ui->telephone_lineEd,SIGNAL(textChanged(QString)),this,SLOT(phoneChanged(QString)));
    connect (m_ui->invNumber_lineEd,SIGNAL(textChanged(QString)),this,SLOT(invNumberChanged(QString)));
    connect (m_ui->dateField_dateEd,SIGNAL(dateChanged(QDate)),this,SLOT(dateFiledChanged(QDate)));
    connect (m_ui->mbNumberLineEd,SIGNAL(textChanged(QString)),this,SLOT(mbNumChanged(QString)));
    connect (m_ui->templatesCbox,SIGNAL(currentIndexChanged(QString)),this,SLOT(templatesChanged(QString)));
    m_ui->dateField_dateEd->setDate(QDate::currentDate());
}

void askTheUser::setMode(int work_mode)
{
    currentMode=work_mode;
    if (currentMode==2){
	m_ui->previewBtn->setText(QObject::trUtf8("Печать на учтенных листах"));
	//Запретим все поля ввода кроме МБ
	m_ui->docName_plainTextEdit->setEnabled(false);
m_ui->secretCBox->setEnabled(false);
m_ui->copyNumberSpinBox->setEnabled(false);
m_ui->punktLineEd->setEnabled(false);
m_ui->reciverOne_lineEd->setEnabled(false);
m_ui->reciverTwo_lineEd->setEnabled(false);
m_ui->reciverThree_lineEd->setEnabled(false);
m_ui->reciverFour_lineEd->setEnabled(false);
m_ui->reciverFive_lineEd->setEnabled(false);
m_ui->executor_lineEd->setEnabled(false);
m_ui->pressman_lineEd->setEnabled(false);
m_ui->telephone_lineEd->setEnabled(false);
m_ui->invNumber_lineEd->setEnabled(false);
m_ui->dateField_dateEd->setEnabled(false);
m_ui->templatesCbox->setEnabled(false);
//m_ui->mbNumberLineEd
    }

}
void askTheUser::docNameChanged()
{
    docName=m_ui->docName_plainTextEdit->document()->toPlainText();
}

void askTheUser::accept()
{

if (currentMode==1){
    QString msg;
    if (docName.isEmpty()){
	msg=QObject::trUtf8( "Название документа должно быть заполнено");
    }else if (secretStamp.isEmpty()){
		msg=QObject::trUtf8("Гриф секретности документа должен быть выбран");
	}else if (punkt.isEmpty()){
		    msg=QObject::trUtf8("Поле Пункт  должно быть заполнено");
		}else if (mbNumber.isEmpty()){
			    msg=QObject::trUtf8("Поле номер МБ должно быть заполнено");
			}else if (copyNumber ==0){
				    msg=QObject::trUtf8("Номер копии должен быть больше 0");
			    }else if (templ.isEmpty()){
				    msg=QObject::trUtf8("Шаблон документа должен быть выбран");
				}else if (m_ui->reciversList_chBox->checkState()){
					    if (reciver_1.isEmpty() &&  reciver_2.isEmpty() &&
						reciver_3.isEmpty() &&  reciver_4.isEmpty() &&  reciver_5.isEmpty()){
						msg=QObject::trUtf8("Не задан ни один адресат в списке рассылки");
					    }
				    }else if (m_ui->clisheOnLastPageCheckBox->checkState()){
						if (executor.isEmpty()){
						    msg= QObject::trUtf8("Поле исполнитель должно быть заполнено");
						}else if(pressman.isEmpty()){
							    msg=QObject::trUtf8("Поле отпечатал должно быть заполнено");
						    }else if (phoneNumber.isEmpty()){
								msg= QObject::trUtf8("Поле телефон должно быть заполнено");
							}else if(invNumber.isEmpty()){
								    msg= QObject::trUtf8("Поле Инв. номер должно быть заполнено");
								}else if (!date.isValid()){
									    msg= QObject::trUtf8("Поле дата содержит неверную дату или не  заполнено");
								    }
					}
	if (msg.isEmpty()){
		QDialog::accept();
		// запись в базу
		QString msg;
		emit save2base(msg);
		 emit printOverSidePage(m_ui->mbNumberLineEd->text());
	}else{
	    QMessageBox msgBox;
	    msgBox.setText(msg);
	    msgBox.exec();
	}


	}else if (currentMode==2){
	    QDialog::accept();
	    qDebug() << Q_FUNC_INFO;
		    emit printWithTemplate(m_ui->mbNumberLineEd->text());//TODO должен быть шаблон
		}

}

void askTheUser::setMBCompleter(QStringList &cmp)
{
    MBcompleter->setModel(new QStringListModel(cmp));
    MBcompleter->setCaseSensitivity(Qt::CaseInsensitive);
    m_ui->mbNumberLineEd->setCompleter(MBcompleter);
}
void askTheUser::mbNumChanged(QString mb)
{
    //m_ui->previewBtn->setEnabled(true);
    mbNumber =mb;
    emit isMBExist(mb); // запрос на сервер на наличие документа в базе
}

void askTheUser::parserDocData(QString doc_data)
{
    if (currentMode==1){
	QMessageBox msgBox;
	msgBox.setText("В базе уже существует документ с таким МБ");
	msgBox.setInformativeText("Вы хотите допечать или перепечатать его?");
	QPushButton *printButton  = msgBox.addButton(QObject::trUtf8("Согласен"), QMessageBox::ActionRole);
	QPushButton *abortButton = msgBox.addButton(QObject::trUtf8("Отмена"),   QMessageBox::RejectRole);
	msgBox.exec();

	if (msgBox.clickedButton() == printButton) {
	    // connect
	    m_ui->previewBtn->setEnabled(true);
	    // Разбор полей и запись в нужные места
	    save2field(doc_data);
	} else if (msgBox.clickedButton() == abortButton) {
	    m_ui->mbNumberLineEd->clear();
	}
    }else if (currentMode==2){
		m_ui->previewBtn->setEnabled(true);
		// Разбор полей и запись в нужные места
		save2field(doc_data);
	    }
}

void  askTheUser::save2field(QString &data)
{
     QStringList list;
     list = data.split("~");
     for (int i = 0; i < list.size(); ++i){
	 QRegExp rx("^(.+)=(.+)$");
	 if (rx.indexIn(list.at(i))!=-1) {
	     QString key =rx.cap(1);
	     QString value = rx.cap(2);
	 // Говонокод но пока не знаю как сделать лучше
	     if (key=="docName"){
		 m_ui->docName_plainTextEdit->setPlainText(value);
	     }else if (key=="secretStamp"){
			m_ui->secretCBox->setCurrentIndex(m_ui->secretCBox->findText(value));
		    }else if (key=="punkt"){
				m_ui->punktLineEd->setText(value);
			    }else if (key=="mbNumber"){
					m_ui->mbNumberLineEd->setText(value);
				    }else if (key=="templ"){
						m_ui->templatesCbox->setCurrentIndex(m_ui->templatesCbox->findText(value));
					    }else if (key=="invNumber"){
						    m_ui->invNumber_lineEd->setText(value);
						}else if (key=="reciver_1"){
							    m_ui->reciversList_chBox->setChecked(true);
							    m_ui->reciverOne_lineEd->setText(value);
							}else if (key=="reciver_2"){
								    m_ui->reciverTwo_lineEd->setText(value);
								}else if (key=="reciver_3"){
									    m_ui->reciverThree_lineEd->setText(value);
									}else if (key=="reciver_4"){
										    m_ui->reciverFour_lineEd->setText(value);
										}else if (key=="reciver_5"){
											    m_ui->reciverFive_lineEd->setText(value);
											}else if (key=="executor"){
												    m_ui->executor_lineEd->setText(value);
												    //TODO заменить на нормальное имя бокса
												    m_ui->groupBox_2->setEnabled(true);
												    m_ui->clisheOnLastPageCheckBox->setChecked(true);
												}else if (key=="pressman"){
													    m_ui->pressman_lineEd->setText(value);
													}else if (key=="phoneNumber"){
														    m_ui->telephone_lineEd->setText(value);
														}else if (key=="copyNumber"){
															    m_ui->copyNumberSpinBox->setValue(value.toInt());
															}
													/*
													else if (key=="date"){
															    m_ui->dateField_dateEd->setDate(value);
															}*/
	 }
     }
}
void askTheUser::executorChanged(QString fio)
{
    executor =fio;
}
void askTheUser::pressmanChanged(QString fio)
{
    pressman=fio;
}
void askTheUser::phoneChanged(QString phone)
{
    phoneNumber =phone;
}
void askTheUser::invNumberChanged(QString iNumber)
{
    invNumber=iNumber;
}
void askTheUser::dateFiledChanged(QDate DT)
{
    date =DT;
}
void askTheUser::reciverOneChanged(QString res)
{
    reciver_1=res;
}
void askTheUser::reciverTwoChanged(QString res)
{
    reciver_2=res;
}
void askTheUser::reciverThreeChanged(QString res)
{
    reciver_3=res;
}
void askTheUser::reciverFourChanged(QString res)
{
    reciver_4=res;
}
void askTheUser::reciverFiveChanged(QString res)
{
    reciver_5=res;
}
void askTheUser::copyNumberChanged(int cpNum)
{
    copyNumber=cpNum;
}
void askTheUser::templatesChanged(QString Templates)
{
    templ=Templates;
}
void askTheUser::secretStampChanged(QString stamp)
{
    secretStamp=stamp;
}
void askTheUser::punktChanged(QString punktStr)
{
    punkt =punktStr;
}
askTheUser::~askTheUser()
{
    delete m_ui;
}
void askTheUser::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
