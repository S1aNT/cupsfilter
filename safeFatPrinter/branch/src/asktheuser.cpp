#include "asktheuser.h"
#include "ui_asktheuser.h"


AskTheUser::AskTheUser(QWidget *parent) :
    QDialog(parent),
   m_ui(new Ui::AskTheUser)
{
    m_ui->setupUi(this);
    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

    signalMapper = new QSignalMapper(this);
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
    signalMapper->setMapping(m_ui->pageCountEd, 11);
    signalMapper->setMapping(m_ui->dateField_dateEd, 12);
    //signalMapper->setMapping(m_ui->docName_plainTextEdit, QString("название документа"));
    //signalMapper->setMapping(m_ui->secretCBox, QString("гриф секретности"));
    //signalMapper->setMapping(m_ui->templatesCbox, QString("шаблон"));

    createConnections();
    m_ui->dateField_dateEd->setDate(QDate::currentDate());
    this->emptyFieldsExist=true;
}

void AskTheUser::createConnections()
{
    connect(m_ui->paperAccountsOutSide,SIGNAL(toggled(bool)),this,SLOT(changePAOLabel(bool)));
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

    connect(m_ui->mbNumberLineEd,SIGNAL(editingFinished()),this,SLOT(checkMB()));
    connect(m_ui->switch_Local_GlolbalBtn,SIGNAL(toggled(bool)),this,SLOT(selectLocalTemplatesDir(bool)));
    connect(m_ui->editTemplatesTBtn,SIGNAL(clicked()),this,SIGNAL(showTemplatesEditor()));
}



void AskTheUser::checkMB()
{
    if (!m_ui->mbNumberLineEd->text().isEmpty() ){
	QString stat= this->documentStatus.text();
	if (stat != QObject::trUtf8("checking") || stat !=QObject::trUtf8("check") || stat !=QObject::trUtf8("clear")){
	    this->documentStatus.setText(QObject::trUtf8("checking"));
	    emit needCheckMB(m_ui->mbNumberLineEd->text());
	    QString msg;
	    msg=QObject::trUtf8("Идет проверка номера МБ - [%1] по базе аудита").arg(m_ui->mbNumberLineEd->text());

	    //this->showAndClear(msg,false);
	   QMessageBox msgBox(this);
	   msgBox.setObjectName("info_msg_box");
	    msgBox.setText(msg);
	    msgBox.addButton(QObject::trUtf8("Очистить поле МБ"),QMessageBox::RejectRole);
	    msgBox.exec();
	}
    }
}
void AskTheUser::do_mbExist(int RowId)
{
    QString msg;
    // Закроем окошко
   QMessageBox*m_box = this->findChild<QMessageBox *>("info_msg_box");
    if (m_box->isActiveWindow()){
	QTimer::singleShot(500,m_box,SLOT(close()));
    }
    QMessageBox msgBox(this);
    QPushButton *contButton;
    QPushButton *abortButton;

    msgBox.setObjectName("info_msg_box");

    if (RowId==DOC_PRINTED){
	msg=QObject::trUtf8("Документ с номером МБ - [%1] был успешно распечатан.").arg(m_ui->mbNumberLineEd->text());
    }else{
	msg=QObject::trUtf8("Документ с номером МБ - [%1] зарегистрирован в базе аудита как бракованный ").arg(m_ui->mbNumberLineEd->text());
	msgBox.setInformativeText(QObject::trUtf8("Вы хотите продолжить работу с этим документом?"));
	contButton=msgBox.addButton(QObject::trUtf8("Продолжить работу"), QMessageBox::AcceptRole);
	currentRow=RowId;
    }
    abortButton=msgBox.addButton(QObject::trUtf8("Очистить поле МБ"),QMessageBox::RejectRole);
    msgBox.setText(msg);
    msgBox.exec();
    if (msgBox.clickedButton() == contButton) {
	//переключим на отображение данных из строки модели == currentRow;
	for (int i=0;i<currentRow;i++){
	    this->mapper->toNext();
	}
	 this->documentStatus.setText(QObject::trUtf8("check"));
	 this->update();
	  m_ui->previewBtn->setEnabled(true);
     }else if (msgBox.clickedButton()==abortButton){
	    m_ui->mbNumberLineEd->clear();
	    this->documentStatus.setText("clear");
    }
}
void AskTheUser::do_mbNumberNotExist()
{
    QString msg;
    // Закроем окошко
   QMessageBox*m_box = this->findChild<QMessageBox *>("info_msg_box");
    if (m_box->isActiveWindow()){
	QTimer::singleShot(1500,m_box,SLOT(close()));
    }
    QMessageBox msgBox(this);
    msgBox.setObjectName("info_msg_box");
    msg=QObject::trUtf8("Документ с номером МБ - [%1] в базе аудита не зарегистрирован ").arg(m_ui->mbNumberLineEd->text());
    msgBox.addButton(QObject::trUtf8("Продолжить работу"), QMessageBox::AcceptRole);
    msgBox.setText(msg);
    msgBox.exec();
    QTimer::singleShot(2500,&msgBox,SLOT(accept()));
    if (!emptyFieldsExist){
	    m_ui->previewBtn->setEnabled(true);
    }
}

