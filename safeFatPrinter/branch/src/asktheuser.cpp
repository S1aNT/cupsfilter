#include "asktheuser.h"
#include "ui_asktheuser.h"

AskTheUser::AskTheUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AskTheUser)
{
    ui->setupUi(this);
}

AskTheUser::~AskTheUser()
{
    delete ui;
}

void AskTheUser::changeEvent(QEvent *e)
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

bool AskTheUser::isEnabledPAO() const
{
    return this->ui->paperAccountsOutSide->isEnabled();
}
bool AskTheUser::isCheckedPAO() const
{
    return this->ui->paperAccountsOutSide->isChecked();
}
void AskTheUser::setEnabledPAO(bool flag)
{
    this->ui->paperAccountsOutSide->setEnabled(flag);
}
void AskTheUser::setWorkMode(int mode)
{
    QString title;
    switch (mode){
    case 0:
	title=QObject::trUtf8("Предварительный учет листов");
	this->setEnabledPAO(true);
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
