#include "asktheuser.h"
#include "ui_asktheuser.h"

askTheUser::askTheUser(QWidget *parent) :
    QDialog(parent),
m_ui(new Ui::askTheUser)
{
    m_ui->setupUi(this);
    MBcompleter = new QCompleter(this);
    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    createConnections();
    m_ui->dateField_dateEd->setDate(QDate::currentDate());

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
void askTheUser::checkMBdublicat()
{
    emit isMBExist(m_ui->mbNumberLineEd->text());
    //Переключим на вторую страницу, покажем пользователя чудесное сообщение
    m_ui->informerTextEd->setText(QObject::trUtf8("Проверяю наличие регистрации в БД аудита документа с номером МБ: \n%1 .").arg(m_ui->mbNumberLineEd->text()));
    this->selectInfoPage();
}

void askTheUser::createConnections()
{
    connect (m_ui->mbNumberLineEd,SIGNAL(editingFinished()),this,SLOT(checkMBdublicat()));
    //TODO проверить не надо ли будет по нажатию на эту кнопку отменять что либо еще
    connect (m_ui->abortInformerButton,SIGNAL(clicked()),m_ui->mbNumberLineEd,SLOT(clear()));
    connect (m_ui->abortInformerButton,SIGNAL(clicked()),this,SLOT(selectWorkPage()));

}
void askTheUser::setModel (QStandardItemModel * model)
{
  mainModel = model;
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
/*
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
*/
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
