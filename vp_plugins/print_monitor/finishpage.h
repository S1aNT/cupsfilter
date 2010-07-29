#ifndef FINISHPAGE_H
#define FINISHPAGE_H

#include <QWidget>

QT_FORWARD_DECLARE_CLASS ( QLabel )
QT_FORWARD_DECLARE_CLASS ( QGridLayout )
QT_FORWARD_DECLARE_CLASS ( QLineEdit )
QT_FORWARD_DECLARE_CLASS ( QPlainTextEdit )
QT_FORWARD_DECLARE_CLASS ( QGroupBox )

class FinishPage : public QWidget
{
    Q_OBJECT
public:
    explicit FinishPage(QWidget *parent = 0);

signals:

public slots:
private:
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *stampLab;
    QLabel *mbLab;
    QLabel *numberLab;
    QLabel *pageCountLab;
    QLineEdit *pageCountLE;
    QLineEdit *stampLE;
    QLineEdit *mbLE;
    QLineEdit *numberLE;
    QLabel *docNameLab;
    QLineEdit *printerLE;
    QLabel *printerLab;
    QPlainTextEdit *docNamePE;
    QLabel *executorLab;
    QLineEdit *executorLE;
    QLabel *printManLab;
    QLineEdit *printManLE;
    QLabel *printDateLab;
    QLineEdit *printDateLE;
};

#endif // FINISHPAGE_H
