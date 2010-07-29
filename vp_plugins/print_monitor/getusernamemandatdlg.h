#ifndef GETUSERNAMEMANDATDLG_H
#define GETUSERNAMEMANDATDLG_H

#include <QtGui/QDialog>
#include <QtCore/QStringList>

QT_FORWARD_DECLARE_CLASS ( QFormLayout )
QT_FORWARD_DECLARE_CLASS ( QLabel )
QT_FORWARD_DECLARE_CLASS ( QLineEdit )
QT_FORWARD_DECLARE_CLASS ( QComboBox )

class getUserNameMandatDlg : public QDialog {
    Q_OBJECT
public:
    getUserNameMandatDlg(QWidget *parent = 0);

    void setMandatList(const QStringList &mandatList);

public slots:
    void setUserName(QString &userName);
    void accept() ;
private slots:

    void enableButtons(int index);
signals:
    void setAuthData(const QString &login,const QString &mandat);
private:   

    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *userNameEd;
    QLabel *label_2;
    QComboBox *mandatCBox;
    QPushButton *okButton;
    QPushButton *cancelButton;
};

#endif // GETUSERNAMEMANDATDLG_H
