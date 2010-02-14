#ifndef ADDTEMPLATE_H
#define ADDTEMPLATE_H

#include <QDialog>
#include <QStringListModel>
#include <QFont>

#include "itmpl_sql_plugin.h"
#include "mytypes.h"

class TemplateInfoEditModel;
class QSqlQueryModel;
class QSqlTableModel;
class QDataWidgetMapper;


using namespace VPrn;

namespace Ui {
    class AddTemplate;
}

class AddTemplate : public QDialog {
    Q_OBJECT
public:
    AddTemplate(QWidget *parent = 0);
    ~AddTemplate();
    //void setUserName(const QString & name);
    //void setPageSize(QStringListModel *page_size_model);

    void setLocalTemplatesDir(const QString &l_dir){local_dir = l_dir;};
    //void setTemplatesInfo(Templ_info templ_Info);
    void setEnableGUI(bool mode);

    /**
      * @fn default_init() Начальная настройка окна Информация о шаблоне
      * задан порядок инициализации моделей
      */
    void default_init();

    /**
      * @fn Установим и настроим модель РАЗМЕРЫ_ЛИСТА
      */
    void setPageSizeModel(QSqlQueryModel *model){pSizeModel = model;};
    /**
      * @fn Установим и настроим модель ИНФО_ШАБЛОНА
      */
    void setInfoModel(TemplateInfoEditModel *model){tInfoModel = model;};
    //void setInfoModel2(QSqlTableModel *model){tInfoModel2 = model;};
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
    void setTemplatesName(const QString & name);
    void setTemplatesDesc();
    void setMarginTop();
    void setMarginBottom();
    void setMarginLeft();
    void setMarginRight();

protected:
    void changeEvent(QEvent *e);

protected slots:
    void accept();

private:
    Ui::AddTemplate *ui;
    QFont boldFont;
    QFont normalFont;
    QString local_dir;
    int currentPSizeId;

    bool work_mode;
    /// связь между моделью РАЗМЕРЫ_ЛИСТА и элементами отображения
    QDataWidgetMapper *pSizeDWMapper;

    /// связь между моделью ИНФО_ШАБЛОНА и элементами отображения
    QDataWidgetMapper *tInfoDWMapper;

    /// указатель на модель РАЗМЕР_ЛИСТА
    QSqlQueryModel *pSizeModel;
    /// Указатель на модель ИНФО_ШАБЛОНА
    TemplateInfoEditModel *tInfoModel;

    //QSqlTableModel *tInfoModel2;

    void showInfo(const QString & info);
    void connector();
    /**
      * @fn Ищет в модели РАЗМЕР_ЛИСТА ID для строки pSizeHuman
      */
    int getIndexInPSizeModel(const QString pSizeHuman);
    /**
      * @fn Получает данные из модели ИНФО_ШАБЛОНА и записывает их в поля ввода
      * Мой аналог QDataWidgetMapper, который "работает" очень своеобразно
      */
    void getData4Models();
    /**
      * @fn Получает данные из полей ввода и записывает их в модель ИНФО_ШАБЛОНА
      * Мой аналог QDataWidgetMapper, который "работает" очень своеобразно
      */
    void setData4Models();
};

#endif // ADDTEMPLATE_H