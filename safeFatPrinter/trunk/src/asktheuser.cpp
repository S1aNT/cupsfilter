#include "asktheuser.h"
#include "ui_asktheuser.h"

askTheUser::askTheUser(QWidget *parent) :
    QDialog(parent),
m_ui(new Ui::askTheUser)
{
    m_ui->setupUi(this);
    this->selectWorkPage();
     isDocChecked= false;
     timer = new QTimer(this);

     connect(timer, SIGNAL(timeout()), this, SLOT(enablePrintButton()));
     timer->start(1000);
    MBcompleter = new QCompleter(this);
    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

    signalMapper = new QSignalMapper(this);
    //signalMapper->setMapping(m_ui->docName_plainTextEdit, QString("название документа"));
    //signalMapper->setMapping(m_ui->secretCBox, QString("гриф секретности"));
    signalMapper->setMapping(m_ui->copyNumberLineEd,0);
    signalMapper->setMapping(m_ui->punktLineEd, 1);
    signalMapper->setMapping(m_ui->reciverOne_lineEd, 2);
    signalMapper->setMapping(m_ui->reciverTwo_lineEd, 3);
    signalMapper->setMapping(m_ui->reciverThree_lineEd, 4);
    signalMapper->setMapping(m_ui->reciverFour_lineEd,5);
    signalMapper->setMapping(m_ui->reciverFive_lineEd,6);
    signalMapper->setMapping(m_ui->executor_lineEd, 7);
    signalMapper->setMapping(m_ui->pressman_lineEd, 8);
    signalMapper->setMapping(m_ui->telephone_lineEd, 9);
    signalMapper->setMapping(m_ui->invNumber_lineEd, 10);
    
    //signalMapper->setMapping(m_ui->templatesCbox, QString("шаблон"));
    signalMapper->setMapping(m_ui->pageCountEd, 11);
    signalMapper->setMapping(m_ui->dateField_dateEd, 12);
    createConnections();
    m_ui->dateField_dateEd->setDate(QDate::currentDate());

}

void askTheUser::isFileldNotEmpty(int sender_id)
{
    QString msg;
    QString info;
    QLineEdit *editor;
    switch (sender_id){
    case 0:
	editor =(QLineEdit *)signalMapper->mapping(sender_id);
	 if (editor->text().isEmpty()){
	     msg=QString(QObject::trUtf8("Поле [Число копий] не может быть пустым!"));
	     qDebug() <<Q_FUNC_INFO<< sender_id <<editor->text() <<editor->text().isEmpty() << msg;

	 }
	break;
    case 1:	 
	editor =(QLineEdit *)signalMapper->mapping(sender_id);
	 if (editor->text().isEmpty()){
	    msg=QString(QObject::trUtf8("Поле [Пункт] не может быть пустым!"));
	 }
	break;
    case 2:
	editor =(QLineEdit *)signalMapper->mapping(sender_id);
	 if (editor->text().isEmpty()){
	     msg=QString(QObject::trUtf8("Поле [Получатель №1] не может быть пустым!"));
	 }
	break;

    case 3:
	editor =(QLineEdit *)signalMapper->mapping(sender_id);
	 if (editor->text().isEmpty()){
	    msg=QString(QObject::trUtf8("Поле [Получатель №2] не может быть пустым!"));
	 }
	break;
    case 4:
	editor =(QLineEdit *)signalMapper->mapping(sender_id);
	 if (editor->text().isEmpty()){
	msg=QString(QObject::trUtf8("Поле [Получатель №3] не может быть пустым!"));
	 }
	break;
    case 5:
	editor =(QLineEdit *)signalMapper->mapping(sender_id);
	 if (editor->text().isEmpty()){
	msg=QString(QObject::trUtf8("Поле [Получатель №4] не может быть пустым!"));
	 }
	break;
    case 6:
	editor =(QLineEdit *)signalMapper->mapping(sender_id);
	 if (editor->text().isEmpty()){
	    msg=QString(QObject::trUtf8("Поле [Получатель №5] не может быть пустым!"));
	 }
	break;
    case 7:
	editor =(QLineEdit *)signalMapper->mapping(sender_id);
	 if (editor->text().isEmpty()){
	  msg=QString(QObject::trUtf8("Поле [Исполнитель] не может быть пустым!"));
	 }
	break;	
    case 8:
	editor =(QLineEdit *)signalMapper->mapping(sender_id);
	 if (editor->text().isEmpty()){
	  msg=QString(QObject::trUtf8("Поле [Отпечатал] не может быть пустым!"));
	 }
	break;
    case 9:
	editor =(QLineEdit *)signalMapper->mapping(sender_id);
	 if (editor->text().isEmpty()){
		msg=QString(QObject::trUtf8("Поле [№ телефона] не может быть пустым!"));
	 }
	break;
    case 10:
	editor =(QLineEdit *)signalMapper->mapping(sender_id);
	 if (editor->text().isEmpty()){
	    msg=QString(QObject::trUtf8("Поле [Инв. номер] не может быть пустым!"));
	 }
	break;
    case 11:
	editor =(QLineEdit *)signalMapper->mapping(sender_id);
	 if (editor->text().isEmpty()){
	    msg=QString(QObject::trUtf8("Поле [Число страниц] не может быть пустым!"));
	 }
	break;
    case 12:
	 QDateEdit *date_edit =(QDateEdit *)signalMapper->mapping(sender_id);
	 if (!date_edit->date().isValid()){
	    msg=QString(QObject::trUtf8("Поле [Дата печати] не верная дата!"));
	 }
	break;
    }
    if (!msg.isEmpty()){
	//msg.arg(QString(info));
	QMessageBox msgBox;
	msgBox.addButton(QObject::trUtf8("Продолжить"), QMessageBox::AcceptRole);
	msgBox.setText(msg);
	msgBox.exec();
	  m_ui->previewBtn->setEnabled(false);
    }else{
	if ( isDocChecked){
	    m_ui->previewBtn->setEnabled(true);
	}
    }
}

