#ifndef PRINTMONITOR_H
#define PRINTMONITOR_H

#include <QWizard>

#include "mytypes.h"
#include "engine.h"

using namespace VPrn;

namespace Ui {
    class PrintMonitor;
}


class PrintMonitor : public QWizard {
    Q_OBJECT
public:
    PrintMonitor(QWidget *parent = 0);
    ~PrintMonitor();

protected:
    void changeEvent(QEvent *e);

private slots:
    void showInfoMessage(const QString &info);
private:
    Ui::PrintMonitor *ui;
    /**
      * @var app_core Основной объект программы
      */
    Engine   *core_app;
};

#endif // PRINTMONITOR_H
