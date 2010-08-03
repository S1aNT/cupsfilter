#include "selectpage.h"

#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QComboBox>
#include <QtGui/QRadioButton>
#include <QtGui/QStandardItem>
#include <QtGui/QStandardItemModel>
#include <QtCore/QModelIndex>

#include "mytypes.h"
using namespace VPrn;


SelectPage::SelectPage(QWidget *parent)
    : QWidget(parent)
    , printer_model(0)
{
    this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::MinimumExpanding);

    this->setWindowTitle(QObject::trUtf8("Выбор режима работы ..."));

    QLabel *topLabel = new QLabel(QObject::trUtf8("На этом шаге необходимо выбрать <b>Принтер</b> и один из режимов работы:"
                                                  "<ul><li>Пометка зарегистрованного документа как БРАК</li><br/>"
                                                  "<li>Учет листов в БД</li><br/><li>Печать на учтенных листах</li><br/>"
                                                  "<li>Печать с автоматическим учетом листов</li></ul>"
                                                  ));
    topLabel->setWordWrap(true);

    printersCBox  = new QComboBox(this);
    markBrakDoc   = new QRadioButton(this);
    accountingDoc = new QRadioButton(this);
    printDoc      = new QRadioButton(this);
    both_step     = new QRadioButton(this);

    markBrakDoc->setText(QObject::trUtf8("Пометка зарегистрированного и распечатанного документа как брак"));
    markBrakDoc->setEnabled(false);
    accountingDoc->setText(QObject::trUtf8("Учет листов в БД"));
    printDoc->setText(QObject::trUtf8("Печать на учтенных листах"));
    both_step->setText(QObject::trUtf8("Печать на учтенных листах, с автоматичеcкой регистрацией в БД"));

    QVBoxLayout *verticalLayout = new QVBoxLayout();
    verticalLayout->addStretch(0);
    verticalLayout->addWidget(topLabel);
    verticalLayout->addSpacing(3);
    verticalLayout->addWidget(printersCBox);
    verticalLayout->addWidget(markBrakDoc);
    verticalLayout->addWidget(accountingDoc);
    verticalLayout->addWidget(printDoc);
    verticalLayout->addWidget(both_step);
    verticalLayout->addStretch(0);
    setLayout(verticalLayout);

    connect (printersCBox,SIGNAL(currentIndexChanged(int)),
             this,        SLOT(selectedPrinter(int))
             );
}


void SelectPage::setPrintersModel(QStandardItemModel *p_model )
{
    if (p_model){
        printer_model = p_model;
        printersCBox->setModel( printer_model );
        printersCBox->setModelColumn( 0 );
        printersCBox->setCurrentIndex(-1);
    }
}

void SelectPage::selectedPrinter(int idx)
{
    for (int i=0; i< printer_model->rowCount();i++){
        QModelIndex index = printer_model->index(i,3);
        bool sel_flag = false;
        if (i == idx){
            sel_flag = true;
        }
        printer_model->setData(index,sel_flag,Qt::EditRole);
    }
}

bool SelectPage::enableNext()
{
    bool Ok = true;
    {
        if (printersCBox->currentIndex() == -1){
            Ok &= false;
        }
        Ok &= ( markBrakDoc->isChecked()   ||
                accountingDoc->isChecked() ||
                printDoc->isChecked()      ||
                both_step->isChecked());

    }
    return Ok;
}
