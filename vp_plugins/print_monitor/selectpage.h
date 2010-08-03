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

    /**
      * @fn bool enableNext();
      * @brief Разрешает преход на следующую страницу
      * если все шаги инициализации выполнены
      */
    bool enableNext();
private slots:
    void selectedPrinter(int idx);

private:
    QStandardItemModel *printer_model;
    QRadioButton * markBrakDoc;
    QRadioButton * accountingDoc;
    QRadioButton * printDoc;
    QRadioButton * both_step;
    QComboBox    * printersCBox;
};


#endif // SELECTPAGE_H
