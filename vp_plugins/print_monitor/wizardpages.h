#ifndef WIZARDPAGES_H
#define WIZARDPAGES_H
#include "mytypes.h"
using namespace VPrn;


#include <QtGui/QWizardPage>

class QLabel;
class QCheckBox;
class QRadioButton;

class IntroPage : public QWizardPage
 {
     Q_OBJECT
 public:
     IntroPage(QWidget *parent = 0);
     int nextId() const;
 public slots:
     void setPluginsLoad();
     void setLocalSrvRegistr();
     void setRemoteDemonRegistr();
     void setReciveUserName();
     void setReciveUserMandat();
     void setRecivePrintersList();
     void setReciveSecLevelList();
 private:
     QLabel *topLabel;
     QCheckBox *loadPlugin_step;
     QCheckBox *connect2LocalServer_step;
     QCheckBox *connect2RemoteDemon_step;
     QCheckBox *reciveUserName_step;
     QCheckBox *reciveUserMandat_step;
     QCheckBox *reciveSecLabel_step;
     QCheckBox *recivePrintersList_step;
 };

class SelectPage : public QWizardPage
{
    Q_OBJECT
public:
    SelectPage(QWidget *parent = 0);
    int nextId() const;
private:
    QLabel *topLabel;
    QRadioButton * accountingDoc;
    QRadioButton * printDoc;
    QCheckBox *both_step;
    int work_mode;
};

#endif // WIZARDPAGES_H
