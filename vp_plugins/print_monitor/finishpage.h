#ifndef FINISHPAGE_H
#define FINISHPAGE_H

#include <QWidget>

QT_FORWARD_DECLARE_CLASS ( QLabel )
QT_FORWARD_DECLARE_CLASS ( QVBoxLayout )
QT_FORWARD_DECLARE_CLASS ( QLineEdit )
QT_FORWARD_DECLARE_CLASS ( QPlainTextEdit )


class FinishPage : public QWidget
{
    Q_OBJECT
public:
    explicit FinishPage(QWidget *parent = 0);

signals:
    /**
      * @brief Ответ пользователя, утверждает он документ или нет
      */
    void saveCopiesState(bool doc_state);

public slots:
    void addInfoMsg(const QString &info);

private:
    QPlainTextEdit *myInfoEdit;

};

#endif // FINISHPAGE_H
