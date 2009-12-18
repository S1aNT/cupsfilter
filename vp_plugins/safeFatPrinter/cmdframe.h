#ifndef CMDFRAME_H
#define CMDFRAME_H

#include <QFrame>

namespace Ui {
    class cmdFrame;
}

QT_BEGIN_NAMESPACE
class QVBoxLayout;
class QPushButton;
class QSignalMapper;
QT_END_NAMESPACE
class cmdFrame : public QFrame {
    Q_OBJECT
public:
    cmdFrame(QWidget *parent = 0);
    ~cmdFrame();
    void setCmdButton(QStringList c_list);
    void retranslateUi(QFrame *cmdFrame);
protected:
    void changeEvent(QEvent *e);
signals:
    void clicked(const QString &);
    void test_signal();
private slots:
    void test();
private:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *addImgBtn;
    QPushButton *pushButton_2;
    QSignalMapper *signalMapper;
};

#endif // CMDFRAME_H
