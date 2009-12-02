#ifndef ADDELEMDLG_H
#define ADDELEMDLG_H

#include <QDialog>

namespace Ui {
    class addElemDlg;
}

class addElemDlg : public QDialog {
    Q_OBJECT
public:
    addElemDlg(QWidget *parent = 0);
    ~addElemDlg();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::addElemDlg *ui;
};

#endif // ADDELEMDLG_H
