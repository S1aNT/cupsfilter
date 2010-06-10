#ifndef INTORPAGE_H
#define INTORPAGE_H

#include "mytypes.h"
using namespace VPrn;

#include <QtGui/QWizardPage>

QT_FORWARD_DECLARE_CLASS ( QLabel )
QT_FORWARD_DECLARE_CLASS ( QCheckBox )

class IntroPage : public QWizardPage
{
    Q_OBJECT
public:
    IntroPage(QWidget *parent = 0);
    void setVisible(bool visible);
public slots:
    /**
      * @brief  Эти слоты взводят галочки в Checkbox,когда приложение
      * проходит через обязательные контрольные точки при старте
      * @fn void setInputFile();          // Есть файл для печати
      * @fn void setLocalSrvRegistr();    // Регистрация на лок. сервере
      * @fn void setRemoteDemonRegistr(); // Регистрация на удал. сервере
      * @fn void setReciveUserName();     // Полученно имя пользователя
      * @fn void setReciveUserMandat();   // Получен мандат пользователя
      * @fn void setRecivePrintersList(); // Получен список принтеров
      * @fn void setReciveSecLevelList(); // Получен список уровней секретности

    */
    void setInputFile();
    void setLocalSrvRegistr();
    void setRemoteDemonRegistr();
    void setReciveUserName();
    void setReciveUserMandat();
    void setRecivePrintersList();
    void setReciveSecLevelList();
private:
    QLabel *topLabel;
    QCheckBox *inputFile_step;
    QCheckBox *loadPlugin_step;
    QCheckBox *connect2LocalServer_step;
    QCheckBox *connect2RemoteDemon_step;
    QCheckBox *reciveUserName_step;
    QCheckBox *reciveUserMandat_step;
    QCheckBox *reciveSecLabel_step;
    QCheckBox *recivePrintersList_step;
};


#endif // INTORPAGE_H
