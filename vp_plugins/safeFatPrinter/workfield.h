#ifndef WORKFIELD_H
#define WORKFIELD_H

#include <QDialog>
#include <QPixmap>
#include <QMessageBox>
#include <QStringListModel>
#include <QStandardItemModel>

#include <QSignalMapper>
#include <QDataWidgetMapper>
#include <QFileDialog>

#include "previewwnd.h"
#include "tech_global.h"


using namespace SafeVirtualPrinter;

namespace Ui {
    class workField;
}

class workField : public QDialog{

    Q_OBJECT
    Q_ENUMS(WorkMode)

public:
    workField(QWidget *parent = 0);
    ~workField();
    void setPagesCount(int p_count);
    void setStampModel(QStringListModel *stamp_model);
    void setPageSizeModel(QStringListModel *page_size_model){p_size_mod =page_size_model;};
    void setUserName(const QString & user_name){userName=user_name;};
    void setModel (QStandardItemModel * model);
    void setMode (int mode_value);
    void setTemplatesModel(QStringListModel *global,QStringListModel *local);
signals:
    // Запрос на преобразование шаблона в набор из 4-х сцен
    void convertTemplatesToScenes(const QString &t_file_name);

    void checkMBInBase(const QString &mb_value,const QString &copyNum_value,WorkMode work_mode);
    void needAuthUserToPrinter(); // Требуется авторизовать пользователя на принтер
    void needPrintPage(const QString &t_file_name);
    void dontNeedPrintPage();
    void needCreateEmptyTemplates(const QString & file_name);
    /**
     * @brief Запрос на сохранение данных из модели в файл dat
     */
    void saveModelInFile();
    /**
      * @brief Пользователь выбрал шаблон
      */
    void userSelectTemplates(const QString & t);
    /**
      * @brief  Запрос к посреднику на запуск редактора
      * с отображением данных
      */
    void showEditorWithData();

public slots:
    void showInfoWindow(const QString &info);
    void showPreviewPage(QPixmap &preview_page);
    void setCurrentTemplates(QString temp);
    void doPrintAllowed(); // Демон разрешил печать !
private slots:
    void flipLabel(bool flip);
    void checkData(); // При нажатии на кнопку идет анализ введенных данных
    //Выбор локальный или глобальный каталог шаблонов
    void selectTemplatesDir(bool mode);
    void do_addTemplates(); // добавление пустого шаблона

    void do_editTemplates();
    void setStampField(QString field);
protected:
    void changeEvent(QEvent *e);
private:
    Ui::workField *ui;
    QDataWidgetMapper *mapper;
    QSignalMapper *signalMapper;

    QStandardItemModel * w_model;

    QStringListModel *p_size_mod;
    QString userName;

    QLineEdit *hide_stamp; // Скрытое поле Гриф секретности

    bool localORglobal; // Режим шаблонов локальные или глобальные
    QStringListModel *local_templ_model;
    QStringListModel *global_templ_model;
    QString currentTemplates;



    int mode;

    void setEnableField(bool e);
};

#endif // WORKFIELD_H
