#ifndef SELECTPAGE_H
#define SELECTPAGE_H


#include <QtGui/QWidget>
#include <QtCore/QMap>

QT_FORWARD_DECLARE_CLASS ( QRadioButton )
QT_FORWARD_DECLARE_CLASS ( QCheckBox )
QT_FORWARD_DECLARE_CLASS ( QComboBox )
QT_FORWARD_DECLARE_CLASS ( QLabel )
QT_FORWARD_DECLARE_CLASS ( QStandardItemModel )

class SelectPage : public QWidget
{
    Q_OBJECT
public:
    explicit SelectPage(QWidget *parent = 0);
    void setPrintersModel(QStandardItemModel *p_model );

    int selectPrinterId(){return printer_id;}
private slots:
    void setCurrentPrinter(int p_id){printer_id = p_id;}
private:
    int printer_id;    
    QRadioButton * markBrakDoc;
    QRadioButton * accountingDoc;
    QRadioButton * printDoc;
    QRadioButton * both_step;
    QComboBox    * printersCBox;
};


#endif // SELECTPAGE_H
