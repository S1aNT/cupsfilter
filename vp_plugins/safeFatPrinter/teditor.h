#ifndef TEDITOR_H
#define TEDITOR_H

#include <QDialog>
#include <QGraphicsScene>
#include <QSignalMapper>

#include "view.h"

namespace Ui {
    class TEditor;
}

class TEditor : public QDialog {
    Q_OBJECT
public:
    TEditor(QWidget *parent = 0);
    ~TEditor();
public slots:
    // Установка сцен
    void setScene(QGraphicsScene *scene_1,QGraphicsScene *scene_2,
                  QGraphicsScene *scene_3,QGraphicsScene *scene_4);
signals:
    void addBaseElementToPage(int page);
    void saveTemplates();
private slots:

protected:
    void changeEvent(QEvent *e);

private:
    Ui::TEditor *ui;

    QSignalMapper *signalMapper;
    View *view_1;
    View *view_2;
    View *view_3;
    View *view_4;
    QString curTemplFile;
};

#endif // TEDITOR_H
