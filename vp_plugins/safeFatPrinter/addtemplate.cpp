#include "addtemplate.h"
#include "ui_addtemplate.h"

AddTemplate::AddTemplate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTemplate)
{
    ui->setupUi(this);
}

AddTemplate::~AddTemplate()
{
    delete ui;
}

void AddTemplate::changeEvent(QEvent *e)
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
