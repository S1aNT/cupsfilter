#ifndef FINISHPAGE_H
#define FINISHPAGE_H

#include "mytypes.h"
using namespace VPrn;

#include <QtGui/QWizardPage>


QT_FORWARD_DECLARE_CLASS ( QLabel )
QT_FORWARD_DECLARE_CLASS ( QCheckBox )

class FinishPage : public QWizardPage
{
Q_OBJECT
public:
    explicit FinishPage(QWidget *parent = 0);

signals:

public slots:
private:
    QLabel *topLabel;

};

#endif // PREVIEWPAGE_H
