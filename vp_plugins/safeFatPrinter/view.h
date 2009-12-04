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
#include <QStandardItemModel>
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

class View : public QDialog {
    Q_OBJECT
public:
    View(QWidget *parent = 0);
    ~View();
    void setModel (QStandardItemModel  *model);
    void setTemplatesList(QStringList & t_list);
    bool get_pageOrient();
    void set_pageOrient(bool orient);
public slots:
    void setPaperSize(const QString & text);//Возникает когда юзер выберает новый размер листа
    void flipPage(int Btn);
    void readFromStream(QString template_name);
    void saveToStream();
private slots:

    void addElemToPage();
    void setupMatrix();
    void print();

    void drawElement2Scene(int pageIndex);

    void zoomIn();
    void zoomOut();
protected:
    void changeEvent(QEvent *e);
    void createScene();
    void drawPage();
    void flipElem(qreal angle);
    void init();
    QStringList getPageSizeList();
    QGraphicsScene *getCurrentScene(int page);
    void setDataToTemplates();
private:
    Ui::View *m_ui;

    QGraphicsView *graphicsView;
    QGraphicsView *graphicsView1;
    QGraphicsView *graphicsView2;
    QGraphicsView *graphicsView3;
    //QGraphicsScene *scene;
    QLabel *label;
    QLabel *label_page;
    QToolButton *printButton;
    QToolButton *addElemButton;
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
    qreal page_angle;

    QStandardItemModel *work_model;

    QGraphicsScene *first_scene;
    QGraphicsScene *face_scene;
    QGraphicsScene *over_scene;
    QGraphicsScene *last_scene;

    QList <SimpleItem *> last_pageList;
    int currentPage;
    QString fileTemplName;
};

#endif // VIEW_H
