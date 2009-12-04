#ifndef ADDTEMPLATE_H
#define ADDTEMPLATE_H

#include <QDialog>

namespace Ui {
    class AddTemplate;
}

class AddTemplate : public QDialog {
    Q_OBJECT
public:
    AddTemplate(QWidget *parent = 0);
    ~AddTemplate();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::AddTemplate *ui;
};

#endif // ADDTEMPLATE_H
