#ifndef DCONTROLLER_H
#define DCONTROLLER_H
#include <QDebug>
#include <QObject>
#include <QStringList>
#include <QUuid>
#include <QStandardItemModel>

#include <tech_global.h>


class dController : public QObject
{
    Q_OBJECT

public:
    dController();
    QStandardItemModel *document_model () const;

public slots:
    void connect2Demon(); // Соединение с настройками по умолчанию
    // Преобразует выходной файл в pdf
    void convert2PDF(QString &in_file);
    // Объединяет два файла в один печатая шаблон поверх исходного файла
    void mergeTwoPDF(QString &fg_file,QString &bg_file,QString &out_file);

signals:
    void error(int ErrorCode,QString ErrorString);
    // Процесс настройки состоит из 2 шагов
    // 1 - Установка соединения с сервером
    // 2 - Успешное преобразование входного файла в pdf
    void init_done(int step,QString &message);
    void inputFileMerged();    // Файл объединен

private:
    void read_settings();
    void createModel();

    void insertDocToModel();
    void insertDocToModel(QString &item);
    //void write_settings();
    QString host;
    int port;
    QString SID;

    QStandardItemModel *doc_model;
    QStringList header;
};

#endif // DCONTROLLER_H
