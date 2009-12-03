#ifndef WORKFIELD_H
#define WORKFIELD_H

#include <QDialog>
#include <QMessageBox>
#include <QStringListModel>
#include <QStandardItemModel>

#include <QSignalMapper>
#include <QDataWidgetMapper>


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

};

#endif // WORKFIELD_H
