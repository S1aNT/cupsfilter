/********************************************************************************
** Form generated from reading ui file 'asktheuser.ui'
**
** Created: Fri 23. Oct 14:10:34 2009
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ASKTHEUSER_H
#define UI_ASKTHEUSER_H

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
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QToolButton>

QT_BEGIN_NAMESPACE

class Ui_askTheUser
{
public:
    QGridLayout *gridLayout_5;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_3;
    QLabel *label_10;
    QLineEdit *reciverOne_lineEd;
    QLabel *label_11;
    QLineEdit *reciverTwo_lineEd;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    QLineEdit *reciverFour_lineEd;
    QLineEdit *reciverThree_lineEd;
    QCheckBox *reciversList_chBox;
    QLineEdit *reciverFive_lineEd;
    QPushButton *previewBtn;
    QGroupBox *groupBox_4;
    QPlainTextEdit *docName_plainTextEdit;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *label;
    QComboBox *secretCBox;
    QLabel *label_2;
    QSpinBox *copyNumberSpinBox;
    QLabel *label_3;
    QLineEdit *punktLineEd;
    QLabel *label_4;
    QComboBox *templatesCbox;
    QToolButton *toolButton;
    QLineEdit *mbNumberLineEd;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QLabel *label_5;
    QLineEdit *executor_lineEd;
    QLabel *label_6;
    QLineEdit *pressman_lineEd;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLineEdit *invNumber_lineEd;
    QLineEdit *telephone_lineEd;
    QDateEdit *dateField_dateEd;
    QCheckBox *clisheOnLastPageCheckBox;
    QPushButton *cancelBtn;

    void setupUi(QDialog *askTheUser)
    {
        if (askTheUser->objectName().isEmpty())
            askTheUser->setObjectName(QString::fromUtf8("askTheUser"));
        askTheUser->setWindowModality(Qt::NonModal);
        askTheUser->resize(689, 498);
        askTheUser->setModal(true);
        gridLayout_5 = new QGridLayout(askTheUser);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        groupBox_3 = new QGroupBox(askTheUser);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridLayout_3 = new QGridLayout(groupBox_3);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_10 = new QLabel(groupBox_3);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        QFont font;
        font.setFamily(QString::fromUtf8("Times New Roman"));
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label_10->setFont(font);

        gridLayout_3->addWidget(label_10, 1, 0, 1, 1);

        reciverOne_lineEd = new QLineEdit(groupBox_3);
        reciverOne_lineEd->setObjectName(QString::fromUtf8("reciverOne_lineEd"));
        reciverOne_lineEd->setEnabled(false);

        gridLayout_3->addWidget(reciverOne_lineEd, 1, 2, 1, 1);

        label_11 = new QLabel(groupBox_3);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setFont(font);

        gridLayout_3->addWidget(label_11, 2, 0, 1, 1);

        reciverTwo_lineEd = new QLineEdit(groupBox_3);
        reciverTwo_lineEd->setObjectName(QString::fromUtf8("reciverTwo_lineEd"));
        reciverTwo_lineEd->setEnabled(false);

        gridLayout_3->addWidget(reciverTwo_lineEd, 2, 2, 1, 1);

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

        reciverFour_lineEd = new QLineEdit(groupBox_3);
        reciverFour_lineEd->setObjectName(QString::fromUtf8("reciverFour_lineEd"));
        reciverFour_lineEd->setEnabled(false);

        gridLayout_3->addWidget(reciverFour_lineEd, 4, 2, 1, 1);

        reciverThree_lineEd = new QLineEdit(groupBox_3);
        reciverThree_lineEd->setObjectName(QString::fromUtf8("reciverThree_lineEd"));
        reciverThree_lineEd->setEnabled(false);

        gridLayout_3->addWidget(reciverThree_lineEd, 3, 2, 1, 1);

        reciversList_chBox = new QCheckBox(groupBox_3);
        reciversList_chBox->setObjectName(QString::fromUtf8("reciversList_chBox"));

        gridLayout_3->addWidget(reciversList_chBox, 0, 0, 1, 3);

        reciverFive_lineEd = new QLineEdit(groupBox_3);
        reciverFive_lineEd->setObjectName(QString::fromUtf8("reciverFive_lineEd"));
        reciverFive_lineEd->setEnabled(false);

        gridLayout_3->addWidget(reciverFive_lineEd, 5, 2, 1, 1);


        gridLayout_5->addWidget(groupBox_3, 3, 0, 1, 5);

        previewBtn = new QPushButton(askTheUser);
        previewBtn->setObjectName(QString::fromUtf8("previewBtn"));
        previewBtn->setEnabled(false);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/win/fileprint.png"), QSize(), QIcon::Normal, QIcon::Off);
        previewBtn->setIcon(icon);
        previewBtn->setAutoDefault(false);
        previewBtn->setDefault(true);

        gridLayout_5->addWidget(previewBtn, 5, 0, 1, 1);

        groupBox_4 = new QGroupBox(askTheUser);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        docName_plainTextEdit = new QPlainTextEdit(groupBox_4);
        docName_plainTextEdit->setObjectName(QString::fromUtf8("docName_plainTextEdit"));
        docName_plainTextEdit->setGeometry(QRect(10, 20, 641, 41));
        docName_plainTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        docName_plainTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        gridLayout_5->addWidget(groupBox_4, 1, 0, 1, 4);

        groupBox = new QGroupBox(askTheUser);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setFont(font);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        secretCBox = new QComboBox(groupBox);
        secretCBox->setObjectName(QString::fromUtf8("secretCBox"));

        gridLayout->addWidget(secretCBox, 0, 1, 1, 2);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font);

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        copyNumberSpinBox = new QSpinBox(groupBox);
        copyNumberSpinBox->setObjectName(QString::fromUtf8("copyNumberSpinBox"));

        gridLayout->addWidget(copyNumberSpinBox, 1, 1, 1, 2);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font);

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        punktLineEd = new QLineEdit(groupBox);
        punktLineEd->setObjectName(QString::fromUtf8("punktLineEd"));

        gridLayout->addWidget(punktLineEd, 2, 1, 1, 2);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setFont(font);

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        templatesCbox = new QComboBox(groupBox);
        templatesCbox->setObjectName(QString::fromUtf8("templatesCbox"));

        gridLayout->addWidget(templatesCbox, 4, 0, 1, 2);

        toolButton = new QToolButton(groupBox);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));
        toolButton->setEnabled(false);

        gridLayout->addWidget(toolButton, 4, 2, 1, 1);

        mbNumberLineEd = new QLineEdit(groupBox);
        mbNumberLineEd->setObjectName(QString::fromUtf8("mbNumberLineEd"));

        gridLayout->addWidget(mbNumberLineEd, 3, 1, 1, 2);


        gridLayout_5->addWidget(groupBox, 2, 0, 1, 2);

        groupBox_2 = new QGroupBox(askTheUser);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setFont(font);

        gridLayout_2->addWidget(label_5, 1, 0, 1, 1);

        executor_lineEd = new QLineEdit(groupBox_2);
        executor_lineEd->setObjectName(QString::fromUtf8("executor_lineEd"));
        executor_lineEd->setEnabled(false);

        gridLayout_2->addWidget(executor_lineEd, 1, 1, 1, 1);

        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setFont(font);

        gridLayout_2->addWidget(label_6, 2, 0, 1, 1);

        pressman_lineEd = new QLineEdit(groupBox_2);
        pressman_lineEd->setObjectName(QString::fromUtf8("pressman_lineEd"));
        pressman_lineEd->setEnabled(false);

        gridLayout_2->addWidget(pressman_lineEd, 2, 1, 1, 1);

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

        invNumber_lineEd = new QLineEdit(groupBox_2);
        invNumber_lineEd->setObjectName(QString::fromUtf8("invNumber_lineEd"));
        invNumber_lineEd->setEnabled(false);

        gridLayout_2->addWidget(invNumber_lineEd, 4, 1, 1, 1);

        telephone_lineEd = new QLineEdit(groupBox_2);
        telephone_lineEd->setObjectName(QString::fromUtf8("telephone_lineEd"));
        telephone_lineEd->setEnabled(false);

        gridLayout_2->addWidget(telephone_lineEd, 3, 1, 1, 1);

        dateField_dateEd = new QDateEdit(groupBox_2);
        dateField_dateEd->setObjectName(QString::fromUtf8("dateField_dateEd"));
        dateField_dateEd->setEnabled(false);
        dateField_dateEd->setDateTime(QDateTime(QDate(1752, 9, 14), QTime(0, 0, 0)));
        dateField_dateEd->setCalendarPopup(true);

        gridLayout_2->addWidget(dateField_dateEd, 6, 1, 1, 1);

        clisheOnLastPageCheckBox = new QCheckBox(groupBox_2);
        clisheOnLastPageCheckBox->setObjectName(QString::fromUtf8("clisheOnLastPageCheckBox"));

        gridLayout_2->addWidget(clisheOnLastPageCheckBox, 0, 0, 1, 1);


        gridLayout_5->addWidget(groupBox_2, 2, 3, 1, 1);

        cancelBtn = new QPushButton(askTheUser);
        cancelBtn->setObjectName(QString::fromUtf8("cancelBtn"));
        cancelBtn->setAutoDefault(false);

        gridLayout_5->addWidget(cancelBtn, 5, 1, 1, 1);


        retranslateUi(askTheUser);
        QObject::connect(previewBtn, SIGNAL(clicked()), askTheUser, SLOT(accept()));
        QObject::connect(cancelBtn, SIGNAL(clicked()), askTheUser, SLOT(reject()));
        QObject::connect(clisheOnLastPageCheckBox, SIGNAL(toggled(bool)), executor_lineEd, SLOT(setEnabled(bool)));
        QObject::connect(clisheOnLastPageCheckBox, SIGNAL(toggled(bool)), pressman_lineEd, SLOT(setEnabled(bool)));
        QObject::connect(clisheOnLastPageCheckBox, SIGNAL(toggled(bool)), telephone_lineEd, SLOT(setEnabled(bool)));
        QObject::connect(clisheOnLastPageCheckBox, SIGNAL(toggled(bool)), invNumber_lineEd, SLOT(setEnabled(bool)));
        QObject::connect(clisheOnLastPageCheckBox, SIGNAL(toggled(bool)), dateField_dateEd, SLOT(setEnabled(bool)));
        QObject::connect(reciversList_chBox, SIGNAL(toggled(bool)), reciverOne_lineEd, SLOT(setEnabled(bool)));
        QObject::connect(reciversList_chBox, SIGNAL(toggled(bool)), reciverTwo_lineEd, SLOT(setEnabled(bool)));
        QObject::connect(reciversList_chBox, SIGNAL(toggled(bool)), reciverThree_lineEd, SLOT(setEnabled(bool)));
        QObject::connect(reciversList_chBox, SIGNAL(toggled(bool)), reciverFour_lineEd, SLOT(setEnabled(bool)));
        QObject::connect(reciversList_chBox, SIGNAL(toggled(bool)), reciverFive_lineEd, SLOT(setEnabled(bool)));

        secretCBox->setCurrentIndex(-1);
        templatesCbox->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(askTheUser);
    } // setupUi

    void retranslateUi(QDialog *askTheUser)
    {
        askTheUser->setWindowTitle(QApplication::translate("askTheUser", "\320\227\320\260\320\277\320\276\320\273\320\275\320\265\320\275\320\270\320\265 \321\203\321\207\320\265\321\202\320\275\320\276\320\271 \320\272\320\260\321\200\321\202\320\276\321\207\320\272\320\270 \320\264\320\276\320\272\321\203\320\274\320\265\320\275\321\202\320\260", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("askTheUser", "\320\241\320\277\320\270\321\201\320\276\320\272 \321\200\320\260\321\201\321\201\321\213\320\273\320\272\320\270", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("askTheUser", "\320\255\320\272\320\267\320\265\320\274\320\277\320\273\321\217\321\200 \342\204\2261", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("askTheUser", "\320\255\320\272\320\267\320\265\320\274\320\277\320\273\321\217\321\200 \342\204\2262", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("askTheUser", "\320\255\320\272\320\267\320\265\320\274\320\277\320\273\321\217\321\200 \342\204\2263", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("askTheUser", "\320\255\320\272\320\267\320\265\320\274\320\277\320\273\321\217\321\200 \342\204\2264", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("askTheUser", "\320\255\320\272\320\267\320\265\320\274\320\277\320\273\321\217\321\200 \342\204\2265", 0, QApplication::UnicodeUTF8));
        reciverFour_lineEd->setInputMask(QString());
        reciverFour_lineEd->setText(QString());
        reciverThree_lineEd->setInputMask(QString());
        reciversList_chBox->setText(QApplication::translate("askTheUser", "\320\237\320\265\321\207\320\260\321\202\320\260\321\202\321\214 \321\201\320\277\320\270\321\201\320\276\320\272 \321\200\320\260\321\201\321\201\321\213\320\273\320\272\320\270", 0, QApplication::UnicodeUTF8));
        reciverFive_lineEd->setInputMask(QString());
        reciverFive_lineEd->setText(QString());
        previewBtn->setText(QApplication::translate("askTheUser", "\320\237\320\265\321\207\320\260\321\202\321\214 \320\264\320\276\320\272\321\203\320\274\320\265\320\275\321\202\320\260 \321\201 \320\260\320\262\321\202\320\276\320\274\320\260\321\202\320\270\321\207\320\265\321\201\320\272\320\270\320\274 \321\203\321\207\320\265\321\202\320\276\320\274 \320\273\320\270\321\201\321\202\320\276\320\262", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("askTheUser", "\320\235\320\260\320\267\320\262\320\260\320\275\320\270\320\265 \320\264\320\276\320\272\321\203\320\274\320\265\320\275\321\202\320\260...", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("askTheUser", "\320\236\320\261\321\211\320\270\320\265 \320\260\321\202\321\200\320\270\320\261\321\203\321\202\321\213", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("askTheUser", "\320\223\321\200\320\270\321\204", 0, QApplication::UnicodeUTF8));
        secretCBox->clear();
        secretCBox->insertItems(0, QStringList()
         << QApplication::translate("askTheUser", "\320\241\320\265\320\272\321\200\320\265\321\202\320\275\320\276", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("askTheUser", "\320\241\320\276\320\262.\321\201\320\265\320\272\321\200\320\265\321\202\320\275\320\276", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("askTheUser", "\320\236\321\201\320\276\320\261\320\276\320\271 \320\262\320\260\320\266\320\275\320\276\321\201\321\202\320\270", 0, QApplication::UnicodeUTF8)
        );
        label_2->setText(QApplication::translate("askTheUser", "\320\235\320\276\320\274\320\265\321\200 \321\215\320\272\320\267\320\265\320\274\320\277\320\273\321\217\321\200\320\260", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("askTheUser", "\320\237\321\203\320\275\320\272\321\202", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("askTheUser", "\320\235\320\276\320\274\320\265\321\200 \320\234\320\221", 0, QApplication::UnicodeUTF8));
        templatesCbox->clear();
        templatesCbox->insertItems(0, QStringList()
         << QApplication::translate("askTheUser", "\320\241\320\265\320\272\321\200\320\265\321\202\320\275\321\213\320\271 \320\277\321\200\320\270\320\272\320\260\320\267", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        templatesCbox->setToolTip(QApplication::translate("askTheUser", "\320\222\321\213\320\261\320\265\321\200\320\270\321\202\320\265 \321\210\320\260\320\261\320\273\320\276\320\275,\320\272\320\276\321\202\320\276\321\200\321\213\320\271 \320\261\321\203\320\264\320\265\321\202 \320\277\321\200\320\270\320\274\320\265\320\275 \320\272 \321\202\320\265\320\272\321\203\321\211\320\265\320\274\321\203 \320\264\320\276\320\272\321\203\320\274\320\265\320\275\321\202\321\203", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        toolButton->setToolTip(QApplication::translate("askTheUser", "\320\243\320\277\321\200\320\260\320\262\320\273\320\265\320\275\320\270\320\265 \321\210\320\260\320\261\320\273\320\276\320\275\320\260\320\274\320\270 (\320\264\320\276\320\261\320\260\320\262\320\270\321\202\321\214,\321\203\320\264\320\260\320\273\320\270\321\202\321\214,\321\200\320\265\320\264\320\260\320\272\321\202\320\270\321\200\320\276\320\262\320\260\321\202\321\214)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton->setText(QApplication::translate("askTheUser", "...", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("askTheUser", "\320\250\321\202\320\260\320\274\320\277 \320\277\320\276\321\201\320\273\320\265\320\264\320\275\320\265\320\271 \321\201\321\202\321\200\320\260\320\275\320\270\321\206\321\213", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("askTheUser", "\320\230\321\201\320\277\320\276\320\273\320\275\320\270\321\202\320\265\320\273\321\214", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("askTheUser", "\320\236\321\202\320\277\320\265\321\207\320\260\321\202\320\260\320\273", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("askTheUser", "\320\242\320\265\320\273\320\265\321\204\320\276\320\275", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("askTheUser", "\320\230\320\275\320\262. \342\204\226", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("askTheUser", "\320\224\320\260\321\202\320\260", 0, QApplication::UnicodeUTF8));
        invNumber_lineEd->setInputMask(QString());
        invNumber_lineEd->setText(QString());
        telephone_lineEd->setInputMask(QApplication::translate("askTheUser", "#DDDddddddd; ", 0, QApplication::UnicodeUTF8));
        clisheOnLastPageCheckBox->setText(QApplication::translate("askTheUser", "\320\237\320\265\321\207\320\260\321\202\320\260\321\202\321\214 \321\210\321\202\320\260\320\274\320\277", 0, QApplication::UnicodeUTF8));
        cancelBtn->setText(QApplication::translate("askTheUser", "\320\236\321\202\320\274\320\265\320\275\320\260", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(askTheUser);
    } // retranslateUi

};

namespace Ui {
    class askTheUser: public Ui_askTheUser {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ASKTHEUSER_H
