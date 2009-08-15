/********************************************************************************
** Form generated from reading ui file 'docview.ui'
**
** Created: Fri 14. Aug 13:31:08 2009
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_DOCVIEW_H
#define UI_DOCVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_DocView
{
public:

    void setupUi(QFrame *DocView)
    {
        if (DocView->objectName().isEmpty())
            DocView->setObjectName(QString::fromUtf8("DocView"));
        DocView->resize(400, 300);
        DocView->setFrameShape(QFrame::StyledPanel);
        DocView->setFrameShadow(QFrame::Raised);

        retranslateUi(DocView);

        QMetaObject::connectSlotsByName(DocView);
    } // setupUi

    void retranslateUi(QFrame *DocView)
    {
        DocView->setWindowTitle(QApplication::translate("DocView", "Frame", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(DocView);
    } // retranslateUi

};

namespace Ui {
    class DocView: public Ui_DocView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DOCVIEW_H
