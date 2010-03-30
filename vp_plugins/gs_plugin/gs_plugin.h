#ifndef GS_PLUGIN_H
#define GS_PLUGIN_H
#include "igs_plugin.h"
#include  "mytypes.h"

#include <QObject>
#include <QtCore/QLibrary>

#include <QtCore/QMap>


class QString;

using namespace VPrn;

class GS_plugin :public QObject, Igs_plugin
{
    Q_OBJECT
    Q_INTERFACES(Igs_plugin)
public:
    GS_plugin(QObject *parent=0);
    void init(const QString &gs_bin,const QString &pdftk_bin,const QString &temp_folder,const QString &sid);
    void convertPs2Pdf(const QString &client_uuid,const QString &input_fn);
    /*
    QString getFirstPages(){return firstPage_fn;};
    QString getOtherPages(){return otherPages_fn;};
    QPixmap getSnapShot(){return currentPageSnapShot;};
    void convertPs2Pdf(const QString &input_fn);
    void getPageCount(const QString &input_fn);
    void convertPdf2Png(const QString &fn, int convertedPage);
    void merge2Pdf(const QString &input_fn,const QString &background_fn,const QString &output_fn);
    void printPdf(const QString &print_fn,const QString &printer_name);
    void addPdfMark(const QString &input_fn,const QString &user_name, const QString &host_name,quint16 host_ip);
    void merge_mark_print(const QString &input_fn,const QString &background_fn,
                          const QString &user_name,const QString &printer_name);
    void clearAll();
*/
signals:
    void error(pluginsError errCode,QString error_message);
    /**
      * @fn void jobFinish (const QString &client_uuid,
      *                      VPrn::Jobs job_id,int code,
      *                      const QString &output_message);
      * @param client;         Кто заказал работу
      * @param job_id;         Какую работу выполняли
      * @param code;           С каким результатом 0 успешно, 1 и далее ошибка
      * @param output_message; Расширенный ответ
      */
    void jobFinish(const QString &client_uuid,VPrn::Jobs job_id,int code,
                   const QString &output_message);

private slots:
    void threadFinish(const QString &jobKey,int code,const QString &output_message);

    /*
    //TODO добавить для всех этих слотов сигнал маппер и объединить в одну функцию
    //TODO порождаемый поток держать в спящем состоянии и пробуждать при приходе новой команды
    //то есть при инициализации загружать библиотеку gs_lib
    // а не создавать каждый раз заново.
    void parseCnvThread(int Code,QString output);
    void parsePageCountThread(int Code,QString output);
    void parsePrintThread(int Code,QString output);
    void parseFirstPageThread(int Code,QString output);
    void parseOtherPageThread(int Code,QString output);
    void parseMergeThread(int Code,QString output);
   void parseMergeToPrint(int Code,QString output );
    void parseAddPdfMarkThread(int Code,QString output);
    void parseCnv2PngThread(int Code,QString output);
*/
private:
    QString gsBin;
    QString tempPath;
    QString pdftkBin;
    QStringList args;
    QStringList myEnv; // Мои переменные среды для gs
    QString Sid;
    QString firstPage_fn;
    QString otherPages_fn;
    QString mainPDF;
    QString pdf2png_page;
    QString gs_rcp;
    QString printer;
    QString currentPrintPage;


    /**
  * @fn  void getPageCount(const QString &client_uuid,const QString &input_fn);
  * @brief Используя pdfTk получим число страниц в pdf документе
  */
    void getPageCount(const QString &client_uuid,const QString &input_fn);
    /**
    * @fn   void splitPdf(const QString &client_uuid,const QString &main_pdf,
    *                     const QString &first_page, const QString &other_pages);
    * @brief Делит файл имеющий на блоки первая страница, остальные страницы
    */
    void splitPdf(const QString &client_uuid,const QString &main_pdf,
                  const QString &first_page, const QString &other_pages);

    /**
      * @fn void start_proc(const QString &client_uuid,const QString &bin,
                            QStringList &arg_list,VPrn::Jobs job_id);
      * @brief Запускает отдельный поток вычислений, в интересах клиента
      */
    void start_proc(const QString &client_uuid,const QString &bin,
                    QStringList &arg_list,VPrn::Jobs job_id);
    /**
      * @fn  QString getUuid() const
      * @brief служебная функция генерирует ключ
      */
    QString getUuid() const;
    /**
      * @var clients_list; Список uuid,client_id
      * @var jobs_list;   Список uuid,job_id
      */
    QMap <QString,QString> clients_list;
    QMap <QString,VPrn::Jobs> jobs_list;

};


#endif
