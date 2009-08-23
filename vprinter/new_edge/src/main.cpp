#include "new_edge.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    new_edge w;
    w.show();
    return a.exec();
}
