#ifndef STARTDLG_H
#define STARTDLG_H

#include <QtGui/QDialog>
#include <QMessageBox>
#include <QProcess>
#include <QtDebug>
#include <QUuid>
#include <QPrinterInfo>
#include <QFile>
#include <QSettings>
#include <QCloseEvent>

#include "proc_thread.h"
#include "netAgent.h"
#include "asktheuser.h"

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

    void convertToPDF(const QString &input_file);
    void closeEvent(QCloseEvent *event);
    void makeConnection();
    void init();
    void setDocName(const QString &docName);
    void setMandat(const QString &userName,const QString &mandatName);
public slots:
    void processDone(int Code, const QString &s_output);
    void mergePDF(QString &main_file,QString &background_file);
    void markPaper();
    
private slots:

    /**
     *  ExecuteNextStep  after the convert to PDF process is finished.
     * @param exitCode The exit code of the process.
     */

    void displayDebug(const QString &line);
private:
    Ui::StartDlg *ui;


    ProcessT proc;
    QString dTitle;
    QString mandat;
    QString currentUserName;
    QUuid SID;
    QStringList args;
    QString cmd;
    QString mainPDF;	 // Файл который получился в результате конвертирования исходного
    QString gsBin;	// путь к исполняемому файлу ghostscript
    QString pdftkBin;	// Путь к файлу pdfTK
    QString serverHostName;

    netAgent n_ag; // сетевой агент

    int serverPort;
    void read_settings();
    void write_settings();
};

#endif // STARTDLG_H
