#ifndef NEW_EDGE_H
#define NEW_EDGE_H

#include <QtGui/QWidget>
#include "ui_new_edge.h"

class new_edge : public QWidget
{
    Q_OBJECT

public:
    new_edge(QWidget *parent = 0);
    ~new_edge();

private:
    Ui::new_edgeClass ui;
};

#endif // NEW_EDGE_H
