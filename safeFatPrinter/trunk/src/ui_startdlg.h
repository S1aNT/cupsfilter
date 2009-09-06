/********************************************************************************
** Form generated from reading ui file 'startdlg.ui'
**
** Created: Tue 1. Sep 17:13:07 2009
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_STARTDLG_H
#define UI_STARTDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StartDlg
{
public:
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QComboBox *comboBox;

    void setupUi(QDialog *StartDlg)
    {
        if (StartDlg->objectName().isEmpty())
            StartDlg->setObjectName(QString::fromUtf8("StartDlg"));
        StartDlg->resize(359, 113);
        gridLayoutWidget = new QWidget(StartDlg);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 10, 341, 91));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setMargin(11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        pushButton = new QPushButton(gridLayoutWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout->addWidget(pushButton, 1, 0, 1, 1);

        pushButton_2 = new QPushButton(gridLayoutWidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        gridLayout->addWidget(pushButton_2, 1, 1, 1, 1);

        comboBox = new QComboBox(gridLayoutWidget);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        gridLayout->addWidget(comboBox, 2, 0, 1, 2);


        retranslateUi(StartDlg);

        QMetaObject::connectSlotsByName(StartDlg);
    } // setupUi

    void retranslateUi(QDialog *StartDlg)
    {
        StartDlg->setWindowTitle(QApplication::translate("StartDlg", "\320\227\320\260\321\211\320\270\321\211\320\265\320\275\320\275\320\260\321\217 \320\277\320\265\321\207\320\260\321\202\321\214", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("StartDlg", "\320\243\321\207\320\265\321\202 \320\273\320\270\321\201\321\202\320\276\320\262", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("StartDlg", "\320\237\320\265\321\207\320\260\321\202\321\214 \320\275\320\260 \321\203\321\207\321\202\320\265\320\275\321\213\321\205 \320\273\320\270\321\201\321\202\320\260\321\205", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(StartDlg);
    } // retranslateUi

};

namespace Ui {
    class StartDlg: public Ui_StartDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STARTDLG_H
