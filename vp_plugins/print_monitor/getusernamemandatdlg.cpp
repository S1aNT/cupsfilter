#include "getusernamemandatdlg.h"
#include "ui_getusernamemandatdlg.h"

getUserNameMandatDlg::getUserNameMandatDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::getUserNameMandatDlg)
{
    ui->setupUi(this);
    connect (ui->mandatCBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(setCurrentMandat(QString )));
}

getUserNameMandatDlg::~getUserNameMandatDlg()
{
    delete ui;
}

void getUserNameMandatDlg::setUserName(QString &userName)
{
    ui->userNameEd->setText(userName);

}

void getUserNameMandatDlg::setMandatList(const QStringList &mandatList)
{
    ui->mandatCBox->addItems(mandatList);
}

void getUserNameMandatDlg::setCurrentMandat(QString mandat)
{
    u_mandat=mandat;
}

void getUserNameMandatDlg::changeEvent(QEvent *e)
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
