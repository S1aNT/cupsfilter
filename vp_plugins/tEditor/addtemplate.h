#ifndef ADDTEMPLATE_H
#define ADDTEMPLATE_H

#include <QDialog>
#include <QStringListModel>
#include <QFont>
#include "templ_info.h"

namespace Ui {
    class AddTemplate;
}

class AddTemplate : public QDialog {
    Q_OBJECT
public:
    AddTemplate(QWidget *parent = 0);
    ~AddTemplate();
    //void setUserName(const QString & name);
    void setPageSize(QStringListModel *page_size_model);
    void setLocalTemplatesDir(const QString &l_dir){local_dir = l_dir;};
    void setTemplatesInfo(Templ_info templ_Info);
    void setEnableGUI(bool mode);

signals:

    void needCreateEmptyTemplates(QString &fileName);
    /*
    void needCreateEmptyTemplates(const QString & file_name,
                                  const QString & t_author,
                                  const QString & t_name,
                                  const QString & t_desc,
                                  const QString & p_size,
                                  bool p_orient,
                                  const QString & c_date,
                                  qreal m_top,
                                  qreal m_bottom,
                                  qreal m_right,
                                  qreal m_left);
                                  */
private slots:
    void set_portret();
    void set_landscape();
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
      * @brief начальная настройка шаблона
      */
    void default_init();
protected slots:
    void accept();
private:
    Ui::AddTemplate *ui;
    QFont boldFont;
    QFont normalFont;
    QString local_dir;

    bool work_mode;
    Templ_info tInfo;

};

#endif // ADDTEMPLATE_H
