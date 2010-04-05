#include "previewpage.h"

#include <QtGui/QVBoxLayout>
#include <QtGui/QRadioButton>
#include <QtGui/QCheckBox>

PreViewPage::PreViewPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(QObject::trUtf8("Предварительный просмотр сформированного документа."));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/select_mode.png"));

}
