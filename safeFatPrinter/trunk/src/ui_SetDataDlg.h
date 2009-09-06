/********************************************************************************
** Form generated from reading ui file 'SetDataDlg.ui'
**
** Created: Thu 3. Sep 15:04:55 2009
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SETDATADLG_H
#define UI_SETDATADLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDateEdit>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_SetDataDlg
{
public:
    QGridLayout *gridLayout_5;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *label;
    QComboBox *comboBox;
    QLabel *label_2;
    QSpinBox *spinBox;
    QLabel *label_3;
    QLineEdit *lineEdit;
    QLabel *label_4;
    QComboBox *comboBox_2;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QLabel *label_5;
    QLineEdit *lineEdit_3;
    QLabel *label_6;
    QLineEdit *lineEdit_4;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLineEdit *lineEdit_5;
    QLineEdit *lineEdit_6;
    QDateEdit *dateEdit;
    QCheckBox *checkBox_2;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_3;
    QLabel *label_10;
    QLineEdit *lineEdit_7;
    QLabel *label_11;
    QLineEdit *lineEdit_8;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    QLineEdit *lineEdit_9;
    QLineEdit *lineEdit_10;
    QCheckBox *checkBox;
    QLineEdit *lineEdit_11;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_5;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_4;
    QLabel *label_15;
    QLineEdit *lineEdit_12;
    QLabel *label_16;
    QLineEdit *lineEdit_13;
    QLabel *label_17;
    QLabel *label_18;
    QLabel *label_19;
    QLineEdit *lineEdit_14;
    QLineEdit *lineEdit_15;
    QDateEdit *dateEdit_2;
    QCheckBox *checkBox_3;

    void setupUi(QDialog *SetDataDlg)
    {
        if (SetDataDlg->objectName().isEmpty())
            SetDataDlg->setObjectName(QString::fromUtf8("SetDataDlg"));
        SetDataDlg->resize(689, 406);
        gridLayout_5 = new QGridLayout(SetDataDlg);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        groupBox = new QGroupBox(SetDataDlg);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setFamily(QString::fromUtf8("Times New Roman"));
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        comboBox = new QComboBox(groupBox);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        gridLayout->addWidget(comboBox, 0, 1, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font);

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        spinBox = new QSpinBox(groupBox);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));

        gridLayout->addWidget(spinBox, 1, 1, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font);

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        lineEdit = new QLineEdit(groupBox);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        gridLayout->addWidget(lineEdit, 2, 1, 1, 1);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setFont(font);

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        comboBox_2 = new QComboBox(groupBox);
        comboBox_2->setObjectName(QString::fromUtf8("comboBox_2"));

        gridLayout->addWidget(comboBox_2, 3, 1, 1, 1);


        gridLayout_5->addWidget(groupBox, 0, 0, 1, 3);

        groupBox_2 = new QGroupBox(SetDataDlg);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setFont(font);

        gridLayout_2->addWidget(label_5, 1, 0, 1, 1);

        lineEdit_3 = new QLineEdit(groupBox_2);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));

        gridLayout_2->addWidget(lineEdit_3, 1, 1, 1, 1);

        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setFont(font);

        gridLayout_2->addWidget(label_6, 2, 0, 1, 1);

        lineEdit_4 = new QLineEdit(groupBox_2);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));

        gridLayout_2->addWidget(lineEdit_4, 2, 1, 1, 1);

        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setFont(font);

        gridLayout_2->addWidget(label_7, 3, 0, 1, 1);

        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setFont(font);

        gridLayout_2->addWidget(label_8, 4, 0, 1, 1);

        label_9 = new QLabel(groupBox_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setFont(font);

        gridLayout_2->addWidget(label_9, 6, 0, 1, 1);

        lineEdit_5 = new QLineEdit(groupBox_2);
        lineEdit_5->setObjectName(QString::fromUtf8("lineEdit_5"));

        gridLayout_2->addWidget(lineEdit_5, 4, 1, 1, 1);

        lineEdit_6 = new QLineEdit(groupBox_2);
        lineEdit_6->setObjectName(QString::fromUtf8("lineEdit_6"));

        gridLayout_2->addWidget(lineEdit_6, 3, 1, 1, 1);

        dateEdit = new QDateEdit(groupBox_2);
        dateEdit->setObjectName(QString::fromUtf8("dateEdit"));
        dateEdit->setCalendarPopup(true);

        gridLayout_2->addWidget(dateEdit, 6, 1, 1, 1);

        checkBox_2 = new QCheckBox(groupBox_2);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));

        gridLayout_2->addWidget(checkBox_2, 0, 0, 1, 1);


        gridLayout_5->addWidget(groupBox_2, 0, 3, 1, 3);

        groupBox_3 = new QGroupBox(SetDataDlg);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridLayout_3 = new QGridLayout(groupBox_3);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_10 = new QLabel(groupBox_3);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setFont(font);

        gridLayout_3->addWidget(label_10, 1, 0, 1, 1);

        lineEdit_7 = new QLineEdit(groupBox_3);
        lineEdit_7->setObjectName(QString::fromUtf8("lineEdit_7"));

        gridLayout_3->addWidget(lineEdit_7, 1, 2, 1, 1);

        label_11 = new QLabel(groupBox_3);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setFont(font);

        gridLayout_3->addWidget(label_11, 2, 0, 1, 1);

        lineEdit_8 = new QLineEdit(groupBox_3);
        lineEdit_8->setObjectName(QString::fromUtf8("lineEdit_8"));

        gridLayout_3->addWidget(lineEdit_8, 2, 2, 1, 1);

        label_12 = new QLabel(groupBox_3);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setFont(font);

        gridLayout_3->addWidget(label_12, 3, 0, 1, 1);

        label_13 = new QLabel(groupBox_3);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setFont(font);

        gridLayout_3->addWidget(label_13, 4, 0, 1, 1);

        label_14 = new QLabel(groupBox_3);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setFont(font);

        gridLayout_3->addWidget(label_14, 5, 0, 1, 1);

        lineEdit_9 = new QLineEdit(groupBox_3);
        lineEdit_9->setObjectName(QString::fromUtf8("lineEdit_9"));

        gridLayout_3->addWidget(lineEdit_9, 4, 2, 1, 1);

        lineEdit_10 = new QLineEdit(groupBox_3);
        lineEdit_10->setObjectName(QString::fromUtf8("lineEdit_10"));

        gridLayout_3->addWidget(lineEdit_10, 3, 2, 1, 1);

        checkBox = new QCheckBox(groupBox_3);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));

        gridLayout_3->addWidget(checkBox, 0, 0, 1, 3);

        lineEdit_11 = new QLineEdit(groupBox_3);
        lineEdit_11->setObjectName(QString::fromUtf8("lineEdit_11"));

        gridLayout_3->addWidget(lineEdit_11, 5, 2, 1, 1);


        gridLayout_5->addWidget(groupBox_3, 1, 0, 1, 3);

        pushButton = new QPushButton(SetDataDlg);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout_5->addWidget(pushButton, 2, 2, 1, 2);

        pushButton_2 = new QPushButton(SetDataDlg);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/win/fileprint.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_2->setIcon(icon);

        gridLayout_5->addWidget(pushButton_2, 2, 0, 1, 1);

        pushButton_5 = new QPushButton(SetDataDlg);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));

        gridLayout_5->addWidget(pushButton_5, 2, 1, 1, 1);

        groupBox_4 = new QGroupBox(SetDataDlg);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        gridLayout_4 = new QGridLayout(groupBox_4);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        label_15 = new QLabel(groupBox_4);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setFont(font);

        gridLayout_4->addWidget(label_15, 1, 0, 1, 1);

        lineEdit_12 = new QLineEdit(groupBox_4);
        lineEdit_12->setObjectName(QString::fromUtf8("lineEdit_12"));

        gridLayout_4->addWidget(lineEdit_12, 1, 1, 1, 1);

        label_16 = new QLabel(groupBox_4);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setFont(font);

        gridLayout_4->addWidget(label_16, 2, 0, 1, 1);

        lineEdit_13 = new QLineEdit(groupBox_4);
        lineEdit_13->setObjectName(QString::fromUtf8("lineEdit_13"));

        gridLayout_4->addWidget(lineEdit_13, 2, 1, 1, 1);

        label_17 = new QLabel(groupBox_4);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setFont(font);

        gridLayout_4->addWidget(label_17, 3, 0, 1, 1);

        label_18 = new QLabel(groupBox_4);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setFont(font);

        gridLayout_4->addWidget(label_18, 4, 0, 1, 1);

        label_19 = new QLabel(groupBox_4);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setFont(font);

        gridLayout_4->addWidget(label_19, 6, 0, 1, 1);

        lineEdit_14 = new QLineEdit(groupBox_4);
        lineEdit_14->setObjectName(QString::fromUtf8("lineEdit_14"));

        gridLayout_4->addWidget(lineEdit_14, 4, 1, 1, 1);

        lineEdit_15 = new QLineEdit(groupBox_4);
        lineEdit_15->setObjectName(QString::fromUtf8("lineEdit_15"));

        gridLayout_4->addWidget(lineEdit_15, 3, 1, 1, 1);

        dateEdit_2 = new QDateEdit(groupBox_4);
        dateEdit_2->setObjectName(QString::fromUtf8("dateEdit_2"));
        dateEdit_2->setCalendarPopup(true);

        gridLayout_4->addWidget(dateEdit_2, 6, 1, 1, 1);

        checkBox_3 = new QCheckBox(groupBox_4);
        checkBox_3->setObjectName(QString::fromUtf8("checkBox_3"));

        gridLayout_4->addWidget(checkBox_3, 0, 0, 1, 1);


        gridLayout_5->addWidget(groupBox_4, 1, 3, 1, 3);


        retranslateUi(SetDataDlg);

        QMetaObject::connectSlotsByName(SetDataDlg);
    } // setupUi

    void retranslateUi(QDialog *SetDataDlg)
    {
        SetDataDlg->setWindowTitle(QApplication::translate("SetDataDlg", "Dialog", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("SetDataDlg", "\320\236\320\261\321\211\320\270\320\265 \320\260\321\202\321\200\320\270\320\261\321\203\321\202\321\213", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("SetDataDlg", "\320\223\321\200\320\270\321\204", 0, QApplication::UnicodeUTF8));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("SetDataDlg", "\320\241\320\265\320\272\321\200\320\265\321\202\320\275\320\276", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SetDataDlg", "\320\241\320\276\320\262.\321\201\320\265\320\272\321\200\320\265\321\202\320\275\320\276", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SetDataDlg", "\320\236\321\201\320\276\320\261\320\276\320\271 \320\262\320\260\320\266\320\275\320\276\321\201\321\202\320\270", 0, QApplication::UnicodeUTF8)
        );
        label_2->setText(QApplication::translate("SetDataDlg", "\320\235\320\276\320\274\320\265\321\200 \321\215\320\272\320\267\320\265\320\274\320\277\320\273\321\217\321\200\320\260", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("SetDataDlg", "\320\237\321\203\320\275\320\272\321\202", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("SetDataDlg", "\320\235\320\276\320\274\320\265\321\200 \320\234\320\221", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("SetDataDlg", "\320\250\321\202\320\260\320\274\320\277 \320\277\320\276\321\201\320\273\320\265\320\264\320\275\320\265\320\271 \321\201\321\202\321\200\320\260\320\275\320\270\321\206\321\213", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("SetDataDlg", "\320\230\321\201\320\277\320\276\320\273\320\275\320\270\321\202\320\265\320\273\321\214", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("SetDataDlg", "\320\236\321\202\320\277\320\265\321\207\320\260\321\202\320\260\320\273", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("SetDataDlg", "\320\242\320\265\320\273\320\265\321\204\320\276\320\275", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("SetDataDlg", "\320\230\320\275\320\262. \342\204\226", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("SetDataDlg", "\320\224\320\260\321\202\320\260", 0, QApplication::UnicodeUTF8));
        lineEdit_5->setInputMask(QString());
        lineEdit_5->setText(QString());
        lineEdit_6->setInputMask(QApplication::translate("SetDataDlg", "#DDDddddddd; ", 0, QApplication::UnicodeUTF8));
        checkBox_2->setText(QApplication::translate("SetDataDlg", "\320\237\320\265\321\207\320\260\321\202\320\260\321\202\321\214 \321\210\321\202\320\260\320\274\320\277", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("SetDataDlg", "\320\241\320\277\320\270\321\201\320\276\320\272 \321\200\320\260\321\201\321\201\321\213\320\273\320\272\320\270", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("SetDataDlg", "\320\255\320\272\320\267\320\265\320\274\320\277\320\273\321\217\321\200 \342\204\2261", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("SetDataDlg", "\320\255\320\272\320\267\320\265\320\274\320\277\320\273\321\217\321\200 \342\204\2262", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("SetDataDlg", "\320\255\320\272\320\267\320\265\320\274\320\277\320\273\321\217\321\200 \342\204\2263", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("SetDataDlg", "\320\255\320\272\320\267\320\265\320\274\320\277\320\273\321\217\321\200 \342\204\2264", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("SetDataDlg", "\320\255\320\272\320\267\320\265\320\274\320\277\320\273\321\217\321\200 \342\204\2265", 0, QApplication::UnicodeUTF8));
        lineEdit_9->setInputMask(QString());
        lineEdit_9->setText(QString());
        lineEdit_10->setInputMask(QApplication::translate("SetDataDlg", "#DDDddddddd; ", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("SetDataDlg", "\320\237\320\265\321\207\320\260\321\202\320\260\321\202\321\214 \321\201\320\277\320\270\321\201\320\276\320\272 \321\200\320\260\321\201\321\201\321\213\320\273\320\272\320\270", 0, QApplication::UnicodeUTF8));
        lineEdit_11->setInputMask(QString());
        lineEdit_11->setText(QString());
        pushButton->setText(QApplication::translate("SetDataDlg", "\320\240\320\265\320\264\320\260\320\272\321\202\320\276\321\200 \321\210\320\260\320\261\320\273\320\276\320\275\320\260", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("SetDataDlg", "\320\237\320\265\321\207\320\260\321\202\321\214", 0, QApplication::UnicodeUTF8));
        pushButton_5->setText(QApplication::translate("SetDataDlg", "\320\236\321\202\320\274\320\265\320\275\320\260", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("SetDataDlg", "\320\250\321\202\320\260\320\274\320\277 \320\276\320\261\321\200\320\260\321\202\320\275\320\276\320\271 \321\201\321\202\321\200\320\260\320\275\320\270\321\206\321\213", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("SetDataDlg", "\320\230\321\201\320\277\320\276\320\273\320\275\320\270\321\202\320\265\320\273\321\214", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("SetDataDlg", "\320\236\321\202\320\277\320\265\321\207\320\260\321\202\320\260\320\273", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("SetDataDlg", "\320\242\320\265\320\273\320\265\321\204\320\276\320\275", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("SetDataDlg", "\320\230\320\275\320\262. \342\204\226", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("SetDataDlg", "\320\224\320\260\321\202\320\260", 0, QApplication::UnicodeUTF8));
        lineEdit_14->setInputMask(QString());
        lineEdit_14->setText(QString());
        lineEdit_15->setInputMask(QApplication::translate("SetDataDlg", "#DDDddddddd; ", 0, QApplication::UnicodeUTF8));
        checkBox_3->setText(QApplication::translate("SetDataDlg", "\320\237\320\265\321\207\320\260\321\202\320\260\321\202\321\214 \321\210\321\202\320\260\320\274\320\277", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(SetDataDlg);
    } // retranslateUi

};

namespace Ui {
    class SetDataDlg: public Ui_SetDataDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETDATADLG_H
