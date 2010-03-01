#ifndef UNDOFRAME_H
#define UNDOFRAME_H

#include <QDockWidget>

class QVBoxLayout;
class QHBoxLayout;
class QSpacerItem;
class QLabel;
class QSpinBox;
class QUndoView;
class QUndoGroup;

class UndoFrame : public QDockWidget {
    Q_OBJECT
public:
    UndoFrame(QWidget *parent = 0);
    void setUndoViewGroup(QUndoGroup * undoGroup);
protected:
    void changeEvent(QEvent *e);
signals:
    void undoLimitChange(int limit);
private:
    QWidget     *dockWidgetContents;
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QLabel      *label;
    QSpinBox    *undoLimit;
    QUndoView   *undoView;


    void retranslateUi();

};

#endif // UNDOFRAME_H
