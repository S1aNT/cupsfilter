#ifndef CLIENTDATA_H
#define CLIENTDATA_H

#include <QObject>
#include <QtCore/QStack>

QT_FORWARD_DECLARE_CLASS(QStringList)

class ClientData : public QObject
{
Q_OBJECT
public:

    explicit ClientData(QObject *parent = 0 );
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

    void startConverPdf2Png(int number);
    bool isFinishedConvertPdf2Png();

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
};

#endif // CLIENTDATA_H
