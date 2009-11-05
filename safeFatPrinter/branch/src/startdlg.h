#ifndef STARTDLG_H
#define STARTDLG_H

#include <QtGui/QDialog>
#include <QDateTime>
#include <QSignalMapper>
#include <QPrinterInfo>
#include <QStandardItemModel>



#include "dcontroller.h"
#include "asktheuser.h"
#include "workreport.h"

namespace Ui {
    class StartDlg;
}

class StartDlg : public QDialog {
    Q_OBJECT
public:
    StartDlg(QWidget *parent = 0);
    ~StartDlg();
    void convertToPDF(QString &filename);
    void setModel (QStandardItemModel  *model);
private slots:
    void enableGUI(steps_t step,QString &message);
    void fill_docCard4Print(int Mode);

protected:
    void changeEvent(QEvent *e);
    void createConnection();
    void printToLog(QString & log_mes);
    void setPrinterList();
    void rabbitHole(); // Крольчья нора в которой сходяттся все сигналы и слоты
private:
    Ui::StartDlg  *ui;
    AskTheUser *askDlg;
    workReport  *wrkDlg;
    dController  *control;


    QSignalMapper *signalMapper;
    bool connectStep;
    bool convertStep;
    bool getMandatUserStep;
};

#endif // STARTDLG_H
