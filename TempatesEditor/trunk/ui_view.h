/********************************************************************************
** Form generated from reading ui file 'view.ui'
**
** Created: Thu 8. Oct 07:05:23 2009
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_VIEW_H
#define UI_VIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_View
{
public:

    void setupUi(QFrame *View)
    {
        if (View->objectName().isEmpty())
            View->setObjectName(QString::fromUtf8("View"));
        View->resize(640, 480);
        View->setFrameShape(QFrame::StyledPanel);
        View->setFrameShadow(QFrame::Raised);

        retranslateUi(View);

        QMetaObject::connectSlotsByName(View);
    } // setupUi

    void retranslateUi(QFrame *View)
    {
        View->setWindowTitle(QApplication::translate("View", "Frame", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(View);
    } // retranslateUi

};

namespace Ui {
    class View: public Ui_View {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEW_H
