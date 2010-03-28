#ifndef PRINTMONITOR_H
#define PRINTMONITOR_H

#include "mytypes.h"
#include "engine.h"
#include "getusernamemandatdlg.h"
#include "wizardpages.h"


#include <QtGui/QWizard>
#include <QtCore/QMap>
#include <QtCore/QQueue>

using namespace VPrn;

//namespace Ui {
//    class PrintMonitor;
//
//}

class QPoint;
class QErrorMessage;

class PrintMonitor : public QWizard {
    Q_OBJECT
public:    

    PrintMonitor(QWidget *parent = 0);
    ~PrintMonitor();


    int nextId() const;
public slots:   
    /**
      * @fn void setFile4Work(const QString &input_file);
      * @brief На вход виртуального принтера поступает файл в формате ps
      * После проверки на валидность, т.е файл есть и доступен, он помещается в очередь
      * печати, которая обрабатывается на GateKeeper
      */
    void setFile4Work(const QString &input_file);
private slots:
    /**
      * @fn void do_needAuthUser(const QString &login_mandat_list);
      * @param QString &login_mandat_list сообщение в формате:
      * login;:;mandat_i;:;mandat_i+1;:;....mandat_n
      * @brief Сервер в панике, не знает как нас звать величать,
      * прислал логин и набор мандатов, просит выбрать.
      */
    void do_needAuthUser(const QString &login_mandat_list);
    void showHelp();
    /**
      * @fn void print_next();
      * @brief отправляет на обработку следующий файл из очереди печати
      */
    void print_next();
    //protected:
    //    void changeEvent(QEvent *e);
private:
    //    Ui::PrintMonitor *ui;
    /**
      * @var core_app;      Основной объект программы
      * @var UMDlg;         Диалоговое окно выбора мандата
      * @var m_InputFile;   Основной обрабатываемый файл
      * @var intro_page;    Стартовая страница
      * @var select_page;   Страница выбора режимов работы
      * @var printData_page; Страница заполнения карточки документа
      * @var job_list;       Очередь документов на печать,
        возникает если была попытка запуска еще одной копии приложения
      */
    Engine               * core_app;
    getUserNameMandatDlg * UMDlg;
    QString                m_InputFile;
    IntroPage            * intro_page;
    SelectPage           * select_page;
    PrintDataPage        * printData_page;
    QErrorMessage        * eMsgBox;
    QQueue <QString>       job_list;

    //-------------------------------------------------------------------------
    QPoint getDeskTopCenter(int width,int height);
};


#endif // PRINTMONITOR_H
