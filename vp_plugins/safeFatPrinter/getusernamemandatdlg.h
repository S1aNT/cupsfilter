#ifndef GETUSERNAMEMANDATDLG_H
#define GETUSERNAMEMANDATDLG_H

#include <QtGui/QDialog>
#include <QStringListModel>


namespace Ui {
    class getUserNameMandatDlg;
}

class getUserNameMandatDlg : public QDialog {
    Q_OBJECT
public:
    getUserNameMandatDlg(QWidget *parent = 0);
    ~getUserNameMandatDlg();

    QString getCurrentMandat(){return u_mandat;}

    void setMandatModel(QStringListModel *mandat_model);
public slots:
    void setUserName(QString &userName);
private slots:
    void setCurrentMandat(QString &mandat);
protected:
    void changeEvent(QEvent *e);
private:
    Ui::getUserNameMandatDlg *ui;
    QString u_name;
    QString u_mandat;
};

#endif // GETUSERNAMEMANDATDLG_H