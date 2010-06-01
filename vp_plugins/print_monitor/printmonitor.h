#ifndef PRINTMONITOR_H
#define PRINTMONITOR_H

#include "mytypes.h"
#include "engine.h"
#include "getusernamemandatdlg.h"

#include "intorpage.h"
#include "selectpage.h"
#include "printdatapage.h"
#include "checkdatapage.h"
#include "previewpage.h"
#include "finishpage.h"

#include <QtGui/QWizard>
#include <QtCore/QMap>
#include <QtCore/QQueue>

using namespace VPrn;

class QPoint;
class QErrorMessage;

class PrintMonitor : public QWizard {
    Q_OBJECT
public:

    PrintMonitor(QWidget *parent = 0,const QString &input_file = QString());
    ~PrintMonitor();

    //int nextId() const;
public slots:
    void appendStartMsg(const QString &start_msg);

private slots:
    /**
      * @fn void onGoodBayMsg(const QString &shutdown_info);
      * @brief Печальная весть, Хранитель_Врат умирает, пора и нам на покой
      */
    void onGoodBayMsg(const QString &shutdown_info);
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
      * @fn void sendFileToConvertor();
      * @brief отправляет на обработку следующий файл из очереди печати
      */
    void sendFileToConvertor();

    /**
      * @fn void  onReciveSecLevelList();
      * @brief Получили сообщение что готов список уровней секретности
      * Запишем его в соответсвующее поле страницы print_data
      */
    void onReciveSecLevelList();
    /**
      * @fn void check_docMB();
      * @brief Поля ввода прошли формальную проверку на корректность заполнения
      * необходимо проверить МБ документа по базе
      */
    void check_docMB();
    /**
      * @fn on_needRegisterDocInBase();
      * @brief Необходимо передать демону sql запрос на регистрацию документа
      */
    void onNeedRegisterDocInBase();
    /**
      * @fn void onSelectPreview(bool mode);
      * @brief Пользовательвыбрал режи предпросмотра страниц (Все/частично)
      * отправим запрос на оъединение документа и шаблона
      */
    void onSelectPreview(bool mode);


private:

    /**
      * @var core_app;       Основной объект программы
      * @var UMDlg;          Диалоговое окно выбора мандата
      * @var m_InputFile;    Основной обрабатываемый файл
      * @var intro_page;     Стартовая страница
      * @var select_page;    Страница выбора режимов работы
      * @var printData_page; Страница заполнения карточки документа
      * @var job_list;       Очередь документов на печать,
      * @var printer_device_list; Список device_uri для авторизации
      */
    Engine               * core_app;
    getUserNameMandatDlg * UMDlg;
    QString                m_InputFile;
    IntroPage            * intro_page;
    SelectPage           * select_page;
    PrintDataPage        * printData_page;
    CheckDataPage        * checkData_page;
    PreViewPage          * preview_page;
    FinishPage           * finish_page;

    QErrorMessage        * eMsgBox;
    QQueue <QString>       job_list;

    //-------------------------------------------------------------------------
    QPoint getDeskTopCenter(int width,int height);

};


#endif // PRINTMONITOR_H
