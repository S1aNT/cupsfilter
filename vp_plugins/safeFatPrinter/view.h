
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
    View(const QString &name, QWidget *parent = 0);

    QGraphicsView *gr_view() const;
    //void setPageNum(int p_num){pageNum = p_num;};
private slots:
    void addElemToPage(); // Добавим базовый элемент на страницу
    void resetView();
    void setResetButtonEnabled();
    void setupMatrix();
    void toggleAntialiasing();
    void print();

    void zoomIn();
    void zoomOut();

signals:
    // Требование плагину добавить элемент на страницу
    void addBaseElementToPage();
private:
    QGraphicsView *graphicsView;
    QLabel *label;

    QToolButton *antialiasButton;
    QToolButton *printButton;
    QToolButton *resetButton;
    QToolButton *addElemButton;
    QSlider *zoomSlider;

    //int PageNum;
};

#endif
