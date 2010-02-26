
#ifndef VIEW_H
#define VIEW_H

#include <QFrame>

QT_FORWARD_DECLARE_CLASS(QGraphicsView)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QSlider)
QT_FORWARD_DECLARE_CLASS(QToolButton)

class View : public QFrame
{
    Q_OBJECT
public:
    View(QWidget *parent = 0);
    inline void setAngle(qreal angle){curAngle = angle;};
    QGraphicsView *gr_view() const;
    void setTabOrder(int ord){tabOrder = ord;};
    int getTabOrder(){return tabOrder;};
public slots:
    //void flipPage(int Btn);

private slots:
    void resetView();
    void setResetButtonEnabled();
    void setupMatrix();
    void toggleAntialiasing();
    void zoomIn();
    void zoomOut();
private:
    QGraphicsView *graphicsView;
    QLabel *label;
    QSlider *zoomSlider;

    QMatrix matrix;
    qreal curAngle;
    int tabOrder;
};

#endif
