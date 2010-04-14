#include "selectpage.h"

#include <QtGui/QLabel>
#include <QtGui/QCheckBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QComboBox>

SelectPage::SelectPage(QWidget *parent)
    : QWizardPage(parent)
    , sel_printer (QString())
{
    setTitle(QObject::trUtf8("Выбор режима работы ..."));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/select_mode.png"));


    topLabel = new QLabel(QObject::trUtf8("На этом шаге необхоимо выбрать Принтер и один из режимов работы:"
                                          "<ul><li>Пометка зарегистрованного документа как БРАК</li>"
                                          "<li>Учет листов в БД</li><li>Печать на учтенных листах</li>"
                                          "<li>Печать с автоматическим учетом листов</li></ul>"
                                          ));
    topLabel->setWordWrap(true);

    printersCBox = new QComboBox(this);

    markBrakDoc = new QRadioButton(this);
    markBrakDoc->setText(QObject::trUtf8("Пометка зарегистрированного и распечатанного документа как брак"));
    markBrakDoc->setEnabled(false);
    accountingDoc = new QRadioButton(this);
    accountingDoc->setText(QObject::trUtf8("Учет листов в БД"));

    printDoc      = new QRadioButton(this);
    printDoc->setText(QObject::trUtf8("Печать на учтенных листах"));

    both_step      = new QRadioButton(this);
    both_step->setText(QObject::trUtf8("Печать на учтенных листах, с автоматичеcкой регистрацией в БД"));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(topLabel);
    layout->addSpacing(3);
    layout->addWidget(printersCBox);
    layout->addWidget(markBrakDoc);
    layout->addWidget(accountingDoc);
    layout->addWidget(printDoc);
    layout->addWidget(both_step);

    setLayout(layout);
    registerField("printersCBox*",printersCBox);
    registerField("markBrakDoc",  markBrakDoc);
    registerField("accountingDoc", accountingDoc);
    registerField("printDoc",printDoc);
    registerField("both_step",both_step);

    connect (printersCBox,SIGNAL(currentIndexChanged(QString)),
             this, SLOT(setCurrentPrinter(QString)));
}


void SelectPage::setPrinterList(QStringList &pList)
{
    printersCBox->addItems(pList);
    printersCBox->setCurrentIndex(-1);
}
