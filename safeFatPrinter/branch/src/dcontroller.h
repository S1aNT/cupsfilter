#ifndef DCONTROLLER_H
#define DCONTROLLER_H
#include <QDebug>
#include <QObject>
#include <QTextStream>
#include <QRegExp>
#include <QFileInfo>
#include <QStringList>
#include <QUuid>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QSettings>
#include <QDate>
#include <QPrinterInfo>
#include <QCoreApplication>
#include <QGraphicsScene>

#include "simpleitem.h"
#include "tech_global.h"
#include "proc_thread.h"
#include "net_agent.h"
#include "spyder.h"
#define MM_TO_POINT(mm) ((mm)*2.83465058)

class dController : public QObject
{
    Q_OBJECT

public:
    dController(QObject * parent = 0);
    QStandardItemModel *document_model () const {return doc_model;}
    QStringListModel *stamp_model() const{ return stampModel;}
    bool isUser_Mandat() const;
    QStringList printers_list() const {return printerList;}
    QString glob_templates() const {return globalTemplates;};
    void printWithTemplate (int RowId);
    void printOverSide(int RowId);
    QString mainPDF;	 // Файл который получился в результате конвертирования исходного
public slots:
    void connect2Demon(); // Соединение с настройками по умолчанию
    // Преобразует выходной файл в pdf
    void convert2PDF(QString &in_file);
    // Объединяет два файла в один печатая шаблон поверх исходного файла
    void mergeTwoPDF(QString &fg_file,QString &bg_file,QString &out_file);
    void checkMB(QString mb); // Проверка наличия МБ в базе
private slots:
    void readServerResponce(QString &line);
    void done_ftp_transfer();
    void parseCnvThread(int Code,QString output );
    void parseMergeThread(int Code,QString output );
signals:
    void error(int ErrorCode,QString ErrorString);
    // Процесс настройки состоит из 2 шагов
    // 1 - Установка соединения с сервером
    // 2 - Успешное преобразование входного файла в pdf
    void init_done(steps_t step,QString &message);
    void inputFileMerged();    // Файл объединен
    void sendServerCmd(commands_t cmd,QString & body);

    // в базе аудита такого документа (с таким МБ) нет
    void mbNumberNotExist();
     // В базе аудита есть такой документ, RowID -1 документ помечен ка распечатаный
    void mbNumberExist(int RowID);
    // Для пользователя нет ни одного принтера
    void printersNotFound();

private:
    void read_settings();
    //void write_settings();
    void getTemplatesFromServer(QString &saveTo);

    QDate begin_date; // Период за которы запрашивается с сервера МБ при проверке
    QDate end_date;

    QString serverHostName;
    int serverPort;
    QString SID;

    void createModel();
    void getStampName();
    void getPrinterCount();
    void insertDocToModel();
    void insertDocToModel(QString &item);

    QStandardItemModel *doc_model;
    QStringListModel *stampModel;
    QStringList header;

    void getUserNameMandat();
    QString Mandat;
    QString UserName;   

    // задержки сети для тонкой настройки
    net_agent *netAgent;
    int timeout_connect;
    int timeout_read;
    int timeout_write;


    QString gsBin;	// путь к исполняемому файлу ghostscript
    QString pdftkBin;	// Путь к файлу pdfTK (питоновский скрип под линуксом)
    QString outPDF;
    QString spoolDIR;
    QString localTemplates;
    QString globalTemplates;
    QString ftpTemplatesDir;

    void createPrinterList(); // Формирует список доступных принтеров
    QString curPrinter; // Текущий выбранный пользователем принтер
    QStringList printerList; // Список доступных принтеров
    QString printer_list_pid; // файл который создает программа формирования списка принтеров
};

#endif // DCONTROLLER_H
