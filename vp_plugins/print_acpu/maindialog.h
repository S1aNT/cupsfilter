#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtGui/QWidget>

class QGridLayout;
class QFormLayout;
class QVBoxLayout;

class QLabel;
class QGroupBox;
class QComboBox;
class QPushButton;
class QCheckBox ;

class mainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit mainWidget(QWidget *parent = 0);
    ~mainWidget();

protected:
    void changeEvent(QEvent *e);

private:
    QGridLayout *gridLayout;
    QGroupBox *groupBox;
    QFormLayout *formLayout;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_login;
    QLabel *label_mandat;
    QGroupBox *groupBox_2;
    QComboBox *comboBox_printerList;
    QPushButton *pushButton_printFile;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout;
    QCheckBox *checkBox_link_surd;
    QCheckBox *checkBox_load_data;
    QCheckBox *checkBox_select_file;

};

#endif // MAINWIDGET_H
