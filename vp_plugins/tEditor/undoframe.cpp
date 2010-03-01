#include "undoframe.h"


#include <QtGui/QDockWidget>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QUndoView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <QtGui/QUndoGroup>

UndoFrame::UndoFrame(QWidget *parent) :
        QDockWidget(parent)
{
    //resize(100, 420);
    dockWidgetContents = new QWidget();
    dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
    vboxLayout = new QVBoxLayout(dockWidgetContents);
    vboxLayout->setSpacing(4);
    vboxLayout->setMargin(0);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    hboxLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
    hboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
    hboxLayout->setMargin(0);
#endif
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem);

    label = new QLabel(dockWidgetContents);
    label->setObjectName(QString::fromUtf8("label"));

    hboxLayout->addWidget(label);

    undoLimit = new QSpinBox(dockWidgetContents);
    undoLimit->setObjectName(QString::fromUtf8("undoLimit"));

    hboxLayout->addWidget(undoLimit);


    vboxLayout->addLayout(hboxLayout);

    undoView = new QUndoView(dockWidgetContents);
    undoView->setObjectName(QString::fromUtf8("undoView"));
    undoView->setAlternatingRowColors(false);

    vboxLayout->addWidget(undoView);

    setWidget(dockWidgetContents);
    retranslateUi();
connect (undoLimit, SIGNAL(valueChanged(int)),
         this, SIGNAL(undoLimitChange(int))
        );
}



void UndoFrame::setUndoViewGroup(QUndoGroup * undoGroup)
{
    undoView->setGroup(undoGroup);
    undoView->setEmptyLabel(tr("Без изменений"));
    undoView->setCleanIcon(QIcon(":/ok.png"));
}


void UndoFrame::retranslateUi()
{
    this->setWindowTitle(QObject::trUtf8("История..."));
    label->setText(QObject::trUtf8("Шагов назад"));

} // retranslateUi

void UndoFrame::changeEvent(QEvent *e)
{
    QDockWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        this->retranslateUi();
        break;
    default:
        break;
    }
}
