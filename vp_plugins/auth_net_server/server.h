#ifndef SERVER_H
#define SERVER_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QLocalServer;
QT_END_NAMESPACE

class Server : public QDialog
{
    Q_OBJECT

public:
    Server(QWidget *parent = 0);

private slots:
    void sendFortune();

private:
    QLabel *statusLabel;
    QPushButton *quitButton;
    QLocalServer *server;
    QStringList fortunes;
};

#endif
