#include "mainwidget.h"

#include <QWidget>
#include <QEvent>

mainWidget::mainWidget(QWidget *parent)
    : QWidget(parent)
{
    if (mainDialog->objectName().isEmpty()){
        mainDialog->setObjectName(QString::fromUtf8("mainDialog"));
    }
    mainDialog->resize(355, 323);
    gridLayout = new QGridLayout(mainDialog);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    groupBox = new QGroupBox(mainDialog);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    formLayout = new QFormLayout(groupBox);
    formLayout->setObjectName(QString::fromUtf8("formLayout"));
    label = new QLabel(groupBox);
    label->setObjectName(QString::fromUtf8("label"));

    formLayout->setWidget(0, QFormLayout::LabelRole, label);

    label_2 = new QLabel(groupBox);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

    label_login = new QLabel(groupBox);
    label_login->setObjectName(QString::fromUtf8("label_login"));

    formLayout->setWidget(0, QFormLayout::FieldRole, label_login);

    label_mandat = new QLabel(groupBox);
    label_mandat->setObjectName(QString::fromUtf8("label_mandat"));

    formLayout->setWidget(1, QFormLayout::FieldRole, label_mandat);


    gridLayout->addWidget(groupBox, 0, 0, 1, 1);

    groupBox_2 = new QGroupBox(mainDialog);
    groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
    comboBox_printerList = new QComboBox(groupBox_2);
    comboBox_printerList->setObjectName(QString::fromUtf8("comboBox_printerList"));
    comboBox_printerList->setGeometry(QRect(0, 30, 331, 27));
    comboBox_printerList->raise();
    groupBox->raise();

    gridLayout->addWidget(groupBox_2, 2, 0, 1, 1);

    pushButton_printFile = new QPushButton(mainDialog);
    pushButton_printFile->setObjectName(QString::fromUtf8("pushButton_printFile"));

    gridLayout->addWidget(pushButton_printFile, 4, 0, 1, 1);

    groupBox_3 = new QGroupBox(mainDialog);
    groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
    verticalLayout = new QVBoxLayout(groupBox_3);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    checkBox_link_surd = new QCheckBox(groupBox_3);
    checkBox_link_surd->setObjectName(QString::fromUtf8("checkBox_link_surd"));
    checkBox_link_surd->setEnabled(false);

    verticalLayout->addWidget(checkBox_link_surd);

    checkBox_load_data = new QCheckBox(groupBox_3);
    checkBox_load_data->setObjectName(QString::fromUtf8("checkBox_load_data"));
    checkBox_load_data->setEnabled(false);

    verticalLayout->addWidget(checkBox_load_data);

    checkBox_select_file = new QCheckBox(groupBox_3);
    checkBox_select_file->setObjectName(QString::fromUtf8("checkBox_select_file"));
    checkBox_select_file->setEnabled(false);

    verticalLayout->addWidget(checkBox_select_file);


    gridLayout->addWidget(groupBox_3, 1, 0, 1, 1);


    retranslateUi(mainDialog);

}

mainWidget::~mainWidget()
{

}

void mainWidget::changeEvent(QEvent *e)
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
