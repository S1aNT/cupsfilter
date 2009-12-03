#ifndef PREVIEWWND_H
#define PREVIEWWND_H

#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QLabel>


namespace Ui {
    class PreViewWnd;
}

class PreViewWnd : public QWidget {
    Q_OBJECT
public:
    PreViewWnd(QWidget *parent = 0);
    ~PreViewWnd();
    void setPrinter(QPrinter &printer);
signals:
    void needPrintPage();
    void dontNeedPrintPage();
public slots:
    void showPage(const QPixmap &page);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::PreViewWnd *ui;
    QLabel img_lab;

};

#endif // PREVIEWWND_H
