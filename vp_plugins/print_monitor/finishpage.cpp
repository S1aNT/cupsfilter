#include "finishpage.h"

#include <QtGui/QVBoxLayout>
#include <QtGui/QRadioButton>
#include <QtGui/QCheckBox>

FinishPage::FinishPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(QObject::trUtf8("Подтверждение распечаттного документа."));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/select_mode.png"));

}
