#include "getusernamemandatdlg.h"

#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFormLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>
#include <QSpacerItem>
#include <QVBoxLayout>

getUserNameMandatDlg::getUserNameMandatDlg(QWidget *parent)
    :QDialog(parent)

{
    QFont font;
    font.setFamily(QString::fromUtf8("Times New Roman"));
    font.setPointSize(12);
    font.setBold(true);
    font.setWeight(75);

    this->setWindowModality(Qt::WindowModal);
    this->setMinimumSize(340,140);
    this->setMaximumSize(360,140);
    this->setModal(true);
    this->setWindowTitle(QObject::trUtf8("Выберите мандат...") );

    QWidget *centralWidget          = new QWidget();
    formLayout = new QFormLayout(centralWidget);
    formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

    label = new QLabel();
    label->setText(QObject::trUtf8("Логин"));
    label->setFont(font);

    userNameEd = new QLineEdit();
    userNameEd->setEnabled(false);
    userNameEd->setFont(font);

    label_2 = new QLabel();
    label_2->setText(QObject::trUtf8("Мандат"));
    label_2->setFont(font);

    mandatCBox = new QComboBox();

    formLayout->setWidget(0, QFormLayout::LabelRole, label);
    formLayout->setWidget(0, QFormLayout::FieldRole, userNameEd);
    formLayout->setWidget(1, QFormLayout::LabelRole, label_2);
    formLayout->setWidget(1, QFormLayout::FieldRole, mandatCBox);

    // ------------------ Buttons block ------------------------------

    QWidget *horizontalLayoutWidget = new QWidget();
    QHBoxLayout *horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);

    okButton = new QPushButton(horizontalLayoutWidget);
    okButton->setText(QObject::trUtf8("Выбрать"));
    okButton->setEnabled(false);

    QSpacerItem *horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    cancelButton = new QPushButton(horizontalLayoutWidget);
    cancelButton->setText(QObject::trUtf8("Отмена"));

    //-------------------------------------- Компоновка ----------------------------------------------------
    horizontalLayout->addWidget(okButton);
    horizontalLayout->addItem(horizontalSpacer);
    horizontalLayout->addWidget(cancelButton);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(centralWidget);
    mainLayout->addWidget(horizontalLayoutWidget);

    setLayout(mainLayout);
    // -------------------- Connectors --------------------------------
    connect (okButton, SIGNAL(clicked()),
             this, SLOT(accept())
             );
    connect (cancelButton, SIGNAL(clicked()),
             this, SLOT(reject())
             );
    connect (mandatCBox,SIGNAL(activated(int)),
             this,      SLOT  (enableButtons(int) )
             );
}


void getUserNameMandatDlg::enableButtons(int index)
{
    if (index != -1){
        okButton->setEnabled(true);
    }
}

void getUserNameMandatDlg::setUserName(QString &userName)
{
    userNameEd->setText(userName);
}

void getUserNameMandatDlg::accept()
{
    QString m_login  = userNameEd->text();
    QString m_mandat = mandatCBox->currentText();
    emit setAuthData(m_login,m_mandat);
    QDialog::accept();
}

void getUserNameMandatDlg::setMandatList(const QStringList &mandatList)
{
    mandatCBox->addItems(mandatList);
    mandatCBox->setCurrentIndex(-1);
}

