#ifndef CHECKDATAPAGE_H
#define CHECKDATAPAGE_H

#include <QtGui/QWidget>

/**
  * @class Данный клас реализует страницу проверки введенных пользователем данных
  * Cписок проверок:
  * @line
  * @li Авторизация пользователя на устройство, запускается в момент нажатия
  *     юзером кнопки Далее, на странице ввода данных
  * @li Проверка существования документа с МБ ххх и Номером экземпляра 1-5
  *     если документ существует, то кнопка Далее ведет на страницу Браковки документа
  * @li Проверка правильности заполнения пользователем полей данных,
  *     если поле отпечатал не заполненно, туда вноситься значение поля Исполнитель
  *     если выбран режим Печать всех экз., то должны быть заполненны все поля адресатов
  *     если выбран конкретный экз.,то соответветсвенно такое же поле адресат
  *     должно быть заполненно. (Проверка происходит на странице ввода данных)
  *     Если все проверки прошли успешно, и юзер выбрал один режимов предпросмотра
  *     Только значащие страницы шаблона/весь документ. Становиться активной кнопка
  *     Далее, которая запускает иницирует отправку документа на объединение с шаблоном,
  *     и показывает окно предпросмотра.
  */
QT_FORWARD_DECLARE_CLASS ( QRadioButton )
QT_FORWARD_DECLARE_CLASS ( QCheckBox )
QT_FORWARD_DECLARE_CLASS ( QLabel )
QT_FORWARD_DECLARE_CLASS ( QButtonGroup )


class CheckDataPage : public QWidget
{
    Q_OBJECT
public:
    explicit CheckDataPage(QWidget *parent = 0);
signals:
    void select_preview(bool full);
    void select_print  ();

public slots:
    void setAuthCheck( bool flag, const QString & info );
    void setMbCheck  ( bool flag, const QString & info );
    void setCheckMergeDocWithTemplates( bool flag, const QString & info );
private slots:
    /**
      *@fn void startMergedDoc(bool status);
      * @brief Пользователь выбрал режим предпросмотра, документ имеет корректный
      * МБ, доступ к принтеру разрешен, запуск процедуры формирования документа для предпросмотра
      */
    void startMergedDoc4PreviewFull(bool status);
    void startMergedDoc4PreviewPart(bool status);
    void startMergedDoc4Print (bool status);
private:
    /**
      * @var topLabel;           Заголовок
      * @var reviewAllPages;     Выбор предпросмотр всех страниц
      * @var previewPartPages;   Выбор предпросмотр значащих страниц на которые наложен шаблон
      * @var authUserToPrinter;  Проверка юзера на право печати на устройстве
      * @var checkCorrectMB;     Проверка МБ и №экз по базе.
      * @var checkMergeDocWithTemplates; Автом. проверка статуса наложения шаблона на документ
      */
    QLabel       * topLabel;
    QRadioButton * previewAllPages;
    QRadioButton * previewPartPages;
    QRadioButton * printWithoutPreview;
    QCheckBox    * authUserToPrinter;
    QCheckBox    * checkCorrectMB;
    QCheckBox    * checkMergeDocWithTemplates;
    QString authInfo;
    QString mb_checkInfo;
    QButtonGroup *rb_group;

    void enablePreviewButton(bool mode);
};

#endif // CHECKDATAPAGE_H
