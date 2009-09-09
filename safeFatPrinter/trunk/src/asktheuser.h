#ifndef ASKTHEUSER_H
#define ASKTHEUSER_H
#include <QtGui/QDialog>


namespace Ui {
    class askTheUser;
}

class askTheUser : public QDialog {
    Q_OBJECT
public:
    askTheUser(QWidget *parent = 0);
    ~askTheUser();

public slots:

protected:
    void changeEvent(QEvent *e);

private:
    Ui::askTheUser *m_ui;
};

#endif // ASKTHEUSER_H
