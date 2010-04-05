#ifndef PREVIEWWND_H
#define PREVIEWWND_H
#include "viewport.h"

#include <QtGui/QWidget>
#include <QtGui/QPixmap>
#include <QtCore/QMap>

#include "mytypes.h"

using namespace VPrn;

QT_FORWARD_DECLARE_CLASS(QStringlist)
QT_FORWARD_DECLARE_CLASS(QGraphicsScene)
QT_FORWARD_DECLARE_CLASS(QSlider)

class PreviewWnd : public QWidget
{
Q_OBJECT
public:
    explicit PreviewWnd(QWidget *parent = 0);
    void loadPixmapFromFolder(const QString &folder);
signals:

public slots:

private slots:
    /**
      * @fn void zoomIn();
      * @fn void zoomOut();
      * @short Изменение масштаба на 1 ед.
      */
    void zoomIn();
    void zoomOut();

private:
    ViewPort *leftFrame;
    ViewPort *rightFrame;
    QSize thumbSize;
    PixmapList imageFullItems;
    PixmapList imageThumbItems;
    QStringList imageFilesList;

    QSlider *zoomSlider;
    //-------------------------------------------------------------------------
    QStringList findFiles(const QString& path = QString());
};

#endif // PREVIEWWND_H
