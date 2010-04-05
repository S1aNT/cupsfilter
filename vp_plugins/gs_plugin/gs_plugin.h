#ifndef GS_PLUGIN_H
#define GS_PLUGIN_H
#include "igs_plugin.h"
#include  "mytypes.h"

#include <QObject>
#include <QtCore/QLibrary>

#include <QtCore/QMap>

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
              const QString &temp_folder);
    void convertPs2Pdf(const QString &client_uuid,const QString &input_fn);

    void final_clear();

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

private:
    QString gsBin;
    QString tempPath;
    QString pdftkBin;
    QString gs_rcp;
    QStringList args;
    QStringList myEnv; // Мои переменные среды для gs

    /**
      * @fn  void getPageCount(const QString &client_uuid,const QString &input_fn);
      * @brief Используя pdfTk получим число страниц в pdf документе
      */
    void getPageCount(const QString &client_uuid,const QString &input_fn);
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
