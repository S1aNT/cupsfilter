#include "previewwnd.h"
#include "ui_previewwnd.h"

PreViewWnd::PreViewWnd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreViewWnd)
{
    ui->setupUi(this);
    ui->scrollArea->setWidget(&img_lab);
    ui->scrollArea->setBackgroundRole(QPalette::Dark);
    connect (ui->CancelBTn,SIGNAL(clicked()),this,SIGNAL(dontNeedPrintPage()));
    connect (ui->printBtn,SIGNAL(clicked()),this,SIGNAL(needPrintPage()));
}

PreViewWnd::~PreViewWnd()
{
    delete ui;
}

void PreViewWnd::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}



// public slots

 void PreViewWnd::showPage(const QPixmap &page)
 {
     img_lab.setPixmap(page);
     this->update();
 }


