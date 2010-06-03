#ifndef VIEWPORT_H
#define VIEWPORT_H
#include <QtGui/QFrame>
#include <QtGui/QResizeEvent>
#include <QtCore/QMap>

QT_FORWARD_DECLARE_CLASS(QGraphicsView)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QSlider)
QT_FORWARD_DECLARE_CLASS(QToolButton)
QT_FORWARD_DECLARE_CLASS(QUndoStack)
QT_FORWARD_DECLARE_CLASS(QGraphicsScene)

#include "mytypes.h"
using namespace VPrn;



class ViewPort : public QFrame
{
    Q_OBJECT
public:
    ViewPort (QWidget *parent = 0);
    /**
      * @fn void showThumbNail(bool thumb,QSize t_size);
      * @brief Настрока режима отображения списка картинок
      * @param thumb;(true/false) Вкл./выкл режим "наготков"
      * @param t_size; Задать размеры наготков
      */
    void showThumbNail(bool thumb,QSize t_size);
    /**
      * @fn void setPixMapList(PixmapList pList);
      * @brief Задает список картинок для отображения в виде <int,QPixmap>
      */
    void setPixMapList(PixmapList pList);

    void setDescList(PixmapDescList descImagesList);
signals:
    /**
      * @fn void selectPixmap(int p_number);
      * @brief Сигнал несет номер выбраного "наготка"
      */
    void selectPixmap(int p_number);
public slots:
    void showPage(int p_number);

    /**
      * @fn void setupMatrix(int zoom);
      * @brief Настройка масштаба изображения
      */
    void setupMatrix(int zoom);

    void zoomToFullSize();
    void zoomToFit();

private slots:
    /**
      * @fn void onSceneSecletionChnaged();
      * @brief Как только на сцене выделен элемент, обрабатываю эту ситуацию:
      * - если вкл. режим "Наготков", то определяю какой элемент выделен
      * и сообщаю навер его порядковый номер. иначе ничего не делаю
      */
    void onSceneSecletionChnaged();

private:
    int curPixmapNumber;
    QGraphicsView *graphicsView;
    bool m_thumb;
    QGraphicsScene *scene;
    QSize thumb_size;
    PixmapList m_pixmap_list;
    PixmapDescList m_desc_list;

    //-------------------------------------------------------------------------
    /**
      * @fn qreal addPairItem (qreal xPos,qreal yPos,int p_number,bool selected);
      * @brief Рисует на холсте пару элементов картинку и подпись под ней
      * @param qreal xPos    Позиция картинки по горизонтали
      * @param qreal yPos    Позиция картинки по вертикали
      * @param int p_number  Номер картинки в списке
      * @param bool selected Выделена картинка или нет
      * @return qreal yPos   Возвращает позицию за нижмин краем последней отрисованной картинки
      */
      qreal addPairItem (qreal xPos,qreal yPos,int p_number,bool selected);

};

#endif // VIEWPORT_H
