#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QTcpSocket>
#include <QMainWindow>
#include <QByteArray>

#include "message.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void onConnected();
    void readyRead();
    void selectFile();
signals:
    void messageReady( const Message &msg );

private:
    Ui::MainWindow *ui;
    void connectToHost(const QString &host, quint16 port);
    void sendMessage(const Message &m_msg);
    void getPrinterList();
    QString HostName;
    quint16 Port;
    QTcpSocket *client;  
    qint64 packetSize;

    QString getUuid();
    quint64 packFile(const QString &fileName,QByteArray &data);

};

#endif // MAINWINDOW_H
