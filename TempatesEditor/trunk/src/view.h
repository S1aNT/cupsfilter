#ifndef VIEW_H
#define VIEW_H

#include <QtGui/QFrame>
#include <QMap>
#include <QDebug>
#include <QButtonGroup>
#include <qmath.h>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QToolButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPrinter>
#include <QPrintDialog>
#include <QRadioButton>
#include <QComboBox>
#include <QFile>
#include <QDataStream>
//#include <QPrintPreviewDialog>
#include <QMapIterator>
#include "simpleitem.h"

QT_FORWARD_DECLARE_CLASS(QGraphicsView)
QT_FORWARD_DECLARE_CLASS(QGraphicsScene)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QSlider)
QT_FORWARD_DECLARE_CLASS(QToolButton)
QT_FORWARD_DECLARE_CLASS(QRadioButton)
QT_FORWARD_DECLARE_CLASS(QComboBox)

namespace Ui {
    class View;
}

class View : public QFrame {
    Q_OBJECT
public:
    View(QWidget *parent = 0);
    ~View();
    void setTemplatesList(QStringList & t_list);
    bool get_pageOrient();
    void set_pageOrient(bool orient);
public slots:
    void setPaperSize(const QString & text);//Возникает когда юзер выберает новый размер листа
    void flipPage(int Btn);
    void readFromStream(QString template_name);
    void saveToStream();
private slots:
    void setupMatrix();
    void print();

    void zoomIn();
    void zoomOut();
protected:
    void changeEvent(QEvent *e);
    void createScene();
    void drawPage();
    void init();
    QStringList getPageSizeList();
private:
    Ui::View *m_ui;

    QGraphicsView *graphicsView;
    QGraphicsScene *scene;
    QLabel *label;
    QLabel *label_page;
    QToolButton *printButton;
    QToolButton *resetButton;
    QToolButton *addTmplButton;
    QToolButton *delTmplButton;
    QToolButton *saveTmplButton;
    QToolButton *firstPageButton;
    QToolButton *secondPageButton;
    QToolButton *portretButton;
    QToolButton *landscapeButton;
    QSlider *zoomSlider;
    QRadioButton *sys_button ;
    QRadioButton *user_button ;
    QComboBox	*templatesCBox;
    QComboBox	*pageSizeCBox;
    QMap<QString, int> page_size;
    int page_height;
    int page_width;
    int margin_top,margin_bottom,margin_left,margin_right;
    bool page_orient;

    QList <SimpleItem *> elemList;

};

#endif // VIEW_H
