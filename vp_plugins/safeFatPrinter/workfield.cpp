#include "workfield.h"
#include "ui_workfield.h"

workField::workField(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::workField)
{
    ui->setupUi(this);
    connect (ui->paperAccountsOutSide,SIGNAL(toggled(bool)),this,SLOT(flipLabel(bool)));
    connect (ui->previewBtn,SIGNAL(clicked()),this,SLOT(checkData()));
}

workField::~workField()
{
    delete ui;
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

    //QString msg=QObject::trUtf8("Проверка наличия в БД документа с МБ=[%1] и номером экземпляра =[%2]").arg().arg();
    //ui->AnsLabel->setText(msg);
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
