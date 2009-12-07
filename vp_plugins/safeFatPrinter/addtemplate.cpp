#include "addtemplate.h"
#include "ui_addtemplate.h"
#include <QDateTime>

AddTemplate::AddTemplate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTemplate)
{
    ui->setupUi(this);
    ui->date_lineEd->setText(QDateTime::currentDateTime ().toString("dd.MM.yyyy hh:mm:ss"));
}

void AddTemplate::setUserName(const QString & name)
{
    if (!name.isEmpty()){
        ui->author_lineEd->setText(name);
    }
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