void askTheUser::enableInputFields(bool state)
{
  //Запретим все поля ввода кроме МБ
  m_ui->docName_plainTextEdit->setEnabled(state);
  m_ui->secretCBox->setEnabled(state);
  m_ui->copyNumberLineEd->setEnabled(state);
  m_ui->punktLineEd->setEnabled(state);
  m_ui->reciverOne_lineEd->setEnabled(state);
  m_ui->reciverTwo_lineEd->setEnabled(state);
  m_ui->reciverThree_lineEd->setEnabled(state);
  m_ui->reciverFour_lineEd->setEnabled(state);
  m_ui->reciverFive_lineEd->setEnabled(state);
  m_ui->executor_lineEd->setEnabled(state);
  m_ui->pressman_lineEd->setEnabled(state);
  m_ui->telephone_lineEd->setEnabled(state);
  m_ui->invNumber_lineEd->setEnabled(state);
  m_ui->dateField_dateEd->setEnabled(state);
  m_ui->templatesCbox->setEnabled(state);
}

void askTheUser::selectWorkPage()
{
    m_ui->stackedWidget->setCurrentIndex(0);
}
void askTheUser::selectInfoPage()
{
      m_ui->stackedWidget->setCurrentIndex(1);
}
void askTheUser::confirmMB(QString &mb)
{
    // Сервер подтвердил что документа с таким МБ в базе нет, покажем основную страницу,
    // в поле статус поставим значение НОВЫЙ
    this->selectWorkPage();
    documentStatus.setText(QObject::trUtf8("НОВЫЙ"));
    isDocChecked=true;
    currentMB=mb;
    QMessageBox msgBox;
    msgBox.setText("Успешная проверка. В базе аудита нет такой записи");
}
void askTheUser::extenedAnswerMB(int RowId)
{
    // Запрос не прошел проверку , в базе есть такая запись
    QMessageBox msgBox;
    QPushButton *contButton;
    QPushButton *abortButton;

    if (RowId==-1){
	msgBox.setText("Запрос не прошел проверку , в базе аудита есть такая запись");
	abortButton =msgBox.addButton(QObject::trUtf8("Очистить поле МБ"), QMessageBox::RejectRole);
    }else{
	msgBox.setText("В базе аудита есть такая запись, но документ бракованный");
	contButton =msgBox.addButton(QObject::trUtf8("Продолжить"), QMessageBox::AcceptRole);
	abortButton =msgBox.addButton(QObject::trUtf8("Очистить поле МБ"), QMessageBox::RejectRole);
    }
     msgBox.exec();


    if (msgBox.clickedButton() == contButton) {
	mapper->toFirst();
	for (int i=0;i<RowId;++i){
	    mapper->toNext();
	}
	isDocChecked= true;
     } else if (msgBox.clickedButton() == abortButton) {
	 isDocChecked= false;
	 m_ui->mbNumberLineEd->clear();
	 currentMB="";
     }

    this->selectWorkPage();
}


