#ifndef ASKTHEUSER_H
#define ASKTHEUSER_H
#include <QDebug>
#include <QtGui/QDialog>
#include <QDate>
#include <QCompleter>
#include <QStringListModel>
#include <QMessageBox>
#include <QRegExp>
#include <QDataWidgetMapper>
#include <QStandardItemModel>
#include <QRegExp>
#include <QValidator>


namespace Ui {
    class askTheUser;
}

class askTheUser : public QDialog {
    Q_OBJECT
public:
    askTheUser(QWidget *parent = 0);
    ~askTheUser();
    void setViewMode (int work_mode);
    void setModel (QStandardItemModel * model);
signals:
    void isMBExist(QString mb);
    void save2base(QString msg);
    void printOverSidePage(QString mb);
    void printWithTemplate(QString templ);
public slots:
    //void MBExist(bool state);
    void parserDocData(QString doc_data);
    void setMBCompleter(QStringList &cmp);
    void selectWorkPage();
    void selectInfoPage();

private slots:
    void checkMBdublicat();
protected:
    void changeEvent(QEvent *e);

    void createConnections();
    void enableInputFields(bool state);

protected slots:
  void accept();
  //void reject();

private:
    Ui::askTheUser *m_ui;
    QDataWidgetMapper *mapper;
    int currentMode;

    QCompleter *MBcompleter;
    QStandardItemModel * mainModel;
};

#endif // ASKTHEUSER_H
