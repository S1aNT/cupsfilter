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
#include <QLineEdit>
#include <QTimer>
#include <QDateEdit>
#include <QSignalMapper>
#include <QPushButton>
 #include <QTimer>



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
    void printOverSidePage(int RowId);
    void printWithTemplate(int RowId);
public slots:
    //void MBExist(bool state);
    void extenedAnswerMB(int RowId);
    void parserDocData(QString doc_data);
    void confirmMB(QString &mb);

    void setMBCompleter(QStringList &cmp);
    void selectWorkPage();
    void selectInfoPage();
    void enablePrintButton();
private slots:
    void checkMBdublicat();
    // Проверка что поле не пустое
    void isFileldNotEmpty(int sender_id);
protected:
    void changeEvent(QEvent *e);

    void createConnections();
    void enableInputFields(bool state);

protected slots:
  void accept();
  //void reject();

private:
    Ui::askTheUser *m_ui;
     QTimer *timer;
    QDataWidgetMapper *mapper;
    int currentMode;
    QString  currentMB;
    bool isDocChecked; // Статус документа проверялся ли он по базе аудита
    QLineEdit documentStatus;
    QCompleter *MBcompleter;
    QStandardItemModel * mainModel;
    QSignalMapper *signalMapper;
};

#endif // ASKTHEUSER_H
