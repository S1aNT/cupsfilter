#ifndef dController_H
#define dController_H

#include <QDebug>
#include <QSettings>
#include <QUuid>
#include <QDate>
#include <QStringList>
#include <QMap>
#include <QStandardItemModel>


#include <QPrinter>
#include <QPrintPreviewDialog>


#include "template_painter.h"
#include "proc_thread.h"
#include "netagent.h"



class dController : public QObject{
    Q_OBJECT
public:
    dController( QObject *parent = 0 );
    ~dController();
    bool isConnect();
    void read_settings();
    void write_settings();
    QStandardItemModel *document_model () const;

    void insertDocToModel();
    void insertDocToModel(QString &item);
public slots:
    // Преобразует выходной файл в pdf
    void convert2PDF(QString &in_file);
    // Объединяет два файла в один печатая шаблон поверх исходного файла
    void mergeTwoPDF(QString &fg_file,QString &bg_file,QString &out_file);

    void setPrinter(QString selected_printer);
    void savebase(QString wide_msg);
    void printOverSide(int RowId);
    void printWithTemplate (int RowId);

    void checkMB(QString mb);
signals:
    void error(int ErrorCode,QString ErrorString);
    void inputFileConverted();  // Файл преобразован в pdf
    void inputFileMerged();    // Файл объединен
    void connect2Demon();
    void sendServerCmd(commands_t cmd,QString & body);

    // в базе аудита такого документа (с таким МБ) нет
    void mbNumberChecked(QString &mb);
     // В базе аудита есть такой документ, RowID -1 документ помечен ка распечатаный
    void mbNumberExist(int RowID);
     // Выдает форматированну строчку с данными полученными от демона в ответ на запрос документа по МБ
    //void exchangeData2MB(QString field_data);
    void sayMeGood();
private slots:
    void parseCnvThread(int Code,QString output);
    void parseMergeThread(int Code,QString output);
    void parseNetwork(QString &message);
private:
    QString userName;
    QString userMandat;
    void connect2Server(QString &host,int port);
    void createDocModel();
protected:
    QString SID;
    QStandardItemModel *doc_model;
    QStringList header;

    netAgent *netClient;
    QString serverHostName;
    int serverPort;
    bool conn_state;
// задержки сети для тонкой настройки
    int timeout_connect;
    int timeout_read;
    int timeout_write;


    QStringList args;
    QStringList currentDoc;
    QString mainPDF;	 // Файл который получился в результате конвертирования исходного
    QString gsBin;	// путь к исполняемому файлу ghostscript
    QString pdftkBin;	// Путь к файлу pdfTK
    QString outPDF;
    QString spoolDIR;	

    QString curPrinter; // Текущий выбранный пользователем принтер



};

#endif // dController_H

