#ifndef FIRSTASK_H
#define FIRSTASK_H

#include <QDialog>
#include <QSignalMapper>

namespace Ui {
    class firstAsk;
}

class firstAsk : public QDialog {
    Q_OBJECT
public:
    firstAsk(QWidget *parent = 0);
    ~firstAsk();
    int getCurrentMode(){return work_mode;};
private slots:
    void setMode (int signal_mode);
protected:
    void changeEvent(QEvent *e);

private:
    Ui::firstAsk *ui;
private:
     QSignalMapper *signalMapper;
     int work_mode;
};

#endif // FIRSTASK_H
