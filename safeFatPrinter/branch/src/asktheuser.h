#ifndef ASKTHEUSER_H
#define ASKTHEUSER_H

#include <QtGui/QDialog>

namespace Ui {
    class AskTheUser;
}

class AskTheUser : public QDialog {
    Q_OBJECT
public:
    AskTheUser(QWidget *parent = 0);
    ~AskTheUser();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::AskTheUser *ui;
};

#endif // ASKTHEUSER_H
