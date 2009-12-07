#ifndef ADDTEMPLATE_H
#define ADDTEMPLATE_H

#include <QDialog>
#include <QStringList>

namespace Ui {
    class AddTemplate;
}

class AddTemplate : public QDialog {
    Q_OBJECT
public:
    AddTemplate(QWidget *parent = 0);
    ~AddTemplate();
    void setUserName(const QString & name);
    void setPageSize(const QStringList & p_list,int page_height,int page_width);
protected:
    void changeEvent(QEvent *e);

private:
    Ui::AddTemplate *ui;
};

#endif // ADDTEMPLATE_H
