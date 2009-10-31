#ifndef STARTDLG_H
#define STARTDLG_H

#include <QtGui/QDialog>
#include <QDateTime>
#include <QSignalMapper>
#include <QPrinterInfo>

#include "dController.h"
#include "askTheUser.h"
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
private slots:
    void enableGUI(int step,QString &message);
    void fill_docCard4Print(int Mode);
protected:
    void changeEvent(QEvent *e);
    void createConnection();
    void printToLog(QString & log_mes);
    void setPrinterList();
private:
    Ui::StartDlg  *ui;
    AskTheUser *askDlg;
    workReport  *wrkDlg;
    dController  *control;

    QSignalMapper *signalMapper;
    bool connectStep;
    bool convertStep;
};

#endif // STARTDLG_H
