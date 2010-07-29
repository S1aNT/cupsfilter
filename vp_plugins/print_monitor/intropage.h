#ifndef INTORPAGE_H
#define INTORPAGE_H

#include <QtGui/QWidget>

QT_FORWARD_DECLARE_CLASS ( QCheckBox )

class IntroPage : public QWidget
{
    Q_OBJECT
public:    
    explicit IntroPage(QWidget *parent = 0);    
    /**
      * @fn bool enableNext();
      * @brief Разрешает преход на слудующую страницу
      * если все шаги инициализации выполнены
      */
    bool enableNext();
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
