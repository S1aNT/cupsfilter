#include "addelemdlg.h"
#include "ui_addelemdlg.h"

addElemDlg::addElemDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addElemDlg)
{
    ui->setupUi(this);
}

addElemDlg::~addElemDlg()
{
    delete ui;
}

void addElemDlg::changeEvent(QEvent *e)
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
