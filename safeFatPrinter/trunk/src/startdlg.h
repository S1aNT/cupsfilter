#ifndef STARTDLG_H
#define STARTDLG_H

#include <QtGui/QDialog>
#include <QMessageBox>
#include <QProcess>
#include <QDateTime>
#include <QPrinterInfo>
#include "asktheuser.h"
#include "workreport.h"
#include "dController.h"

namespace Ui
{
    class StartDlg;
}


class StartDlg : public QDialog
{
    Q_OBJECT

public:
    StartDlg(QWidget *parent = 0);
    ~StartDlg();
    void convertToPDF(QString &in_file);
    void setController(dController *ctrl);

signals:
    void printerSelected (QString ptr);
public slots:
     void error(int ErrorCode,QString ErrorString);
     void convertDone();
     void mergeDone();
     void connectToDemon();
private slots:
     void markPaper();
    void printOnMarkPaper();
private:
    void enableGUI();
    void setPrinterList();


    Ui::StartDlg *ui;
    dController *d_ctrl;

    askTheUser askDlg;

    workReport reportDlg;
    bool cnv;
    bool merge;
    bool conn_demon;
};

#endif // STARTDLG_H
