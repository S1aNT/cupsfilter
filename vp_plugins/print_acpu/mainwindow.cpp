#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore/QUuid>	
#include <QFileDialog>
#include <QPrinterInfo>
#include <QMessageBox>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) 
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , HostName(QString())
    , Port (0)
    , client(0)
    , packetSize(-1)

{
    ui->setupUi(this);
    getPrinterList();
    connectToHost("192.168.96.135",4243);
    connect (ui->pushButton,SIGNAL(clicked()),
             this,SLOT(selectFile()));
    //this->selectFile();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::selectFile()
{

    QString file2print = QFileDialog::getOpenFileName(this,
                                                      QObject::trUtf8("Выберите файл для печати"),
                                                      ".", tr("PDF,PS files (*.pdf *.ps)"));
    if (!file2print.isEmpty()){
        // Формируем пакет
        Message message;

        QString jobId = getUuid();
        QString printer_queue = ui->comboBox->currentText();

        QString u_login="SlaNT";
        QString jobTitle = QObject::trUtf8("Тестовое задание на печать, сформированное %1")
                           .arg(QDateTime::currentDateTime ().toString("dd.MM.yyyy hh:mm:ss"));

    }

}

quint64 MainWindow::packFile(const QString &fileName,
                             QByteArray &data)
{
    data.clear();

    QFile file_in(fileName);

    if (!file_in.open(QIODevice::ReadOnly) ){
        return 0;
    }
    QByteArray byteArray_in = file_in.readAll();
    file_in.close();
    data = qCompress(byteArray_in);
    return file_in.size();
}

void MainWindow::connectToHost(const QString &host, quint16 port)
{

    client = new QTcpSocket(this);
    connect(client, SIGNAL(readyRead()),
            this, SLOT(readyRead()));
    connect(client, SIGNAL(connected()),
            this, SLOT(onConnected()));
    //    connect(client, SIGNAL(error(QAbstractSocket::SocketError)),
    //            this, SLOT(selectError(QAbstractSocket::SocketError)));

    client->connectToHost(host, port);
}

void MainWindow::sendMessage(const Message &m_msg)
{

}

void MainWindow::readyRead()
{
    //Свяжем поток и сокет
    QDataStream in ( client );
    in.setVersion(QDataStream::Qt_3_0);

    while (client->bytesAvailable() > 0){
        if (packetSize == -1) {
            //Определим количество байт доступных для чтения;
            //на этом шаге необходимо получить больше 4-х байт
            if( client->bytesAvailable() < sizeof(qint64) ){
                return;
            }
            //Читаем размер пакета
            in >> packetSize;
        }
        //Проверим что в сокет пришел весь пакет а не его огрызки
        if (client->bytesAvailable() < packetSize){
            return;
        }
        //Сбросим размер пакета, для обработки следующего
        packetSize = -1;

        // Прочтем тип сообщения
        int m_Type;
        in >> m_Type;

        //Прочтем само сообщение
        QByteArray msg;
        in >> msg;
        Message message( this );
        message.setType((MessageType) m_Type); //Проверить как конвертирует
        message.setMessageData( msg );

        // Отправка сообщения
        emit messageReady(message);
        QMessageBox msgbox;
        msgbox.setText("Recive msg");
        msgbox.exec();

    }
}

QString MainWindow::getUuid()
{
    return QUuid::createUuid().toString().replace("{","").replace("}","");
}

void MainWindow::onConnected()
{

    Message message( this );
    message.setType(VPrn::Que_RegisterGlobal);

    QString str = QObject::trUtf8("[%1];:;").arg(QUuid::createUuid().toString()
                                                 	   .replace("{","")
                                                           .replace("}","")
                                                          );
    //str.arg(str.length(),0,10);						      
    qDebug() << "Send str: " << str;

    message.setMessageData( str.toUtf8() ); // Пробразуем в QByteArray
    sendMessage(message);

}


void MainWindow::getPrinterList()
{
    QList<QPrinterInfo> pList = QPrinterInfo::availablePrinters();

    QList<QPrinterInfo>::const_iterator i;

    for (i = pList.constBegin(); i != pList.constEnd(); ++i){
        QPrinterInfo pInfo = *i;
        if (pInfo.isDefault () ){
            ui->comboBox->addItem(pInfo.printerName(),QVariant("def"));
        }else{
            ui->comboBox->addItem(pInfo.printerName());
        }
    }
    int ind = ui->comboBox->findData(QVariant("def"));
    if (ind != -1){
        ui->comboBox->setCurrentIndex(ind);
    }
}
