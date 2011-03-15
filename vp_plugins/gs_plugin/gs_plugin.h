#ifndef GS_PLUGIN_H
#define GS_PLUGIN_H
#include "igs_plugin.h"
#include "mytypes.h"
#include "clientdata.h"

#include <QObject>
#include <QtCore/QLibrary>
#include <QtCore/QMap>
#include <QtCore/QStack>

/** @todo Сделать загрузку библиотеки gs если под виндой понятно как это сделать,
  * хотя и не очень удобно делать, тот с линуксом засада, не пойму как
  */

QT_FORWARD_DECLARE_CLASS (QString)

        using namespace VPrn;

class GS_plugin :public QObject, Igs_plugin
{
    Q_OBJECT
    Q_INTERFACES(Igs_plugin)
public:
    GS_plugin(QObject *parent=0);

    void init(const QString &gs_bin,const QString &pdftk_bin,
              const QString &gsprint_bin,const QString &temp_folder);
    void convertPs2Pdf(const QString &client_uuid,const QString &input_fn);
    void convertPdf2Ps(const QString &client_uuid);
    void final_clear();
    /**
      * @fn bool directPrint(const QString &client_uuid,const QString &file_name,
      *                      const QString &printer_name,int copies);
      * @brief прямая печать на "локальный" прописанный в системе принтер
      */
    void directPrint(const QString &client_uuid,const QString &file_name,
                     const QString &printer_name,int copies);
    /**
      * @fn void catPdf(const QString &client_uuid,const QString &file_nameA,
      *                 const QString &file_nameB, const QString &output_name);
      * @brief Объединяет pdf файлы в один используя pdfTk
      */
    void catPdf(const QString &client_uuid,const QString &file_nameA,
                const QString &file_nameB, const QString &output_name);

    /**
      * @fn  QString getUuid() const
      * @brief служебная функция генерирует ключ
      */
    QString getUuid() const;

    /**
      * @fn void mergeWithTemplate(const QString &client_uuid,
      *                            const QStringList &t_files,
                                   VPrn::PreviewMode prn_mode);
      * @brief Объединяет исходный файл состоящий из 2х частей с элементами шаблона
      * для i экз документа
      */
    void mergeWithTemplate(const QString &client_uuid,
                           const QStringList &t_files,
                           VPrn::PreviewMode prn_mode );
    /**
      * @fn void convertPdfToPng(const QString &client_uuid,
      *                          const QString &filename);
      * @brief Преобразует исходный файл в набор картинок
      */
    void convertPdfToPng(const QString &client_uuid,
                         const QStringList &files);

    /**
      * @fn void createClientData(const QString &client_uuid);
      * @brief Создает новый экз. хранилища данных для клиента
      */
    /**
      * @fn void deleteClientData(const QString &client_uuid);
      * @brief удаляет экз. хранилища данных для клиента
      */
    void createClientData(const QString &client_uuid);
    void deleteClientData(const QString &client_uuid);

    void setConvertToPngMode(const QString &client_uuid, bool full_doc );
    /**
      * @fn QStringList findFiles(const QString &client_uuid,const QStringList &filters);
      * @brief  Формируем списк файлов для клиента отвечающих заданному фильтру
      */
    QStringList findFiles(const QString &client_uuid,const QStringList &filters);
    /**
      * @fn QStringList findFiles4Copy(const QString &client_uuid,
      *                                int copyNum,
      *                                const QStringList &filters
      *                                );
      *
      * @brief  Формируем списк файлов для клиента отвечающих заданному фильтру
      * для заданного конкретного экз.
      */
    QStringList findFiles4Copy(const QString &client_uuid, /*ID клиента*/
                               int copyNum, /*Номер экземпляра документа*/
                               const QStringList &filters /*Тип файлов*/
                               );
    /**
      * @fn  void calcPageCount(const QString &client_uuid,const QString &input_fn);
      * @brief Используя pdfTk получим число страниц в pdf документе
      */
    void calcPageCount(const QString &client_uuid,const QString &input_fn = QString());

signals:
    void pluginNotReady();
    /**
      * @fn void docConvertedToPdf(const QString &client_uuid)
      * @brief Испукается при завершении процесса конвертации ps -> pdf
      */
    void docConvertedToPdf(const QString &client_uuid);
    /**
      * @fn void docReady4work(const QString &client_uuid,int pagecount);
      * @brief Сигнал испускается после завершения конвертации ps->pdf,
      * подсчета числа страниц и разбиения на 1 и последующие страницы
      */
    void docReady4work(const QString &client_uuid,int pagecount);
    /**
      * @fn void docReady4print(const QString &client_uuid);
      * @brief  Сигнал испускается когда на все экз документа наложен шаблон,
      * документ готов к отправке на принтер, или к дальнейшей конвертации в png
      * и предпросмотр
      */
    void docReady4print(const QString &client_uuid);
    /**
      * @fn void docReady4print(const QString &client_uuid);
      * @brief  Сигнал испускается когда все экз документа, в соответсвии с режимом
      * преобразованны в картинки полностью или частично
      */
    void docReady4preview(const QString &client_uuid);

    void error(VPrn::AppErrorType errCode,QString error_message);
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
    /**
      * @fn void filePrinted(client_uuid);
      * @brief Сообщим клиенту, что его файл распечатан
      */
    void filePrinted(const QString &client_uuid);

private slots:
    void threadFinish(const QString &jobKey,int code,const QString &output_message);

private:
    QString gsBin;
    QString spoolDir;
    QString pdftkBin;
    QString gsprintBin;
    QString gs_rcp;

    QStringList myEnv; // Мои переменные среды для gs

    /**
      * @fn void splitPdf(const QString &client_uuid,const QString &main_pdf,
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
      * @fn void mergePdf(const QString &client_uuid,const QString &in_pdf,
      *                   const QString &back_pdf,const QString &out_pdf);
      * @brief Объединяет 2 pdf файла используя pdftk
      */

    void mergePdf(const QString &client_uuid,const QString &in_pdf,
                  const QString &back_pdf,const QString &out_pdf);


    /**
      * @fn ClientData * findClientData(const QString &client_uuid);
      * @brief Ищет в контейнере объект ClientData если находит, то возвращает ссылку на него,
      * иначе создает и возвращает ссылку
      */
    ClientData * findClientData(const QString &client_uuid);
    /**
      * @fn void catFirstPages( const QString &client_uuid );
      * @brief Объединяет первые страницы в один файл.
      */
    void catFirstPages( const QString &client_uuid );
    /**
      * @fn QString formatFileName(const QString &client_uuid,
      *                            const QString &fName,
      *                            const QString &fType,
      *                            const QString &ext);
      * @brief Формирует имя выходного файла по заданным критериям
      */
    /**
      * @fn void recursiveDeletion(QString path);
      * @brief Рекурсивное удалаление каталога со всем его содержимым
      */
    void recursiveDeletion(QString path);
    void createRCPfile(const QString &tmp_dir);
    //-----------------------------------------------------------------------------------------------------
    /**
      * @var clients_list; Список uuid,client_id
      * @var jobs_list;   Список uuid,job_id
      */
    QMap <QString,QString> clients_list;
    QMap <QString,VPrn::Jobs> jobs_list;
    QMap <QString,ClientData *> clients_data;

};


#endif
