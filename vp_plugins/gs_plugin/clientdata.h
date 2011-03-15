#ifndef CLIENTDATA_H
#define CLIENTDATA_H

#include <QObject>
#include <QtCore/QStack>
#include <QHash>

QT_FORWARD_DECLARE_CLASS(QStringList)

class ClientData : public QObject
{
Q_OBJECT
public:

    explicit ClientData(QObject *parent = 0 );

    /**
      * @fn void incJobsCount(const QString &job_id);
      * @brief Данная функция увеличивает счетчик показывающий кол-во выполняемых
      * фоновых задач для данного клиента
      */
    void incJobsCount(const QString &job_id);
    /**
      * @fn bool isJobFinished(const QString &job_id);
      * @brief Проверка что все задания данного типа для клиента были завершены
      */
    bool isJobFinished(const QString &job_id);

    void clear();

    void setConvertMode(bool b);
    bool getConvertMode();

    void setMainFile (const QString &m);
    QString getMainFile() const;

    void setFirstPage (const QString &fp);
    QString getFirstPage() const;

    void setOtherPage (const QString &op);
    QString getOtherPage() const;

    void setPageCount(int p );
    int getPageCount();

    void startMerge(int number);
    bool isFinishedMerge();

    void startCat(int number);
    bool isFinishedCat();

    void startConvertPdf2Png(int number);
    bool isFinishedConvertPdf2Png();

    void startConvertPdf2Ps(int number);
    bool isFinishedConvertPdf2Ps();
private:
    bool fullDocConvert;

    QString main_file;
    QString first_page;
    QString other_page;
    int page_count;
    /**
      * @var clients_MetaJobMerge;
      * @brief  Для каждого файла который будет объдиняться формируется запись в этом стеке
      * после успешного выполнения команды merge  одна запись из стека удаляется,
      * соответственно когда стек пуст, это служит сигналом об окончании  процесса
      * наложения шаблона на документ
      */
    QStack <int> metaJob_merge;
    QStack <int> metaJob_cat;
    QStack <int> metaJob_pdf2png;
    QStack <int> metaJob_pdf2ps;
    QHash<QString, int> hash;
};

#endif // CLIENTDATA_H
