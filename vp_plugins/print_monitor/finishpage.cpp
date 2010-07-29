#include "finishpage.h"
#include "mytypes.h"

#include <QtGui/QLabel>


#include <QtGui/QLabel>
#include <QtGui/QGridLayout>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QGroupBox>
#include <QtGui/QLineEdit>

using namespace VPrn;

FinishPage::FinishPage(QWidget *parent)
    :QWidget(parent)
{
    this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::MinimumExpanding);
    this->setMinimumSize(QSize(640, 480));
    this->setMaximumSize(QSize(1024, 768));

    this->setWindowTitle(QObject::trUtf8("Запись результатов печати в БД учета документов"));

    QLabel *topLabel = new QLabel(
            QObject::trUtf8("На этом шаге необходимо установить статус документа <br><center><b>Успешно распечатан</b> или <b>Брак</b></center>"
                            "<br><small><b>Внимание</b> согласно приказу МО №010 бракуется экз. документа целиком</small>")
            );
    topLabel->setWordWrap(true);


    groupBox   = new QGroupBox();
    gridLayout = new QGridLayout(groupBox);

    stampLab     = new QLabel(groupBox);
    stampLab->setText(QObject::trUtf8("Гриф"));

    mbLab        = new QLabel(groupBox);
    mbLab->setText(QObject::trUtf8("МБ"));

    numberLab    = new QLabel(groupBox);
    numberLab->setText(QObject::trUtf8("Номер экз."));

    pageCountLab = new QLabel(groupBox);
    pageCountLab->setText(QObject::trUtf8("Число стр."));

    docNameLab   = new QLabel(groupBox);
    docNameLab->setText(QObject::trUtf8("Документ"));

    printerLab   = new QLabel(groupBox);
    printerLab->setText(QObject::trUtf8("Принтер"));

    executorLab  = new QLabel(groupBox);
    executorLab->setText(QObject::trUtf8("Исполнитель"));

    printManLab  = new QLabel(groupBox);
    printManLab->setText(QObject::trUtf8("Отпечатал"));

    printDateLab = new QLabel(groupBox);
    printDateLab->setText(QObject::trUtf8("Дата печати"));

    pageCountLE = new QLineEdit(groupBox);
    pageCountLE->setReadOnly(true);

    stampLE = new QLineEdit(groupBox);
    stampLE->setReadOnly(true);

    mbLE = new QLineEdit(groupBox);
    mbLE->setReadOnly(true);

    numberLE  = new QLineEdit(groupBox);
    numberLE->setReadOnly(true);

    printerLE = new QLineEdit(groupBox);
    printerLE->setReadOnly(true);

    docNamePE = new QPlainTextEdit(groupBox);
    docNamePE->setReadOnly(true);

    executorLE = new QLineEdit(groupBox);
    executorLE->setReadOnly(true);

    printManLE = new QLineEdit(groupBox);
    printManLE->setReadOnly(true);

    printDateLE = new QLineEdit(groupBox);
    printDateLE->setReadOnly(true);

    gridLayout->addWidget(docNameLab,   0, 0, 1, 1);
    gridLayout->addWidget(docNamePE,    0, 2, 1, 4);
    gridLayout->addWidget(stampLab,     1, 0, 1, 1);
    gridLayout->addWidget(stampLE,      1, 2, 1, 1);
    gridLayout->addWidget(executorLab,  1, 4, 1, 1);
    gridLayout->addWidget(executorLE,   1, 5, 1, 1);
    gridLayout->addWidget(mbLab,        2, 0, 1, 1);
    gridLayout->addWidget(mbLE,         2, 2, 1, 1);
    gridLayout->addWidget(printManLab,  2, 4, 1, 1);
    gridLayout->addWidget(printManLE,   2, 5, 1, 1);
    gridLayout->addWidget(numberLab,    5, 0, 1, 1);
    gridLayout->addWidget(numberLE,     5, 2, 1, 1);
    gridLayout->addWidget(printDateLab, 5, 4, 1, 1);
    gridLayout->addWidget(printDateLE,  5, 5, 1, 1);
    gridLayout->addWidget(pageCountLab, 6, 0, 1, 1);
    gridLayout->addWidget(pageCountLE,  6, 2, 1, 1);
    gridLayout->addWidget(printerLE,    6, 5, 1, 1);
    gridLayout->addWidget(printerLab,   6, 4, 1, 1);




    QWidget::setTabOrder(docNamePE, stampLE);
    QWidget::setTabOrder(stampLE, mbLE);
    QWidget::setTabOrder(mbLE, numberLE);
    QWidget::setTabOrder(numberLE, pageCountLE);
    QWidget::setTabOrder(pageCountLE, executorLE);
    QWidget::setTabOrder(executorLE, printManLE);
    QWidget::setTabOrder(printManLE, printDateLE);
    QWidget::setTabOrder(printDateLE, printerLE);

    QVBoxLayout *verticalLayout = new QVBoxLayout();
    verticalLayout->addWidget(topLabel);
    verticalLayout->addStretch(0);
    verticalLayout->addWidget(groupBox);
    setLayout(verticalLayout);
}
