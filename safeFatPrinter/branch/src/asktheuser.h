#ifndef ASKTHEUSER_H
#define ASKTHEUSER_H

#include <QtGui/QDialog>

namespace Ui {
    class AskTheUser;
}

class AskTheUser : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(bool enabledPapersAccountsOutSide READ isEnabledPAO WRITE setEnabledPAO)
    Q_PROPERTY(bool checkedPapersAccountsOutSide READ isCheckedPAO)
public:
    AskTheUser(QWidget *parent = 0);
    ~AskTheUser();
    bool isEnabledPAO() const;
    bool isCheckedPAO() const;
    void setEnabledPAO(bool flag);
    void setWorkMode(int mode);
protected:
    void changeEvent(QEvent *e);

private:
    Ui::AskTheUser *ui;
};

#endif // ASKTHEUSER_H
