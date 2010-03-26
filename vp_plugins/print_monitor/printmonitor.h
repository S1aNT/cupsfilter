#ifndef PRINTMONITOR_H
#define PRINTMONITOR_H

#include "mytypes.h"
#include "engine.h"
#include "getusernamemandatdlg.h"
#include "wizardpages.h"


#include <QtGui/QWizard>
#include <QtCore/QMap>

using namespace VPrn;

//namespace Ui {
//    class PrintMonitor;
//
//}

class QPoint;

class PrintMonitor : public QWizard {
    Q_OBJECT
public:    

    PrintMonitor(QWidget *parent = 0);
    ~PrintMonitor();

//protected:
//    void changeEvent(QEvent *e);
//    int nextId() const;
private slots:
    void showInfoMessage(const QString &info);
    void showCritMessage(const QString &info);
    /**
      * @fn void do_needAuthUser(const QString &login_mandat_list);
      * @param QString &login_mandat_list сообщение в формате:
      * login;:;mandat_i;:;mandat_i+1;:;....mandat_n
      * @brief Сервер в панике, не знает как нас звать величать,
      * прислал логин и набор мандатов, просит выбрать.
      */
    void do_needAuthUser(const QString &login_mandat_list);
    /**
      * @fn void setPrinterList (const QString &pList);
      * @brief Сервер вернул список доступных принтеров, отформатируем его
      * по канонам дзенбуддизма и сделаем доступным для приложения
      */
    void setPrinterList (const QString &pList);

    void showHelp();
private:
//    Ui::PrintMonitor *ui;
    /**
      * @var core_app Основной объект программы
      */
    Engine   *core_app;
    getUserNameMandatDlg *UMDlg;
    QStringList printer_list;
    QMap <QString,QString> printer_device_list;

    QPoint getDeskTopCenter(int width,int height);
    IntroPage * intro_page;
    SelectPage * select_page;
};


#endif // PRINTMONITOR_H
