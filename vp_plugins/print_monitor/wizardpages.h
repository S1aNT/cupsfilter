#ifndef WIZARDPAGES_H
#define WIZARDPAGES_H
#include "mytypes.h"
using namespace VPrn;


#include <QtGui/QWizardPage>

class QLabel;
class QCheckBox;
class QRadioButton;
class QComboBox;
class QGroupBox;
class QLineEdit;
class QLCDNumber;
class QSpinBox;
class QDateEdit;

class IntroPage : public QWizardPage
{
    Q_OBJECT
public:
    IntroPage(QWidget *parent = 0);
    //int nextId() const;
public slots:
    /**
      * @brief  Эти слоты взводят галочки в Checkbox,когда приложение
      * проходит через обязательные контрольные точки при старте
      * @fn void setPluginsLoad();        // Загрузка плагинов
      * @fn void setLocalSrvRegistr();    // Регистрация на лок. сервере
      * @fn void setRemoteDemonRegistr(); // Регистрация на удал. сервере
      * @fn void setReciveUserName();     // Полученно имя пользователя
      * @fn void setReciveUserMandat();   // Получен мандат пользователя
      * @fn void setRecivePrintersList(); // Получен список принтеров
      * @fn void setReciveSecLevelList(); // Получен список уровней секретности

    */
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
    //int nextId() const;
public slots:
    /**
      * @fn void setPrinterList (const QString &pList);
      * @brief Сервер вернул список доступных принтеров, отформатируем его
      * по канонам дзенбуддизма и сделаем доступным для приложения
      */
    void setPrinterList(const QString &plist);

private:
    QLabel *topLabel;
    QRadioButton * markBrakDoc;
    QRadioButton * accountingDoc;
    QRadioButton * printDoc;
    QRadioButton * both_step;
    QComboBox    * printersCBox;

    QMap <QString,QString> printer_device_list;
};

class PrintDataPage : public QWizardPage
{
    Q_OBJECT
public:
    PrintDataPage(QWidget *parent = 0);
    //int nextId() const;
    void setLabelText(const QString &l_txt);
    void setMode(int m_mode);
public slots:
   /**
    * @fn void setDocConverted();
    * @brief ставит галочку в обязательном CheckBox документ сконвертирован  в pdf
    */
    void setDocConverted();
   /**
     * @fn void setPageCounted(int pages);
     * @brief Заполняет поле число страниц
    */
    void setPageCounted(int pages);

private:
    QComboBox *secretCBox;
    QComboBox *templatesCBox;

    QLineEdit *punktLineEd;
    QLineEdit *mbNumberLineEd;
    QLineEdit *pagesCountLineEd;
    QSpinBox   *e_numberSBox_2;
    QSpinBox   *e_numberSBox;

    QLineEdit *executor_lineEd;
    QLineEdit *docName_lineEd;
    QLineEdit *pressman_lineEd;
    QLineEdit *invNumber_lineEd;
    QLineEdit *telephone_lineEd;
    QDateEdit *dateField_dateEd;

    QLineEdit *reciverOne_lineEd;
    QLineEdit *reciverTwo_lineEd;
    QLineEdit *reciverThree_lineEd;
    QLineEdit *reciverFour_lineEd;
    QLineEdit *reciverFive_lineEd;

    QCheckBox *copy_checkBox_1;
    QCheckBox *copy_checkBox_2;
    QCheckBox *copy_checkBox_3;
    QCheckBox *copy_checkBox_4;
    QCheckBox *copy_checkBox_5;

    QCheckBox *docConverted_checkBox;

};
#endif // WIZARDPAGES_H
