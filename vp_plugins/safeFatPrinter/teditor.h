#ifndef TEDITOR_H
#define TEDITOR_H

#include <QDialog>
#include "view.h"

namespace Ui {
    class TEditor;
}

class TEditor : public QDialog {
    Q_OBJECT
public:
    TEditor(QWidget *parent = 0);
    ~TEditor();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::TEditor *ui;
};

#endif // TEDITOR_H
