/********************************************************************************
** Form generated from reading ui file 'startdlg.ui'
**
** Created: Sat 17. Oct 23:37:39 2009
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
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StartDlg
{
public:
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QPushButton *markPaperBtn;
    QPushButton *printOnMarkPaperBtn;
    QPushButton *printWithAutoMarkPaperBtn;
    QComboBox *printerCBox;
    QListWidget *logList;

    void setupUi(QDialog *StartDlg)
    {
        if (StartDlg->objectName().isEmpty())
            StartDlg->setObjectName(QString::fromUtf8("StartDlg"));
        StartDlg->resize(411, 246);
        gridLayoutWidget = new QWidget(StartDlg);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 10, 391, 151));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setMargin(11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        markPaperBtn = new QPushButton(gridLayoutWidget);
        markPaperBtn->setObjectName(QString::fromUtf8("markPaperBtn"));
        markPaperBtn->setEnabled(false);

        gridLayout->addWidget(markPaperBtn, 3, 0, 1, 1);

        printOnMarkPaperBtn = new QPushButton(gridLayoutWidget);
        printOnMarkPaperBtn->setObjectName(QString::fromUtf8("printOnMarkPaperBtn"));
        printOnMarkPaperBtn->setEnabled(false);

        gridLayout->addWidget(printOnMarkPaperBtn, 3, 1, 1, 1);

        printWithAutoMarkPaperBtn = new QPushButton(gridLayoutWidget);
        printWithAutoMarkPaperBtn->setObjectName(QString::fromUtf8("printWithAutoMarkPaperBtn"));
        printWithAutoMarkPaperBtn->setEnabled(false);

        gridLayout->addWidget(printWithAutoMarkPaperBtn, 5, 0, 1, 2);

        printerCBox = new QComboBox(gridLayoutWidget);
        printerCBox->setObjectName(QString::fromUtf8("printerCBox"));
        printerCBox->setEnabled(false);
        printerCBox->setAutoFillBackground(true);

        gridLayout->addWidget(printerCBox, 2, 0, 1, 2);

        logList = new QListWidget(StartDlg);
        logList->setObjectName(QString::fromUtf8("logList"));
        logList->setEnabled(false);
        logList->setGeometry(QRect(10, 170, 391, 61));
        logList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        retranslateUi(StartDlg);

        QMetaObject::connectSlotsByName(StartDlg);
    } // setupUi

    void retranslateUi(QDialog *StartDlg)
    {
        StartDlg->setWindowTitle(QApplication::translate("StartDlg", "\320\227\320\260\321\211\320\270\321\211\320\265\320\275\320\275\320\260\321\217 \320\277\320\265\321\207\320\260\321\202\321\214", 0, QApplication::UnicodeUTF8));
        markPaperBtn->setText(QApplication::translate("StartDlg", "\320\243\321\207\320\265\321\202 \320\273\320\270\321\201\321\202\320\276\320\262", 0, QApplication::UnicodeUTF8));
        printOnMarkPaperBtn->setText(QApplication::translate("StartDlg", "\320\237\320\265\321\207\320\260\321\202\321\214 \320\275\320\260 \321\203\321\207\321\202\320\265\320\275\321\213\321\205 \320\273\320\270\321\201\321\202\320\260\321\205", 0, QApplication::UnicodeUTF8));
        printWithAutoMarkPaperBtn->setText(QApplication::translate("StartDlg", "\320\237\320\265\321\207\320\260\321\202\321\214 \320\264\320\276\320\272\321\203\320\274\320\265\320\275\321\202\320\260 \321\201 \320\260\320\262\321\202\320\276\320\274\320\260\321\202\320\270\321\207\320\265\321\201\320\272\320\270\320\274 \321\203\321\207\320\265\321\202\320\276\320\274 \320\273\320\270\321\201\321\202\320\276\320\262", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        printerCBox->setToolTip(QApplication::translate("StartDlg", "\320\241\320\277\320\270\321\201\320\276\320\272 \321\200\320\260\320\267\321\200\320\265\321\210\320\265\320\275\320\275\321\213\321\205 \320\277\321\200\320\270\320\275\321\202\320\265\321\200\320\276\320\262...", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        Q_UNUSED(StartDlg);
    } // retranslateUi

};

namespace Ui {
    class StartDlg: public Ui_StartDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STARTDLG_H
