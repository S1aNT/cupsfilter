#include "workfield.h"
#include "ui_workfield.h"

workField::workField(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::workField)
{
    ui->setupUi(this);
    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

    connect (ui->paperAccountsOutSide,SIGNAL(toggled(bool)),this,SLOT(flipLabel(bool)));
    connect (ui->previewBtn,SIGNAL(clicked()),this,SLOT(checkData()));
}

workField::~workField()
{
    delete ui;
}

void workField::setPagesCount(int p_count)
{
    ui->pageCountEd->setText(QString::number(p_count,10));
}

void workField::setStampModel(QStringListModel *stamp_model)
{
    ui->secretCBox->setModel(stamp_model);
}

void workField::setModel (QStandardItemModel * model)
{
    //Свяжем  элементы диалогового окна с моделью через mapper
    mapper->setModel(model);
    mapper->toLast();

    for (int i = 0; i < model->columnCount(); i++)
      {
        QStandardItem * header_item = model->horizontalHeaderItem(i);
        QString header = header_item->data(Qt::EditRole).toString();

        if (header.compare(QObject::trUtf8("МБ")) == 0)
          {
            mapper->addMapping(ui->mbNumberLineEd, i);
          }
        else if (header.compare(QObject::trUtf8("Название док-та")) == 0)
          {
            mapper->addMapping(ui->docName_plainTextEdit, i);
          }
        else if (header.compare(QObject::trUtf8("Гриф")) == 0)
          {
            mapper->addMapping(ui->secretCBox, i, "currentIndex");
          }
        else if (header.compare(QObject::trUtf8("Пункт перечня"))==0)
          {
            mapper->addMapping(ui->punktLineEd, i);
          }
        else if (header.compare(QObject::trUtf8("Номер копии"))==0)
          {
            mapper->addMapping(ui->copyNumberLineEd, i);
          }else if (header.compare(QObject::trUtf8("Кол-во листов"))==0)
          {
            mapper->addMapping(ui->pageCountEd, i);
          }
        else if (header.compare(QObject::trUtf8("last_page_stamp"))==0)
          {
            mapper->addMapping(ui->lastPageGroupBoxChecked, i,"checked");
          }
        else if (header.compare(QObject::trUtf8("Исполнитель"))==0)
         {
           mapper->addMapping(ui->executor_lineEd, i);
         }else if (header.compare(QObject::trUtf8("Отпечатал"))==0)
         {
           mapper->addMapping(ui->pressman_lineEd, i);
         } else if (header.compare(QObject::trUtf8("Телефон"))==0)
         {
           mapper->addMapping(ui->telephone_lineEd, i);
         } else if (header.compare(QObject::trUtf8("Инв. N"))==0)
         {
           mapper->addMapping(ui->invNumber_lineEd, i);
         }  else if (header.compare(QObject::trUtf8("Дата распечатки"))==0)
         {
           mapper->addMapping(ui->dateField_dateEd, i);
         }
        else if (header.compare(QObject::trUtf8("recivers_list"))==0)
         {
           mapper->addMapping(ui->reciversListGroupBoxChecked, i,"checked");
         }else if (header.compare(QObject::trUtf8("Получатель N1"))==0)
         {
           mapper->addMapping(ui->reciverOne_lineEd, i);
         }else if (header.compare(QObject::trUtf8("Получатель N2"))==0)
         {
           mapper->addMapping(ui->reciverTwo_lineEd, i);
         }else if (header.compare(QObject::trUtf8("Получатель N3"))==0)
         {
           mapper->addMapping(ui->reciverThree_lineEd, i);
         }else if (header.compare(QObject::trUtf8("Получатель N4"))==0)
         {
           mapper->addMapping(ui->reciverFour_lineEd, i);
         }else if (header.compare(QObject::trUtf8("Получатель N5"))==0)
         {
           mapper->addMapping(ui->reciverFive_lineEd, i);
         }
         /*else if (header.compare(QObject::trUtf8("doc_status"))==0)
         {
             // Мапим скрытое поле статус документа
             mapper->addMapping(&documentStatus, i);
         }
         */
    }

}

//********************************** private slots ******************************************
void workField::flipLabel(bool flip)
{
    QString msg;
    if (flip){
        msg=QObject::trUtf8("Сохранение в БД карточки документа");
    }else{
        msg=QObject::trUtf8("Печать учетной метки на обратной стороне листа");
    }
    ui->previewBtn->setText(msg);
}

void workField::checkData()
{
    // Запретим все элементы
    ui->centralWidget->setEnabled(false);
    // Запишем данные в строку статуса:

    QString msg=QObject::trUtf8("Проверка наличия в БД документа с МБ=[%1] и номером экземпляра =[%2]").arg().arg();
    ui->AnsLabel->setText(msg);
}

//*******************************************************************************************
//                              private function
void workField::isAnyFiedEmpty()
{
    QMessageBox msgBox;
    QPushButton *abortButton;
    QString info_txt=QObject::trUtf8("Данное поле [%1] не может быть пустым");
    QString e_msg;


    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setInformativeText(info_txt);
    abortButton=msgBox.addButton(QObject::trUtf8("Выход"), QMessageBox::RejectRole);
    msgBox.setText(e_msg);
    //QObject::connect(&msgBox,SIGNAL(rejected()),qApp,SLOT(quit()));
    msgBox.exec();

}


//*******************************************************************************************
void workField::changeEvent(QEvent *e)
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
