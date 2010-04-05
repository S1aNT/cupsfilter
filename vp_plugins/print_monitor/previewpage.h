#ifndef PREVIEWPAGE_H
#define PREVIEWPAGE_H

#include "mytypes.h"
using namespace VPrn;

#include <QtGui/QWizardPage>


QT_FORWARD_DECLARE_CLASS ( QLabel )
QT_FORWARD_DECLARE_CLASS ( QCheckBox )

class PreViewPage : public QWizardPage
{
Q_OBJECT
public:
    explicit PreViewPage(QWidget *parent = 0);

signals:

public slots:
private:
    QLabel *topLabel;

};

#endif // PREVIEWPAGE_H
