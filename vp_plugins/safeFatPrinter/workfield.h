#ifndef WORKFIELD_H
#define WORKFIELD_H

#include <QDialog>
#include <QPixmap>
#include <QMessageBox>
#include <QStringListModel>
#include <QStandardItemModel>

#include <QSignalMapper>
#include <QDataWidgetMapper>

#include "previewwnd.h"

namespace Ui {
    class workField;
}

class workField : public QDialog{

    Q_OBJECT
public:
    workField(QWidget *parent = 0);
    ~workField();
    void setPagesCount(int p_count);
    void setStampModel(QStringListModel *stamp_model);
    void setModel (QStandardItemModel * model);
    void setMode (int mode_value);
signals:
    void checkMBInBase(const QString &mb_value,const QString &copyNum_value,int work_mode);
    void needAuthUserToPrinter();
    void needPrintPage();
    void dontNeedPrintPage();
public slots:
    void showInfoWindow(const QString &info);
    void showPreviewPage(QPixmap &preview_page);
private slots:
    void flipLabel(bool flip);
    void checkData(); // При нажатии на кнопку идет анализ введенных данных
protected:
    void changeEvent(QEvent *e);
private:
    Ui::workField *ui;
    QDataWidgetMapper *mapper;
    QSignalMapper *signalMapper;

    int mode;
    void isAnyFiedEmpty();
    void setEnableField(bool e);

};

#endif // WORKFIELD_H
