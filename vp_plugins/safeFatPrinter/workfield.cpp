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

void workField::setMode(WorkMode mode_value)
{
    mode = mode_value;
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
//********************************** public slots ******************************************
void workField::showInfoWindow(const QString &info)
{
    QMessageBox msgBox;
    QPushButton *okButton;

    QString info_txt = QObject::trUtf8("Сервер безопасности уполномочен заявить!");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setInformativeText(info_txt);
    okButton=msgBox.addButton(QObject::trUtf8("Выход"), QMessageBox::AcceptRole);
    msgBox.setText(info);
    msgBox.exec();
}

void workField::showPreviewPage(QPixmap &preview_page)
{
    PreViewWnd *wnd = new PreViewWnd(this);
    connect (wnd,SIGNAL(needPrintPage()),this,SIGNAL(needPrintPage()));
    connect (wnd,SIGNAL(dontNeedPrintPage()),this,SIGNAL(dontNeedPrintPage()));
    wnd->showPage(preview_page);
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
    // Проверим что поле МБ и номер экземпляра не пустые
    QString mb = ui->mbNumberLineEd->text();
    QString copyNum = ui->copyNumberLineEd->text();
    QString msg;
    QString info_txt;
    QString e_msg;
    QMessageBox msgBox;
    QPushButton *abortButton;

    if (!mb.isEmpty()){
        if (!copyNum.isEmpty()){
            // Запретим все элементы
            this->setEnableField(false);
            ui->previewBtn->setEnabled(false);
            // Запишем данные в строку статуса
            msg = QObject::trUtf8("Проверка наличия в БД документа с МБ=[%1] и номером экземпляра =[%2]").arg(mb).arg(copyNum);
            ui->AnsLabel->setText(msg);
            emit needAuthUserToPrinter();
            emit checkMBInBase(mb,copyNum,this->mode);
        }else{
            e_msg = QObject::trUtf8("Номер экземпляра не может быть не заполнен!");
        }
    }else{
        e_msg = QObject::trUtf8("Поле МБ должно быть заполнено");
    }
    if (!e_msg.isEmpty()){
        info_txt = QObject::trUtf8("Данное поле не может быть пустым");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setInformativeText(info_txt);
        abortButton=msgBox.addButton(QObject::trUtf8("Выход"), QMessageBox::RejectRole);
        msgBox.setText(e_msg);
        msgBox.exec();
    }
}

//*******************************************************************************************
//                              private function

void workField::setEnableField(bool e)
{
    ui->titleGroupBox->setEnabled(e);
    ui->commonGroupBox->setEnabled(e);
    ui->lastPageGroupBoxChecked->setEnabled(e);
    ui->reciversListGroupBoxChecked->setEnabled(e);
    ui->paperAccountsOutSide->setEnabled(e);
}

void workField::isAnyFiedEmpty()
{

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
