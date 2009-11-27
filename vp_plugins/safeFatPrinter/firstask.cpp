#include "firstask.h"
#include "ui_firstask.h"

firstAsk::firstAsk(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::firstAsk)
{
    ui->setupUi(this);
    signalMapper = new QSignalMapper(this);
    connect(ui->printModeAccounting, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->printFromAccountPaper, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->printWithAccountingPaper, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(ui->printModeAccounting,0 );
    signalMapper->setMapping(ui->printFromAccountPaper,1);
    signalMapper->setMapping(ui->printWithAccountingPaper,2 );
    connect(signalMapper, SIGNAL(mapped(int)),  this, SLOT(setMode(int)));
}

firstAsk::~firstAsk()
{
    delete ui;
}

void firstAsk::setMode (int signal_mode)
{
    work_mode = signal_mode;
}

void firstAsk::changeEvent(QEvent *e)
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
