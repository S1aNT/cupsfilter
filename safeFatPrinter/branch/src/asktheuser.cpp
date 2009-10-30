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
