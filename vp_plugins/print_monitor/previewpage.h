#ifndef PREVIEWPAGE_H
#define PREVIEWPAGE_H

#include "mytypes.h"
#include "viewport.h"

#include <QtGui/QWizardPage>
#include <QtGui/QPixmap>
#include <QtGui/QSlider>

#include <QtCore/QMap>

using namespace VPrn;



QT_FORWARD_DECLARE_CLASS ( QLabel )
QT_FORWARD_DECLARE_CLASS ( QCheckBox )

class PreViewPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit PreViewPage(QWidget *parent = 0);
    /**
     * @fn bool validatePage();
     * @brief Данная функция проверяет корректность заполнения полей
     * проверка последней минутки
     */
   bool validatePage ();

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
    QStringList imageFilesList;

    QSlider *zoomSlider;
};

#endif // PREVIEWPAGE_H
