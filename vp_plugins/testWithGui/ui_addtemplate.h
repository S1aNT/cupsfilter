/********************************************************************************
** Form generated from reading ui file 'addtemplate.ui'
**
** Created: Wed 10. Feb 11:36:30 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ADDTEMPLATE_H
#define UI_ADDTEMPLATE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AddTemplate
{
public:
    QGroupBox *t_descGBox;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPlainTextEdit *descTextEdit;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *addBtn;
    QPushButton *cancelBtn;
    QGroupBox *t_nameGBox;
    QLineEdit *t_name_lineEd;
    QGroupBox *groupBox_4;
    QWidget *horizontalLayoutWidget_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QLineEdit *author_lineEd;
    QLabel *label_3;
    QLineEdit *date_lineEd;
    QGroupBox *page_orientGBox;
    QWidget *formLayoutWidget;
    QGridLayout *gridLayout_2;
    QToolButton *portretBtn;
    QToolButton *landscapeBtn;
    QLabel *p_orient_label;
    QLabel *l_orient_label;
    QGroupBox *marginGBox;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QSpinBox *margin_topSpBox;
    QSpinBox *margin_bottomSBox;
    QSpinBox *margin_leftSpBox;
    QSpinBox *margin_rightSpBox;
    QGroupBox *page_sizeGBox;
    QWidget *gridLayoutWidget_3;
    QGridLayout *gridLayout_4;
    QLabel *label_10;
    QComboBox *pageSizeCBox;
    QLabel *label_9;
    QLabel *label_11;
    QLineEdit *p_widthLEd;
    QLineEdit *p_heightLEd;
    QButtonGroup *orient_BtnGroup;

    void setupUi(QDialog *AddTemplate)
    {
        if (AddTemplate->objectName().isEmpty())
            AddTemplate->setObjectName(QString::fromUtf8("AddTemplate"));
        AddTemplate->setWindowModality(Qt::WindowModal);
        AddTemplate->resize(460, 455);
        AddTemplate->setModal(true);
        t_descGBox = new QGroupBox(AddTemplate);
        t_descGBox->setObjectName(QString::fromUtf8("t_descGBox"));
        t_descGBox->setGeometry(QRect(10, 57, 441, 91));
        horizontalLayoutWidget = new QWidget(t_descGBox);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 20, 421, 61));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        descTextEdit = new QPlainTextEdit(horizontalLayoutWidget);
        descTextEdit->setObjectName(QString::fromUtf8("descTextEdit"));
        descTextEdit->setCenterOnScroll(true);

        horizontalLayout->addWidget(descTextEdit);

        horizontalLayoutWidget_2 = new QWidget(AddTemplate);
        horizontalLayoutWidget_2->setObjectName(QString::fromUtf8("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(200, 404, 251, 41));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        addBtn = new QPushButton(horizontalLayoutWidget_2);
        addBtn->setObjectName(QString::fromUtf8("addBtn"));

        horizontalLayout_2->addWidget(addBtn);

        cancelBtn = new QPushButton(horizontalLayoutWidget_2);
        cancelBtn->setObjectName(QString::fromUtf8("cancelBtn"));

        horizontalLayout_2->addWidget(cancelBtn);

        t_nameGBox = new QGroupBox(AddTemplate);
        t_nameGBox->setObjectName(QString::fromUtf8("t_nameGBox"));
        t_nameGBox->setGeometry(QRect(10, 4, 440, 51));
        t_name_lineEd = new QLineEdit(t_nameGBox);
        t_name_lineEd->setObjectName(QString::fromUtf8("t_name_lineEd"));
        t_name_lineEd->setGeometry(QRect(10, 20, 420, 20));
        groupBox_4 = new QGroupBox(AddTemplate);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(10, 337, 441, 61));
        horizontalLayoutWidget_3 = new QWidget(groupBox_4);
        horizontalLayoutWidget_3->setObjectName(QString::fromUtf8("horizontalLayoutWidget_3"));
        horizontalLayoutWidget_3->setGeometry(QRect(10, 20, 421, 31));
        horizontalLayout_3 = new QHBoxLayout(horizontalLayoutWidget_3);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(horizontalLayoutWidget_3);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_3->addWidget(label);

        author_lineEd = new QLineEdit(horizontalLayoutWidget_3);
        author_lineEd->setObjectName(QString::fromUtf8("author_lineEd"));
        author_lineEd->setEnabled(false);
        author_lineEd->setReadOnly(true);

        horizontalLayout_3->addWidget(author_lineEd);

        label_3 = new QLabel(horizontalLayoutWidget_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_3->addWidget(label_3);

        date_lineEd = new QLineEdit(horizontalLayoutWidget_3);
        date_lineEd->setObjectName(QString::fromUtf8("date_lineEd"));
        date_lineEd->setEnabled(false);
        date_lineEd->setReadOnly(true);

        horizontalLayout_3->addWidget(date_lineEd);

        page_orientGBox = new QGroupBox(AddTemplate);
        page_orientGBox->setObjectName(QString::fromUtf8("page_orientGBox"));
        page_orientGBox->setGeometry(QRect(10, 151, 141, 91));
        formLayoutWidget = new QWidget(page_orientGBox);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(12, 15, 121, 71));
        gridLayout_2 = new QGridLayout(formLayoutWidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        portretBtn = new QToolButton(formLayoutWidget);
        orient_BtnGroup = new QButtonGroup(AddTemplate);
        orient_BtnGroup->setObjectName(QString::fromUtf8("orient_BtnGroup"));
        orient_BtnGroup->addButton(portretBtn);
        portretBtn->setObjectName(QString::fromUtf8("portretBtn"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/portret.png"), QSize(), QIcon::Normal, QIcon::Off);
        portretBtn->setIcon(icon);
        portretBtn->setIconSize(QSize(32, 32));
        portretBtn->setCheckable(true);
        portretBtn->setChecked(false);

        gridLayout_2->addWidget(portretBtn, 0, 0, 1, 1);

        landscapeBtn = new QToolButton(formLayoutWidget);
        orient_BtnGroup->addButton(landscapeBtn);
        landscapeBtn->setObjectName(QString::fromUtf8("landscapeBtn"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/landscape.png"), QSize(), QIcon::Normal, QIcon::Off);
        landscapeBtn->setIcon(icon1);
        landscapeBtn->setIconSize(QSize(32, 32));
        landscapeBtn->setCheckable(true);
        landscapeBtn->setAutoRaise(false);

        gridLayout_2->addWidget(landscapeBtn, 0, 1, 1, 1);

        p_orient_label = new QLabel(formLayoutWidget);
        p_orient_label->setObjectName(QString::fromUtf8("p_orient_label"));
        QFont font;
        font.setFamily(QString::fromUtf8("Tahoma"));
        p_orient_label->setFont(font);
        p_orient_label->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(p_orient_label, 1, 0, 1, 1);

        l_orient_label = new QLabel(formLayoutWidget);
        l_orient_label->setObjectName(QString::fromUtf8("l_orient_label"));
        l_orient_label->setFont(font);
        l_orient_label->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(l_orient_label, 1, 1, 1, 1);

        marginGBox = new QGroupBox(AddTemplate);
        marginGBox->setObjectName(QString::fromUtf8("marginGBox"));
        marginGBox->setGeometry(QRect(10, 245, 441, 91));
        gridLayoutWidget = new QWidget(marginGBox);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 11, 421, 71));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(gridLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        label_4 = new QLabel(gridLayoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_4, 1, 0, 1, 1);

        label_5 = new QLabel(gridLayoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_5, 0, 2, 1, 1);

        label_6 = new QLabel(gridLayoutWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_6, 1, 2, 1, 1);

        margin_topSpBox = new QSpinBox(gridLayoutWidget);
        margin_topSpBox->setObjectName(QString::fromUtf8("margin_topSpBox"));
        margin_topSpBox->setValue(0);

        gridLayout->addWidget(margin_topSpBox, 0, 1, 1, 1);

        margin_bottomSBox = new QSpinBox(gridLayoutWidget);
        margin_bottomSBox->setObjectName(QString::fromUtf8("margin_bottomSBox"));
        margin_bottomSBox->setValue(0);

        gridLayout->addWidget(margin_bottomSBox, 0, 3, 1, 1);

        margin_leftSpBox = new QSpinBox(gridLayoutWidget);
        margin_leftSpBox->setObjectName(QString::fromUtf8("margin_leftSpBox"));
        margin_leftSpBox->setValue(0);

        gridLayout->addWidget(margin_leftSpBox, 1, 1, 1, 1);

        margin_rightSpBox = new QSpinBox(gridLayoutWidget);
        margin_rightSpBox->setObjectName(QString::fromUtf8("margin_rightSpBox"));
        margin_rightSpBox->setValue(0);

        gridLayout->addWidget(margin_rightSpBox, 1, 3, 1, 1);

        page_sizeGBox = new QGroupBox(AddTemplate);
        page_sizeGBox->setObjectName(QString::fromUtf8("page_sizeGBox"));
        page_sizeGBox->setGeometry(QRect(160, 151, 291, 91));
        gridLayoutWidget_3 = new QWidget(page_sizeGBox);
        gridLayoutWidget_3->setObjectName(QString::fromUtf8("gridLayoutWidget_3"));
        gridLayoutWidget_3->setGeometry(QRect(10, 10, 271, 81));
        gridLayout_4 = new QGridLayout(gridLayoutWidget_3);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        label_10 = new QLabel(gridLayoutWidget_3);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_4->addWidget(label_10, 0, 0, 1, 1);

        pageSizeCBox = new QComboBox(gridLayoutWidget_3);
        pageSizeCBox->setObjectName(QString::fromUtf8("pageSizeCBox"));

        gridLayout_4->addWidget(pageSizeCBox, 0, 1, 1, 1);

        label_9 = new QLabel(gridLayoutWidget_3);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_4->addWidget(label_9, 1, 0, 1, 1);

        label_11 = new QLabel(gridLayoutWidget_3);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_4->addWidget(label_11, 2, 0, 1, 1);

        p_widthLEd = new QLineEdit(gridLayoutWidget_3);
        p_widthLEd->setObjectName(QString::fromUtf8("p_widthLEd"));
        p_widthLEd->setReadOnly(true);

        gridLayout_4->addWidget(p_widthLEd, 1, 1, 1, 1);

        p_heightLEd = new QLineEdit(gridLayoutWidget_3);
        p_heightLEd->setObjectName(QString::fromUtf8("p_heightLEd"));
        p_heightLEd->setReadOnly(true);

        gridLayout_4->addWidget(p_heightLEd, 2, 1, 1, 1);

        QWidget::setTabOrder(t_name_lineEd, descTextEdit);
        QWidget::setTabOrder(descTextEdit, portretBtn);
        QWidget::setTabOrder(portretBtn, landscapeBtn);
        QWidget::setTabOrder(landscapeBtn, pageSizeCBox);
        QWidget::setTabOrder(pageSizeCBox, margin_topSpBox);
        QWidget::setTabOrder(margin_topSpBox, margin_leftSpBox);
        QWidget::setTabOrder(margin_leftSpBox, margin_bottomSBox);
        QWidget::setTabOrder(margin_bottomSBox, margin_rightSpBox);
        QWidget::setTabOrder(margin_rightSpBox, addBtn);
        QWidget::setTabOrder(addBtn, cancelBtn);
        QWidget::setTabOrder(cancelBtn, date_lineEd);
        QWidget::setTabOrder(date_lineEd, author_lineEd);

        retranslateUi(AddTemplate);
        QObject::connect(cancelBtn, SIGNAL(clicked()), AddTemplate, SLOT(reject()));
        QObject::connect(addBtn, SIGNAL(clicked()), AddTemplate, SLOT(accept()));

        QMetaObject::connectSlotsByName(AddTemplate);
    } // setupUi

    void retranslateUi(QDialog *AddTemplate)
    {
        AddTemplate->setWindowTitle(QApplication::translate("AddTemplate", "\320\241\320\276\320\267\320\264\320\260\320\275\320\270\320\265 \320\275\320\276\320\262\320\276\320\263\320\276 \320\277\320\276\320\273\321\214\320\267\320\276\320\262\320\260\321\202\320\265\320\273\321\214\321\201\320\272\320\276\320\263\320\276 \321\210\320\260\320\261\320\273\320\276\320\275\320\260", 0, QApplication::UnicodeUTF8));
        t_descGBox->setTitle(QApplication::translate("AddTemplate", "\320\236\320\277\320\270\321\201\320\260\320\275\320\270\320\265 \321\210\320\260\320\261\320\273\320\276\320\275\320\260", 0, QApplication::UnicodeUTF8));
        addBtn->setText(QApplication::translate("AddTemplate", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \321\210\320\260\320\261\320\273\320\276\320\275 \320\272\320\260\320\272....", 0, QApplication::UnicodeUTF8));
        cancelBtn->setText(QApplication::translate("AddTemplate", "\320\276\321\202\320\274\320\265\320\275\320\260", 0, QApplication::UnicodeUTF8));
        t_nameGBox->setTitle(QApplication::translate("AddTemplate", "\320\235\320\260\320\267\320\262\320\260\320\275\320\270\320\265 \321\210\320\260\320\261\320\273\320\276\320\275\320\260", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("AddTemplate", "\320\236\320\261\321\211\320\260\321\217 \320\270\320\275\321\204\320\276\321\200\320\274\320\260\321\206\320\270\321\217", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("AddTemplate", "  \320\220\320\262\321\202\320\276\321\200:", 0, QApplication::UnicodeUTF8));
        author_lineEd->setText(QApplication::translate("AddTemplate", "\320\235\320\265 \321\203\321\201\321\202\320\260\320\275\320\276\320\262\320\273\320\265\320\275", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("AddTemplate", "  \320\224\320\260\321\202\320\260 \321\201\320\276\320\267\320\264\320\260\320\275\320\270\321\217:  ", 0, QApplication::UnicodeUTF8));
        page_orientGBox->setTitle(QApplication::translate("AddTemplate", "\320\236\321\200\320\270\320\265\320\275\321\202\320\260\321\206\320\270\321\217", 0, QApplication::UnicodeUTF8));
        portretBtn->setText(QApplication::translate("AddTemplate", "...", 0, QApplication::UnicodeUTF8));
        landscapeBtn->setText(QApplication::translate("AddTemplate", "...", 0, QApplication::UnicodeUTF8));
        p_orient_label->setText(QApplication::translate("AddTemplate", "\320\232\320\275\320\270\320\266\320\275\320\260\321\217", 0, QApplication::UnicodeUTF8));
        l_orient_label->setText(QApplication::translate("AddTemplate", "\320\220\320\273\321\214\320\261\320\276\320\274\320\275\320\260\321\217", 0, QApplication::UnicodeUTF8));
        marginGBox->setTitle(QApplication::translate("AddTemplate", "\320\237\320\276\320\273\321\217", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("AddTemplate", "\320\222\320\265\321\200\321\205\320\275\320\265\320\265", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("AddTemplate", "\320\233\320\265\320\262\320\276\320\265", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("AddTemplate", "\320\235\320\270\320\266\320\275\320\265\320\265", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("AddTemplate", "\320\237\321\200\320\260\320\262\320\276\320\265", 0, QApplication::UnicodeUTF8));
        page_sizeGBox->setTitle(QApplication::translate("AddTemplate", "\320\240\320\260\320\267\320\274\320\265\321\200 \320\261\321\203\320\274\320\260\320\263\320\270", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("AddTemplate", "\320\240\320\260\320\267\320\274\320\265\321\200 \320\261\321\203\320\274\320\260\320\263\320\270", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("AddTemplate", "\320\250\320\270\321\200\320\270\320\275\320\260", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("AddTemplate", "\320\222\321\213\321\201\320\276\321\202\320\260", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(AddTemplate);
    } // retranslateUi

};

namespace Ui {
    class AddTemplate: public Ui_AddTemplate {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDTEMPLATE_H
