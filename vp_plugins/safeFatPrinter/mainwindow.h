#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QDebug>
#include <QPluginLoader>
#include <QDir>
#include <QtGui>
#include <QMessageBox>
#include <QFileDialog>

#include "mediator.h"
#include "tech_global.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setSpider(Mediator *spider);
protected:
    void changeEvent(QEvent *e);

public slots:
    //void updateStatusBar(plugin_state_t state);
    void showAuthWindow(QString &);
    void enableGUI();
private slots:
    void getUserName();
    void showError(QString &info);

private:
    Ui::MainWindow *ui;
    Mediator *spiderInTheMiddle; // Центральный элемент который знает все о всех :)

    QString userName;
    QLineEdit *lineEdit;
    QLabel *label;
    QPushButton *button;
    QGridLayout *layout;
    int Mode;
};

#endif // MAINWINDOW_H
