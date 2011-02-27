#ifndef GLASS_H
#define GLASS_H

#include <QObject>
class QColor;
class QMovie;
class QLabel;

class Glass : public QObject
{
    Q_OBJECT
public:
    explicit Glass(QObject *parent = 0);
    virtual ~Glass();

    // Объявим методы установки и снятия стекла с виджета:
    virtual void install(QWidget* widget);
    virtual void remove();
    //Добавим методы управления цветом и прозрачностью:
    void enableColor(const QColor& color = QColor(111, 111, 100));
    void disableColor();

    void enableOpacity(qreal opacity = 0.5);
    void disableOpacity();
    /**
      * Информационный блок будет составным. Состоит он из двух объектов QLabel.
      * Если на писать и рисовать на самом стекле, то текст и графика будут принимать
      * цвет стекла или становиться прозрачными.
      */
    void enableInfoBlock(QMovie* movie = 0, const QString& text = QString::null);
    void disableInfoBlock();

    void enableAnimationBlock(QMovie* movie = 0);
    void disableAnimationBlock();

    void enableInfoTextBlock(const QString& text = QString::null);
    void disableInfoTextBlock();
    QLabel* getInfoTextBlock();

    void setMovie(QMovie* movie);

protected:
    /**
      * Для того, чтобы блокировать любые действия с виджетом, изменять размеры
      * стекла, поверх которого помещено стекло, определим фильтр событий.
      * Его мы будем устанавливать в блокируемый виджет.
      */

    bool eventFilter(QObject* object, QEvent* event);
private:
    /**
      * Добавим методы показа отдельных частей информационного блока и его
      * позиционирования
      */

    void showInfoTextBlock(const QString& text = QString::null);
    void showAnimationBlock(QMovie* movie = 0);
    void infoBlockPositioning();

    //Поля компонент стекла:
    QLabel* glass;
    QLabel* animationContainer;
    QMovie* m_movie;
    QMovie* defaultMovie;
    QLabel* infoTextContaiter;
    QColor glassColor;
    int countPerSecond;
    /**
      * Флаги, по которым при повторной установке стекла будем определять
      * показывать ли информационный блок:
      */
    bool infoTextBlockEnabled;
    bool animationBlockEnabled;
};

#endif // GLASS_H

