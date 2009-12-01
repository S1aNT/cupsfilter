#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QDebug>
#include <QPluginLoader>
#include <QDir>
#include <QtGui>
#include <QMessageBox>
#include <QDesktopWidget>


#include "mediator.h"
#include "firstask.h"
#include "getusernamemandatdlg.h"
#include "tech_global.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
   void setFileToWork(QString &in_file);
   void loadPlugin(const QString &app_dir);


protected:
    void changeEvent(QEvent *e);

signals:
    void pluginLoad(const QString &message, int alignment = Qt::AlignLeft| Qt::AlignBottom, const QColor & color = QColor::fromRgb(170,255,0));
    void closeSplash();

private slots:
    void showCritError(QString e_msg); // окажем окно с сообщением об ошибке и закроем приложение
    void showAuthWindow(QString &user_name);
    void showSelectWindow();
    void cleanUp();
    void checkPluginReady();// каждый загруженный плагин докладывает о своей готовности к работе в этот слот
private:
    Ui::MainWindow *ui;
    Mediator *SpiderInTheMiddle; // Центральный элемент который знает все о всех :)
    getUserNameMandatDlg *UMDlg;
    firstAsk *askDlg;

    QString mainFileName; // Основной рабочий файл

    QPoint centerWindow;
};

#endif // MAINWINDOW_H
