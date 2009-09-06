#ifndef STARTDLG_H
#define STARTDLG_H

#include <QtGui/QDialog>
#include <QMessageBox>
#include <QProcess>
#include <QtDebug>
#include <QUuid>

#include "proc_thread.h"


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
    void setDocTitle(QString &doc_title);
    void convertToPDF(QSring &input_file);

public slots:
    void processDone(int Code, QProcess::ExitStatus Status);
    void mergePDF(QString &main_file,QString &background_file);
private slots:

    /**
     *  ExecuteNextStep  after the convert to PDF process is finished.
     * @param exitCode The exit code of the process.
     */
    void executeNextStep( const QString &pdf_FileName );
private:
    Ui::StartDlg *ui;

    ProcessT proc;
    QString dTitle;
    QUuid SID;
    QString mainPDF;  // Файл который получился в результате конвертирования исходного
};

#endif // STARTDLG_H