void askTheUser::checkMBdublicat()
{
    if (currentMB!= m_ui->mbNumberLineEd->text()){
	isDocChecked=false;
    }
    if (!isDocChecked){
	//Переключим на вторую страницу, покажем пользователя чудесное сообщение
	m_ui->informerTextEd->setText(QObject::trUtf8("Проверяю наличие регистрации в БД аудита документа с номером МБ: \n%1 .").arg(m_ui->mbNumberLineEd->text()));
	this->selectInfoPage();
	//TODO вставил для отображения своего шедевра
	QMessageBox msgBox;
	msgBox.setText("Идет проверка по БД аудита...");
	QPushButton *abortButton = msgBox.addButton(QMessageBox::Abort);
	QTimer::singleShot(2000,&msgBox,SLOT(accept()));
	int ret = msgBox.exec();
	if (ret == QDialog::Accepted){
	    emit isMBExist(m_ui->mbNumberLineEd->text());
	}else if (msgBox.clickedButton() == abortButton) {
	    m_ui->mbNumberLineEd->clear();
	    isDocChecked= false;
	    this->selectInfoPage();
	}
    }
}

void askTheUser::enablePrintButton()
{
    bool flag;
    /*
    if (!isDocChecked){
	flag=true;

    }else {

		if (m_ui->docName_plainTextEdit->toPlainText().isEmpty() ||
		    m_ui->copyNumberLineEd->text().isEmpty() ||
		    m_ui->punktLineEd->text().isEmpty() ){

		}

		  m_ui->reciverOne_lineEd->text().isEmpty() ||
		  m_ui->reciverTwo_lineEd->text().isEmpty() ||
		  m_ui->reciverThree_lineEd->text().isEmpty() ||
		  m_ui->reciverFour_lineEd ->text().isEmpty() ||
		 m_ui->reciverFive_lineEd->text().isEmpty() ||
	m_ui->executor_lineEd->text().isEmpty() ||
	m_ui->pressman_lineEd->text().isEmpty() ||
	m_ui->telephone_lineEd->text().isEmpty() ||
	m_ui->invNumber_lineEd->text().isEmpty() ||
    }
    if (flag){
	timer->start(1000);
    }else{
    }
    */
}


