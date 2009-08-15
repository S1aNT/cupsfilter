#ifndef DOCVIEW_H
#define DOCVIEW_H

#include <QtGui/QFrame>

QT_FORWARD_DECLARE_CLASS(QGraphicsView)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QSlider)
QT_FORWARD_DECLARE_CLASS(QToolButton)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QComboBox)

namespace Ui {
    class DocView;
}

class DocView : public QFrame {
    Q_OBJECT
public:
    DocView(const QString &name, QWidget *parent = 0);
    ~DocView();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DocView *m_ui;
    QGraphicsView *graphicsView;
    QLabel *label;
    QToolButton *openGlButton;
    QToolButton *antialiasButton;
    QToolButton *printButton;
    QToolButton *resetButton;
    QPushButton *portretButton;
    QPushButton *landscapeButton;
    QSlider *zoomSlider;
    QSlider *rotateSlider;
    QComboBox *template_cbox;

};

#endif // DOCVIEW_H
