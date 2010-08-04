#ifndef PREVIEWPAGE_H
#define PREVIEWPAGE_H

#include "viewport.h"

#include <QtGui/QWidget>
#include <QtGui/QPixmap>
#include <QtGui/QSlider>

#include <QtCore/QMap>

QT_FORWARD_DECLARE_CLASS ( QLabel )
QT_FORWARD_DECLARE_CLASS ( QCheckBox )

class PreViewPage : public QWidget
{
    Q_OBJECT
public:
    explicit PreViewPage(QWidget *parent = 0);
        void needRestart();

   bool enableNext();

public slots:
    void showPicturesList(QStringList png_list);
signals:
    void printCurrentDoc();    
private slots:
    /**
      * @fn void zoomIn();
      * @fn void zoomOut();
      * @short Изменение масштаба на 1 ед.
      */
    void zoomIn();
    void zoomOut();

private:
    QCheckBox *checkPicturesList;
    ViewPort *leftFrame;
    ViewPort *rightFrame;
    QSize thumbSize;
    QLabel *topLabel;
    PixmapList imageFullItems;
    PixmapList imageThumbItems;
    PixmapDescList descImagesList;
    QStringList imageFilesList;

    QSlider *zoomSlider;
};

#endif // PREVIEWPAGE_H
