#ifndef ASKTHEUSER_H
#define ASKTHEUSER_H
#include <QDebug>
#include <QtGui/QDialog>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QDataWidgetMapper>
#include <QSignalMapper>
#include <QLineEdit>
#include <QMessageBox>
#include <QTimer>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFileInfoList>
#include <QDir>

#include "tech_global.h"


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
    void setModel (QStandardItemModel * model);
    void setStampModel(QStringListModel *stamp_model);
    void setTemplatesPath(QString global);
public slots:
    void do_mbExist(int RowId); // Приходят сообщения если такой документ в базе есть, если RowId =-1 значит они не бракованные
    void do_mbNumberNotExist();
signals:
    void needCheckMB(QString mb_field);
    void mbNumberNotExist();
    void showTemplatesEditor();
private slots:
    void changePAOLabel(bool flag);
    void isFileldNotEmpty(int sender_id);
    void checkMB();
    void selectLocalTemplatesDir(bool mode);
protected:
    void changeEvent(QEvent *e);
    void createConnections();
    void enableInputFields(bool state);
    void showAndClear(QString &msg,bool advanced);

private:
    Ui::AskTheUser *m_ui;
    QLineEdit documentStatus;
    bool emptyFieldsExist;
    QDataWidgetMapper *mapper;
    QSignalMapper *signalMapper;
    
    int currentRow; // Текущая строка в модеи данных

    QString global_templates_dir;

};

#endif // ASKTHEUSER_H
