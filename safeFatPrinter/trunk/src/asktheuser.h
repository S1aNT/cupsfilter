#ifndef ASKTHEUSER_H
#define ASKTHEUSER_H
#include <QDebug>
#include <QtGui/QDialog>
#include <QDate>
#include <QCompleter>
#include <QStringListModel>
#include <QMessageBox>
#include <QRegExp>



namespace Ui {
    class askTheUser;
}

class askTheUser : public QDialog {
    Q_OBJECT
public:
    askTheUser(QWidget *parent = 0);
    ~askTheUser();
    void setMode (int work_mode);
signals:
    void isMBExist(QString mb);
    void save2base(QString msg);
    void printOverSidePage(QString mb);
    void printWithTemplate(QString templ);
public slots:
    void parserDocData(QString doc_data);
    void setMBCompleter(QStringList &cmp);

private slots:
    void docNameChanged();
    void secretStampChanged(QString stamp);
    void copyNumberChanged(int cpNum);
    void punktChanged(QString punktStr);
    void reciverOneChanged(QString res);
    void reciverTwoChanged(QString res);
    void reciverThreeChanged(QString res);
    void reciverFourChanged(QString res);
    void reciverFiveChanged(QString res);
    void executorChanged(QString fio);
    void pressmanChanged(QString fio);
    void phoneChanged(QString phone);
    void invNumberChanged(QString iNumber);
    void dateFiledChanged(QDate DT);
    void mbNumChanged(QString mb);
    void templatesChanged(QString Templates);
protected:
    void changeEvent(QEvent *e);
    void save2field(QString &data);
protected slots:
  void accept();
  //void reject();

private:
    Ui::askTheUser *m_ui;
    int currentMode;
    QString docName;
    QString secretStamp;
    QString punkt;
    QString mbNumber;
    QString templ;
    QString invNumber;
    int copyNumber;
    QString reciver_1;
    QString reciver_2;
    QString reciver_3;
    QString reciver_4;
    QString reciver_5;
    QString executor;
    QString pressman;
    QString phoneNumber;
    QDate   date;
    QCompleter *MBcompleter;
};

#endif // ASKTHEUSER_H
