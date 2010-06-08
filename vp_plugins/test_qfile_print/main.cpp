#include <QtCore/QCoreApplication>
#include <QFile>
#include <QStringList>
#include <QList>
#include <QPrinterInfo>
#include  <QDebug>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QStringList arg_list = a.arguments();
    QFile prn_file;

    qDebug() << "Available printers:\n";
    QList<QPrinterInfo> prn_list = QPrinterInfo::availablePrinters();
    for (int i = 0; i< prn_list.size();i++){
        qDebug() << i << "-"<< prn_list.at(i).printerName() << "\n";
    }
    qDebug() << arg_list.size();
    if (arg_list.size() == 1 ){
        qDebug() << "Usage: simple_print input_file printer\n";

    }else{

        if (prn_file.copy( arg_list.at(0), arg_list.at(1) )){
            qDebug() << "file: " << arg_list.at(0) << "success send to printer: " << arg_list.at(1) << "\n";
        }else{
            qDebug() << "file: " << arg_list.at(0) << "not send to printer: " << arg_list.at(1) << "\n";
        }
    }
    return a.exec();
}
