#ifndef dController_H
#define dController_H

#include <QDebug>
#include <QSettings>
#include <QUuid>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QStandardItemModel>

#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsRectItem>

#include "proc_thread.h"
#include "netagent.h"

#define POINT_TO_CM(cm) ((cm)/28.3465058)
#define POINT_TO_MM(mm) ((mm)/2.83465058)     ////////  0.352777778
#define POINT_TO_DM(dm) ((dm)/283.465058)
#define POINT_TO_INCH(inch) ((inch)/72.0)

#define MM_TO_POINT(mm) ((mm)*2.83465058)
#define CM_TO_POINT(cm) ((cm)*28.3465058)     ///// 28.346456693
#define DM_TO_POINT(dm) ((dm)*283.465058)
#define INCH_TO_POINT(inch) ((inch)*72.0)
static const int ObjectName = 0;

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
    void insertDocToModel(QStringList &item);
public slots:
    // Преобразует выходной файл в pdf
    void convert2PDF(QString &in_file);
    // Объединяет два файла в один печатая шаблон поверх исходного файла
    void mergeTwoPDF(QString &fg_file,QString &bg_file,QString &out_file);
    void checkMB(QString mb);
    void setPrinter(QString selected_printer);
    void savebase(QString wide_msg);
    void printOverSide(QString mb);
    void printWithTemplate (QString templ);
signals:
    void error(int ErrorCode,QString ErrorString);
    void inputFileConverted();  // Файл преобразован в pdf
    void inputFileMerged();    // Файл объединен
    void connect2Demon();
    void sendServerCmd(commands_t cmd,QString & body);
    void exchangeData2MB(QString field_data); // Выдает форматированну строчку с данными полученными от демона в ответ на запрос документа по МБ
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

    QGraphicsScene *scene;
    QMap<QString, int> page_size;
    int page_height;
    int page_width;
    int margin_top,margin_bottom,margin_left,margin_right;
    bool page_orient;

};

#endif // dController_H

