#include "teditor.h"
#include "ui_teditor.h"



TEditor::TEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TEditor)
{
    ui->setupUi(this);
    View *view1 = new View("test");
    ui->tabWidget->addTab(view1,QObject::trUtf8("1-я страница"));
    View *view2 = new View("test");
    ui->tabWidget->addTab(view2,QObject::trUtf8("2-я страница"));
    View *view3 = new View("test");
    ui->tabWidget->addTab(view3,QObject::trUtf8("3-я страница"));
    View *view4 = new View("test");
    ui->tabWidget->addTab(view4,QObject::trUtf8("4-я страница"));
}

TEditor::~TEditor()
{
    delete ui;
}

void TEditor::changeEvent(QEvent *e)
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
