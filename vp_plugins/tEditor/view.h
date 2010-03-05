
#ifndef VIEW_H
#define VIEW_H

#include <QFrame>

QT_FORWARD_DECLARE_CLASS(QGraphicsView)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QSlider)
QT_FORWARD_DECLARE_CLASS(QToolButton)
QT_FORWARD_DECLARE_CLASS(QUndoStack)
class View : public QFrame
{
    Q_OBJECT
public:
    View(QWidget *parent = 0);
    void setAngle(qreal angle);
    QGraphicsView *gr_view() const;
    void setTabNumber(int ord){tabNumber = ord;};
    int  getTabNumber(){return tabNumber;};
    void setUndoStack(QUndoStack *stack){m_UndoStack = stack;};
    QUndoStack *undoStack() const{return m_UndoStack;};

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
    QUndoStack * m_UndoStack;

    QMatrix matrix;
    qreal curAngle;
    int tabNumber;

};

#endif
