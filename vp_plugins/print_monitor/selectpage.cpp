#include "selectpage.h"

#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QComboBox>
#include <QtGui/QRadioButton>

#include "mytypes.h"
using namespace VPrn;


SelectPage::SelectPage(QWidget *parent)
    : QWidget(parent)
    , printer_id (-1)
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
             this, SLOT(setCurrentPrinter(int)));
}


void SelectPage::setPrintersModel(QStandardItemModel *p_model )
{
    printersCBox->setModel( p_model );
    printersCBox->setModelColumn( 0 );
}
