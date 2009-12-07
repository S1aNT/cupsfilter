#ifndef ADDTEMPLATE_H
#define ADDTEMPLATE_H

#include <QDialog>
#include <QStringListModel>
#include <QFont>

namespace Ui {
    class AddTemplate;
}

class AddTemplate : public QDialog {
    Q_OBJECT
public:
    AddTemplate(QWidget *parent = 0);
    ~AddTemplate();
    void setUserName(const QString & name);
    void setPageSize(QStringListModel *page_size_model);

signals:
    void needCreateEmptyTemplates(const QString & t_name,
                                  const QString & t_desc,
                                  const QString & p_size,
                                  bool p_orient,
                                  const QString & c_date,
                                  qreal m_top,
                                  qreal m_bottom,
                                  qreal m_right,
                                  qreal m_left);
private slots:
    void set_portret();
    void set_landscape();
    void setCurrentPageSize(const QString &psize);
protected:
    void changeEvent(QEvent *e);
    void showInfo(const QString & info);
protected slots:
    void accepted();
private:
    Ui::AddTemplate *ui;
    QFont boldFont;
    QFont normalFont;
    bool page_orient;
    QString page_size;
    QString templ_name;
    QString templ_desc;

    qreal page_height;               //
    qreal page_width;                //
    qreal margin_top;                //
    qreal margin_bottom;             //
    qreal margin_left;               //
    qreal margin_right;
};

#endif // ADDTEMPLATE_H
