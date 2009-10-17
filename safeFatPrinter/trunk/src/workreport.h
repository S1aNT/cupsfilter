#ifndef WORKREPORT_H
#define WORKREPORT_H
#include <QDialog>
#include <QDebug>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
class QCheckBox;
class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QPlainTextEdit;
class QLineEdit;
class QRadioButton;
QT_END_NAMESPACE

class workReport : public QDialog
{
    Q_OBJECT

public:
    workReport(QWidget *parent = 0);
private slots:
    void saveResultToBase();
    void sendErrorReport(bool state);
private:
    QLabel * label;
    QLabel * label_stamp;
    QLabel * label_printer;
    QLabel * label_num;
    QLabel * label_page;
    QLabel * label_brakPage;

    QPlainTextEdit *DocEdit;
    QPlainTextEdit *brakDesEdit;
    QLineEdit * lineEdit;
    QLineEdit * printerEdit;
    QLineEdit * pageEdit;
    QLineEdit * numEdit;
    QLineEdit * brakEdit;

    QRadioButton *singlePageRB;
    QRadioButton *allDocRB;
    QPushButton *acceptButton;
    QPushButton *moreButton;
    QWidget *extension;
    bool saveResult; // if false save  describe error else save result
};

#endif // WORKREPORT_H
