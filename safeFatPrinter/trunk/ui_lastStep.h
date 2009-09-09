/********************************************************************************
** Form generated from reading ui file 'lastStep.ui'
**
** Created: Thu 10. Sep 00:48:10 2009
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_LASTSTEP_H
#define UI_LASTSTEP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *label;
    QTextEdit *textEdit;
    QLabel *label_2;
    QLineEdit *lineEdit;
    QLabel *label_3;
    QLineEdit *lineEdit_2;
    QLabel *label_4;
    QLineEdit *lineEdit_3;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(400, 313);
        gridLayoutWidget = new QWidget(Form);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 10, 381, 231));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(gridLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 1, 1, 1, 1);

        textEdit = new QTextEdit(gridLayoutWidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));

        gridLayout->addWidget(textEdit, 1, 2, 1, 1);

        label_2 = new QLabel(gridLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 2, 1, 1, 1);

        lineEdit = new QLineEdit(gridLayoutWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        gridLayout->addWidget(lineEdit, 2, 2, 1, 1);

        label_3 = new QLabel(gridLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setLayoutDirection(Qt::LeftToRight);

        gridLayout->addWidget(label_3, 3, 1, 1, 1);

        lineEdit_2 = new QLineEdit(gridLayoutWidget);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

        gridLayout->addWidget(lineEdit_2, 3, 2, 1, 1);

        label_4 = new QLabel(gridLayoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 4, 1, 1, 1);

        lineEdit_3 = new QLineEdit(gridLayoutWidget);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));

        gridLayout->addWidget(lineEdit_3, 4, 2, 1, 1);

        horizontalLayoutWidget = new QWidget(Form);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 250, 381, 51));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton = new QPushButton(horizontalLayoutWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(horizontalLayoutWidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        horizontalLayout->addWidget(pushButton_2);


        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Form", "    \320\224\320\276\320\272\321\203\320\274\320\265\320\275\321\202", 0, QApplication::UnicodeUTF8));
        textEdit->setHtml(QApplication::translate("Form", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><a href=\"#\320\2621\"><span style=\" font-size:8pt; text-decoration: underline; color:#0000ff;\">\320\236\320\277\321\200\320\276\320\275\321\213\320\271 \320\272\320\276\320\275\321\201\320\277\320\265\320\272\321\202 \320\277\320\276 \321\202\320\265\320\274\320\265 \320\235\320\260\320\267\320\275\320\260\321\207\320\265\320\275\320\270\320\265 \320\270 \320\261\320\276\320\265\320\262\321\213\320\265 \321\201\320\262\320\276\320\271\321\201\321\202\320\262\320\260 \321\200\321\203\321\207\320\275\321\213\321\205 \320\275\320\260\321\201\321\202\321"
                        "\203\320\277\320\260\321\202\320\265\320\273\321\214\320\275\321\213\321\205, \320\276\320\261\320\276\321\200\320\276\320\275\320\270\321\202\320\265\320\273\321\214\320\275\321\213\321\205 \320\270 \320\277\321\200\320\276\321\202\320\270\320\262\320\276\321\202\320\260\320\275\320\272\320\276\320\262\321\213\321\205 \320\263\321\200\320\260\320\275\320\260\321\202. \320\243\321\201\321\202\321\200\320\276\320\271\321\201\321\202\320\262\320\276 \320\263\321\200\320\260\320\275\320\260\321\202. \320\240\320\260\320\261\320\276\321\202\320\260 \321\207\320\260\321\201\321\202\320\265\320\271 \320\270 \320\274\320\265\321\205\320\260\320\275\320\270\320\267\320\274\320\276\320\262 \320\263\321\200\320\260\320\275\320\260\321\202.</span></a><span style=\" font-size:8pt;\"> </span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Form", "  \320\236\321\202\320\277\320\265\321\207\320\260\321\202\320\260\320\275 \320\275\320\260 ", 0, QApplication::UnicodeUTF8));
        lineEdit->setText(QApplication::translate("Form", "HpLaserJet1500", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Form", "        \320\223\321\200\320\270\321\204", 0, QApplication::UnicodeUTF8));
        lineEdit_2->setText(QApplication::translate("Form", "\320\245\320\245\320\245 \321\205\321\205\321\205\321\205\321\205\321\205", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Form", "\320\247\320\270\321\201\320\273\320\276 \321\201\321\202\321\200\320\260\320\275\320\270\321\206:", 0, QApplication::UnicodeUTF8));
        lineEdit_3->setText(QApplication::translate("Form", "999", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("Form", "\320\237\320\276\320\264\321\202\320\262\320\265\321\200\320\266\320\264\320\260\321\216", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("Form", "\320\240\320\260\320\277\320\276\321\200\321\202 \320\276\320\261 \320\276\321\210\320\270\320\261\320\272\320\265...", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Form);
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LASTSTEP_H
