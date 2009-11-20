#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QPluginLoader>
#include <QDir>
#include <QtGui>
#include <QMessageBox>


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
private slots:
    void getUserName();

private:
    Ui::MainWindow *ui;



    QString userName;

    QLineEdit *lineEdit;
    QLabel *label;
    QPushButton *button;
    QGridLayout *layout;


};

#endif // MAINWINDOW_H
