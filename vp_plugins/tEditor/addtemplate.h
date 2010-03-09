#ifndef ADDTEMPLATE_H
#define ADDTEMPLATE_H

#include "itmpl_sql_plugin.h"
#include "mytypes.h"
#include <QtGui/QDialog>

class TemplateInfoEditModel;
class EditPagesModel;
class QSqlQueryModel;
class QSqlTableModel;


using namespace VPrn;

namespace Ui {
    class AddTemplate;
}

class AddTemplate : public QDialog {
    Q_OBJECT
public:

    AddTemplate(QWidget *parent = 0);
    ~AddTemplate();

    void setLocalTemplatesDir(const QString &l_dir){local_dir = l_dir;};
    void setEnableGUI(bool mode);

    /**
      * @fn default_init()
      * @brief Начальная настройка окна Информация о шаблоне
      * задан порядок инициализации моделей
      */
    void default_init();

    /**
      * @fn setPageSizeModel(QSqlQueryModel *model)
      * @brief Установим и настроим модель РАЗМЕРЫ_ЛИСТА
      */
    void setPageSizeModel(QSqlQueryModel *model){pSizeModel = model;};
    /**
      * @fn setInfoModel(TemplateInfoEditModel *model)
      * @brief Установим и настроим модель ИНФО_ШАБЛОНА
      */
    void setInfoModel(TemplateInfoEditModel *model){tInfoModel = model;};

    /**
      * @fn setPagesModel(QSqlQueryModel * model)
      * @brief Установим и настроим модель СТРАНИЦЫ_ШАБЛОНА
      */
    void setPagesModel(EditPagesModel * model){m_pagesModel=model;};
signals:
    void needCreateEmptyTemplates(QString &fileName);

    void error(pluginsError errCode,QString error_message);

private slots:
    void setPortret();
    void setLandscape();

    /**
      * @fn При выборе пользователем из выпадающего списка размера страницы
      * ищем ID в модели РАЗМЕР_ЛИСТА и записываем в модель ИНФО_ШАБЛОНА
      */
    void setCurrentPageSize(const QString &psize);

protected:
    void changeEvent(QEvent *e);
    void closeEvent ( QCloseEvent * event );
protected slots:
    void accept();

private:
    Ui::AddTemplate *ui;
    QFont boldFont;
    QFont normalFont;
    QString local_dir;
    int currentPSizeId;
    QString currentHumanPSize;

    bool work_mode;

    /// указатель на модель РАЗМЕР_ЛИСТА
    QSqlQueryModel *pSizeModel;
    /// Указатель на модель ИНФО_ШАБЛОНА
    TemplateInfoEditModel *tInfoModel;

    ///Указатель на модель СТРАНИЦЫ_ШАБЛОНА
    EditPagesModel * m_pagesModel;

    void showInfo(const QString & info);
    void connector();
    /**
      * @fn getIndexInPSizeModel(const QString pSizeHuman);
      * @brief Получает в модели РАЗМЕР_ЛИСТА ID для строки pSizeHuman
      */
    int getIndexInPSizeModel(const QString pSizeHuman);
    /**
      * @fn translatePSizeID2CBoxIndex(int psize_id);
      * @brief Получает в модели РАЗМЕР_ЛИСТА номер строки для ID
      */
    int translatePSizeID2ModelRow(int psize_id);

    /**
      * @fn Получает данные из модели ИНФО_ШАБЛОНА и записывает их в поля ввода
      */
    void getData4Models();
    /**
      * @fn Получает данные из полей ввода и записывает их в модель ИНФО_ШАБЛОНА
      */
    void setData4Models();
};

#endif // ADDTEMPLATE_H