void AskTheUser::isFileldNotEmpty(int sender_id)
{
    QString msg;
    QString info;
    QLineEdit *editor;
    switch (sender_id){
    case 0:
	editor =(QLineEdit *)signalMapper->mapping(sender_id);
	 if (editor->text().isEmpty()){
	     msg=QString(QObject::trUtf8("Поле [Число копий] не может быть пустым!"));
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
	emptyFieldsExist=false;

    }else {
	emptyFieldsExist=true;
    }
}

void AskTheUser::enableInputFields(bool state)
{
  m_ui->docName_plainTextEdit->setEnabled(state);
  m_ui->secretCBox->setEnabled(state);
  m_ui->copyNumberLineEd->setEnabled(state);
  m_ui->punktLineEd->setEnabled(state);
  m_ui->templatesCbox->setEnabled(state);
}

void AskTheUser::setTemplatesPath(QString global)
{
    global_templates_dir=global;
}

void AskTheUser::selectLocalTemplatesDir(bool mode)
{
    QString title_str;
    QString find_dir;

    if (mode){
	m_ui->tmplLabel->setText(QObject::trUtf8("Личные шаблоны"));
	QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly | QFileDialog::ReadOnly;
	find_dir = QFileDialog::getExistingDirectory(this,
							    title_str =QObject::trUtf8("Выберите ранее сохраненные индивидуальные шаблоны"),
							    ".", options);
    }

    if (find_dir.isEmpty() ){
	m_ui->tmplLabel->setText(QObject::trUtf8("Общие шаблоны"));
	// Читаем шаблоны из папки глобальных шаблонов
	find_dir=this->global_templates_dir;
	m_ui->switch_Local_GlolbalBtn->setChecked(false);
    }

    if (!find_dir.isEmpty()){
	m_ui->templatesCbox->clear();
	QStringList filters;
	filters << "*.tmpl" << "*.TMPL";

	QDir dir(find_dir);
	dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	dir.setNameFilters(filters);
	QFileInfoList list = dir.entryInfoList();
	for (int i = 0; i < list.size(); ++i) {
	      QFileInfo fileInfo = list.at(i);
	      m_ui->templatesCbox->addItem(fileInfo.fileName());
	  }
    }
}

void AskTheUser::setStampModel(QStringListModel *stamp_model)
{
    m_ui->secretCBox->setModel(stamp_model);
}

void AskTheUser::setModel (QStandardItemModel * model)
{
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

void AskTheUser::changePAOLabel(bool flag)
{
    QString txt;
    if (!flag){
	txt=QObject::trUtf8("Печать учетной метки на обратной стороне листа");
    }else{
	txt=QObject::trUtf8("Запись карточки документа в базу данных аудита");
    }
    m_ui->previewBtn->setText(txt);
}

AskTheUser::~AskTheUser()
{
    delete m_ui;
}

void AskTheUser::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
	m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
bool AskTheUser::isEnabledPAO() const
{
    return this->m_ui->paperAccountsOutSide->isEnabled();
}
bool AskTheUser::isCheckedPAO() const
{
    return this->m_ui->paperAccountsOutSide->isChecked();
}
void AskTheUser::setEnabledPAO(bool flag)
{
    this->m_ui->paperAccountsOutSide->setEnabled(flag);
}
void AskTheUser::setWorkMode(int mode)
{
    QString title;
    switch (mode){
    case 0:
	title=QObject::trUtf8("Предварительный учет листов");
	this->setEnabledPAO(true);
	this->enableInputFields(true);
	break;
    case 1:
	title=QObject::trUtf8("Печать на предварительно учтеных листах");
	this->setEnabledPAO(false);
	break;
    case 2:
	title=QObject::trUtf8("Учет листов с последующей печатью документа");
	this->setEnabledPAO(false);
	break;
    }
     this->setWindowTitle(title);
}


