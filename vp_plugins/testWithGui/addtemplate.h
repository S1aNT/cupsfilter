#ifndef ADDTEMPLATE_H
#define ADDTEMPLATE_H

#include <QDialog>
#include <QStringListModel>
#include <QFont>

#include "itmpl_sql_plugin.h"


class TemplateInfoEditModel;
class QSqlQueryModel;
class QDataWidgetMapper;


//#include "templ_info.h"

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
      * @fn Установим и настроим модель РАЗМЕРЫ_ЛИСТА
      */
    void setPageSizeModel(QSqlQueryModel *model);
    /**
      * @fn Установим и настроим модель ИНФО_ШАБЛОНА
      */
    void setInfoModel(TemplateInfoEditModel *model);

signals:
    void needCreateEmptyTemplates(QString &fileName);

private slots:
    void set_portret();
    void set_landscape();

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
    void showInfo(const QString & info);
    /**
      * @fn начальная настройка шаблона
      */
    void default_init();
    void connector();
protected slots:
    void accept();
    /**
      * @fn Ищет в модели РАЗМЕР_ЛИСТА ID для строки pSizeHuman
      */
    int getIndexInPSizeModel(const QString pSizeHuman);

private:
    Ui::AddTemplate *ui;
    QFont boldFont;
    QFont normalFont;
    QString local_dir;

    bool work_mode;
    /// связь между моделью РАЗМЕРЫ_ЛИСТА и элементами отображения
    QDataWidgetMapper *pSizeDWMapper;

    /// связь между моделью ИНФО_ШАБЛОНА и элементами отображения
    QDataWidgetMapper *tInfoDWMapper;

    /// указатель на модель РАЗМЕР_ЛИСТА
    QSqlQueryModel *pSizeModel;
    /// Указатель на модель ИНФО_ШАБЛОНА
    TemplateInfoEditModel *tInfoModel;
};

#endif // ADDTEMPLATE_H
