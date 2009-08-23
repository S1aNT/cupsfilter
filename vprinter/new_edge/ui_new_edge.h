/********************************************************************************
** Form generated from reading ui file 'new_edge.ui'
**
** Created: Sun 23. Aug 07:54:36 2009
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_NEW_EDGE_H
#define UI_NEW_EDGE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_new_edgeClass
{
public:

    void setupUi(QWidget *new_edgeClass)
    {
        if (new_edgeClass->objectName().isEmpty())
            new_edgeClass->setObjectName(QString::fromUtf8("new_edgeClass"));
        new_edgeClass->resize(400, 300);

        retranslateUi(new_edgeClass);

        QMetaObject::connectSlotsByName(new_edgeClass);
    } // setupUi

    void retranslateUi(QWidget *new_edgeClass)
    {
        new_edgeClass->setWindowTitle(QApplication::translate("new_edgeClass", "new_edge", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(new_edgeClass);
    } // retranslateUi

};

namespace Ui {
    class new_edgeClass: public Ui_new_edgeClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEW_EDGE_H
