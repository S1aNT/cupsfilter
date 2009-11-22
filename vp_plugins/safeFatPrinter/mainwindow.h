#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QPluginLoader>
#include <QDir>
#include <QtGui>
#include <QMessageBox>
#include <QErrorMessage>

#include "mediator.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

public slots:
    void showInfo(QString info);
    void updateStatusBar(Mediator::PluginState state);
private slots:
    void getUserName();
    void showError(QString &info);

private:
    Ui::MainWindow *ui;
    Mediator *SpiderInTheMiddle; // Центральный элемент который знает все о всех :)
    QErrorMessage *errorMessageDialog;


    QString userName;

    QLineEdit *lineEdit;
    QLabel *label;
    QPushButton *button;
    QGridLayout *layout;


};

#endif // MAINWINDOW_H
