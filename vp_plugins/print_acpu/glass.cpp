#include "glass.h"
#include <QMovie>
#include <QLabel>


Glass::Glass(QObject *parent)
    : QObject(parent)
    , m_movie(0)
    , defaultMovie(0)
    , countPerSecond(0)
{
    // Создадим сразу стекло и компоненты информационного блока. Они без
    // родительского объекта, потому что во время установки стекла поверх виджета
    // будут изменяться предки.
    glass = new QLabel();
    animationContainer = new QLabel();
    infoTextContaiter = new QLabel();

    // Загрузим анимацию по умолчанию
    defaultMovie = new QMovie("anim.mng", QByteArray(), animationContainer);
}

// Так как мы не задали родителей для компонент стекла, необходимо
// самостоятельно позаботиться об освобождении памяти
Glass::~Glass()
{
    glass->deleteLater();
    animationContainer->deleteLater();
    infoTextContaiter->deleteLater();
    defaultMovie->deleteLater();
}

// Теперь реализуем один из самых важных методов. Установку стекла поверх
// виджета
void Glass::install(QWidget* widget)
{
    // Для начала удалим его с предыдущего виджета
    remove();

    // Установим стекло поверх виджета
    glass->setParent(widget);

    // Начнем отлавливать все события, установив фильтр  widget->installEventFilter(this);

    // Покажем стекло и информационный блок, если это необходимо
    glass->show();
    if (infoTextBlockEnabled){
        showInfoTextBlock();
    }
    if (animationBlockEnabled){
        showAnimationBlock();
    }
    // Пошлем событие сами себе, чтобы стекло и информационный блок правильно
    // разместились на виджете
    QEvent event(QEvent::Resize);
    eventFilter(0, &event);
}

// Удаление виджета противоположно установке
void Glass::remove()
{
    // Если стекло было установлено, то удаляем его
    if (glass->parentWidget() != 0) {
        // Перестаем отлавливать события на низлежащем виджете
        glass->parentWidget()->removeEventFilter(this);

        // Скрываем все компоненты стекла
        glass->hide();
        glass->setParent(0);
        animationContainer->hide();
        infoTextContaiter->hide();
        animationContainer->setParent(0);
        infoTextContaiter->setParent(0);
    }
}

void Glass::enableInfoTextBlock(const QString& text)
{
    if ( !text.isEmpty()) {
      infoTextContaiter->setText (text);
    }

}

void Glass::disableInfoTextBlock()
{
    infoTextContaiter->clear ();
}

QLabel* Glass::getInfoTextBlock()
{
    return infoTextContaiter;
}

//---------------------------- Protected ---------------------------------------
// Реализуем фильтр событий на виджете. Назначение этого фильтра - не
// допустить работу с виджетом под стеклом и обеспечить актуальные размеры
// и перерисовку стекла
bool Glass::eventFilter(QObject* /* object */, QEvent* event)
{
    // Если показывается виджет или изменились его размеры, изменяем размеры
    // стекла и позиционируем информационный блок
    if ((event->type() == QEvent::Show) || (event->type() == QEvent::Resize)) {
        glass->resize(glass->parentWidget()->size());
        glass->move(0, 0);
        infoBlockPositioning();
        return true;
    }
    // Всегда отбираем фокус, чтобы запретить работу с виджетом под стеклом
    glass->setFocus();
    return false;
}

//------------------------------ Private ---------------------------------------
// Реализуем позиционирование информационного блока. Он будет размещаться по
// центру стекла. Под анимацией будем отображать пояснительный текст.
void Glass::infoBlockPositioning()
{
    if (animationContainer->isVisible() && infoTextContaiter->isVisible()) {
        animationContainer->move((glass->width() - animationContainer->width()) / 2,
                                 glass->height() / 2 - animationContainer->height());
        infoTextContaiter->move((glass->width() - infoTextContaiter->width()) / 2,
                                glass->height() / 2 + infoTextContaiter->height());
    } else {
        if (animationContainer->isVisible())
            animationContainer->move((glass->width() - animationContainer->width()) / 2,
                                     (glass->height() - animationContainer->height()) / 2);

        if (infoTextContaiter->isVisible())
            infoTextContaiter->move((glass->width() - infoTextContaiter->width()) / 2,
                                    (glass->height() - infoTextContaiter->height()) / 2);
    }
}

// Реализуем методы показа информационного блока
void Glass::showInfoTextBlock(const QString& text)
{
    // Устанавливаем пояснительный текст поверх стекла и накрытого виджета. Здесь
    // используется особенность размещения графических компонентов Qt: последний
    // добавленный графический компонент будет выше всех остальных
    infoTextContaiter->setParent(glass->parentWidget());
    if (!text.isNull()){
        infoTextContaiter->setText(text);
    }
    infoTextContaiter->show();
    infoTextContaiter->adjustSize();
    // Корректируем позицию
    infoBlockPositioning();
}

void Glass::showAnimationBlock(QMovie* movie)
{
    // Устанавливаем анимационный контейнер поверх стекла и накрытого виджета
    animationContainer->setParent(glass->parentWidget());
    if (movie != 0){
        setMovie(movie);
    }
    animationContainer->show();
    infoBlockPositioning();
}

void Glass::setMovie(QMovie* movie)
{
    m_movie = movie;
}