void askTheUser::createConnections()
{
    connect (m_ui->mbNumberLineEd,SIGNAL(editingFinished()),this,SLOT(checkMBdublicat()));
    // Свяжем все сигналы от полей ввода с маппером сигналов , а его соответсвенно с валидатором

    //connect(m_ui->docName_plainTextEdit, SIGNAL(editingFinished()),signalMapper, SLOT (map()));
    //connect(m_ui->secretCBox, SIGNAL(editingFinished()),signalMapper, SLOT (map()));
    //connect(m_ui->templatesCbox, SIGNAL(editingFinished()),signalMapper, SLOT (map()));

    connect(m_ui->copyNumberLineEd, SIGNAL(editingFinished()),signalMapper, SLOT (map()));
    connect(m_ui->punktLineEd, SIGNAL(editingFinished()),signalMapper, SLOT (map()));
    connect(m_ui->reciverOne_lineEd, SIGNAL(editingFinished()),signalMapper, SLOT (map()));
    connect(m_ui->reciverTwo_lineEd, SIGNAL(editingFinished()),signalMapper, SLOT (map()));
    connect(m_ui->reciverThree_lineEd, SIGNAL(editingFinished()),signalMapper, SLOT (map()));
    connect(m_ui->reciverFour_lineEd, SIGNAL(editingFinished()),signalMapper, SLOT (map()));
    connect(m_ui->reciverFive_lineEd, SIGNAL(editingFinished()),signalMapper, SLOT (map()));
    connect(m_ui->executor_lineEd, SIGNAL(editingFinished()),signalMapper, SLOT (map()));
    connect(m_ui->pressman_lineEd, SIGNAL(editingFinished()),signalMapper, SLOT (map()));
    connect(m_ui->telephone_lineEd, SIGNAL(editingFinished()),signalMapper, SLOT (map()));
    connect(m_ui->invNumber_lineEd, SIGNAL(editingFinished()),signalMapper, SLOT (map()));
    connect(m_ui->dateField_dateEd, SIGNAL(editingFinished()),signalMapper, SLOT (map()));

    connect(m_ui->pageCountEd, SIGNAL(editingFinished()),signalMapper, SLOT (map()));
    // Подключим маппер к валидатору
    //connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(isFileldNotEmpty(int)));
}
void askTheUser::setModel (QStandardItemModel * model)
{
  mainModel = model;
  //mainModel->clear();

  QStringList stamp_items;
  stamp_items << QObject::trUtf8("Секретно") << QObject::trUtf8("Сов. секретно") << QObject::trUtf8("Особой Важности");
  QStringListModel *stampModel = new QStringListModel(stamp_items, this);
  m_ui->secretCBox->setModel(stampModel);

  //Свяжем  элементы диалогового окна с моделью через mapper
  mapper->setModel(model);
  mapper->toLast();

  for (int i = 0; i < model->columnCount(); i++)
    {
      QStandardItem * header_item = model->horizontalHeaderItem(i);
      QString header = header_item->data(Qt::EditRole).toString();

      if (header.compare(QObject::trUtf8("Номер док-та")) == 0)
        {
          mapper->addMapping(m_ui->mbNumberLineEd, i);
        }
      else if (header.compare(QObject::trUtf8("Название док-та")) == 0)
        {
          mapper->addMapping(m_ui->docName_plainTextEdit, i);
        }
      else if (header.compare(QObject::trUtf8("Гриф")) == 0)
        {
          mapper->addMapping(m_ui->secretCBox, i, "currentIndex");
        }
      else if (header.compare(QObject::trUtf8("Пункт перечня"))==0)
        {
          mapper->addMapping(m_ui->punktLineEd, i);
        }
      else if (header.compare(QObject::trUtf8("Номер копии"))==0)
        {
	  mapper->addMapping(m_ui->copyNumberLineEd, i);
	}else if (header.compare(QObject::trUtf8("Кол-во листов"))==0)
	{
	  mapper->addMapping(m_ui->pageCountEd, i);
	}
      else if (header.compare(QObject::trUtf8("Штамп последней стр."))==0)
        {
	  mapper->addMapping(m_ui->lastPageGroupBoxChecked, i,"checked");
	}
      else if (header.compare(QObject::trUtf8("Исполнитель"))==0)
       {
	 mapper->addMapping(m_ui->executor_lineEd, i);
       }else if (header.compare(QObject::trUtf8("Отпечатал"))==0)
       {
	 mapper->addMapping(m_ui->pressman_lineEd, i);
       } else if (header.compare(QObject::trUtf8("Телефон"))==0)
       {
	 mapper->addMapping(m_ui->telephone_lineEd, i);
       } else if (header.compare(QObject::trUtf8("Инв. №"))==0)
       {
	 mapper->addMapping(m_ui->invNumber_lineEd, i);
       }  else if (header.compare(QObject::trUtf8("Дата распечатки"))==0)
       {
	 mapper->addMapping(m_ui->dateField_dateEd, i);
       }
      else if (header.compare(QObject::trUtf8("Список рассылки"))==0)
       {
	 mapper->addMapping(m_ui->reciversListGroupBoxChecked, i,"checked");
       }else if (header.compare(QObject::trUtf8("Получатель №1"))==0)
       {
	 mapper->addMapping(m_ui->reciverOne_lineEd, i);
       }else if (header.compare(QObject::trUtf8("Получатель №2"))==0)
       {
	 mapper->addMapping(m_ui->reciverTwo_lineEd, i);
       }else if (header.compare(QObject::trUtf8("Получатель №3"))==0)
       {
	 mapper->addMapping(m_ui->reciverThree_lineEd, i);
       }else if (header.compare(QObject::trUtf8("Получатель №4"))==0)
       {
	 mapper->addMapping(m_ui->reciverFour_lineEd, i);
       }else if (header.compare(QObject::trUtf8("Получатель №5"))==0)
       {
	 mapper->addMapping(m_ui->reciverFive_lineEd, i);
       }else if (header.compare(QObject::trUtf8("Статус документа"))==0)
       {
	   // Мапим скрытое поле статус документа
	   mapper->addMapping(&documentStatus, i);
       }
  }
}
void askTheUser::setViewMode(int work_mode)
{
    currentMode=work_mode;
    if (currentMode==2){
	m_ui->previewBtn->setText(QObject::trUtf8("Печать на учтенных листах"));
	this->enableInputFields(false);
    }
}


void askTheUser::accept()
{
    if (currentMode==1){
	 emit printOverSidePage(mapper->currentIndex());
    }else if (currentMode==2){
		    emit printWithTemplate(mapper->currentIndex());//TODO должен быть шаблон
	     }
}

void askTheUser::setMBCompleter(QStringList &cmp)
{
    MBcompleter->setModel(new QStringListModel(cmp));
    MBcompleter->setCaseSensitivity(Qt::CaseInsensitive);
    m_ui->mbNumberLineEd->setCompleter(MBcompleter);
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
	    //save2field(doc_data);
	} else if (msgBox.clickedButton() == abortButton) {
	    m_ui->mbNumberLineEd->clear();
	}
    }else if (currentMode==2){
		m_ui->previewBtn->setEnabled(true);
		// Разбор полей и запись в нужные места
		//save2field(doc_data);
	    }
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
