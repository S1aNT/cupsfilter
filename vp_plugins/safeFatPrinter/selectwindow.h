#ifndef SELECTWINDOW_H
#define SELECTWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QSignalMapper>


#include "mediator.h"

#include "getusernamemandatdlg.h"
#include "workfield.h"
#include "tech_global.h"

namespace Ui {
    class SelectWindow;
}

class SelectWindow : public QMainWindow {
    Q_OBJECT
public:
    SelectWindow(QWidget *parent = 0);
    ~SelectWindow();
    void setFileToWork(QString &in_file);
    void loadPlugin(const QString &app_dir);
    //int getCurrentMode(){return work_mode;};

signals:
    void pluginMessage(const QString &message);

private slots:
    void setMode (int signal_mode);
    void enableGUI();
protected:
    void changeEvent(QEvent *e);
    QPoint SelectWindow::calcCenter();

private slots:
    void showCritError(QString e_msg); // окажем окно с сообщением об ошибке и закроем приложение
    void showAuthWindow(QString &user_name);
    void showPluginMessage(const QString &message);
private:
    Ui::SelectWindow *ui;
    QString mainFileName; // Основной рабочий файл

    Mediator *SpiderInTheMiddle; // Центральный элемент который знает все о всех :)
    getUserNameMandatDlg *UMDlg;
    workField *WorkDlg;

    QSignalMapper *signalMapper;
    int work_mode;
};

#endif // SELECTWINDOW_H
