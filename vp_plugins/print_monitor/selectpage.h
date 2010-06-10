#ifndef SELECTPAGE_H
#define SELECTPAGE_H

#include "mytypes.h"
using namespace VPrn;


#include <QtGui/QWizardPage>
#include <QtCore/QMap>

QT_FORWARD_DECLARE_CLASS ( QRadioButton )

QT_FORWARD_DECLARE_CLASS ( QCheckBox )
QT_FORWARD_DECLARE_CLASS ( QComboBox )

QT_FORWARD_DECLARE_CLASS ( QLabel )

class SelectPage : public QWizardPage
{
    Q_OBJECT
public:
    SelectPage(QWidget *parent = 0);

    QString getSeclectPrinter(){return sel_printer;}
    int nextId() const;

public slots:
    void setPrinterList(QStringList &pList);
private slots:
    void setCurrentPrinter(const QString &p);

private:
    QString sel_printer;
    QLabel *topLabel;
    QRadioButton * markBrakDoc;
    QRadioButton * accountingDoc;
    QRadioButton * printDoc;
    QRadioButton * both_step;
    QComboBox    * printersCBox;

};


#endif // SELECTPAGE_H
