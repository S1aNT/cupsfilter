#include "teditor.h"
#include "ui_teditor.h"



TEditor::TEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TEditor)
{
    ui->setupUi(this);
    view_1 = new View("test");
    ui->tabWidget->addTab(view_1,QObject::trUtf8("1-я страница"));
    view_2 = new View("test");
    ui->tabWidget->addTab(view_2,QObject::trUtf8("2-я страница"));
    view_3 = new View("test");
    ui->tabWidget->addTab(view_3,QObject::trUtf8("3-я страница"));
    view_4 = new View("test");
    ui->tabWidget->addTab(view_4,QObject::trUtf8("4-я страница"));
}

void TEditor::setScene(QGraphicsScene *scene_1,QGraphicsScene *scene_2,
                       QGraphicsScene *scene_3,QGraphicsScene *scene_4)
{
    view_1->gr_view()->setScene(scene_1);
    view_2->gr_view()->setScene(scene_2);
    view_3->gr_view()->setScene(scene_3);
    view_4->gr_view()->setScene(scene_4);
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
