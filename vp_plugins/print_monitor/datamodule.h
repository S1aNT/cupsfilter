#ifndef DATAMODULE_H
#define DATAMODULE_H

#include <QObject>
QT_FORWARD_DECLARE_CLASS (QStandardItemModel)
QT_FORWARD_DECLARE_CLASS (QStringListModel)

class DataModule : public QObject
{
    Q_OBJECT
public:
    explicit DataModule(QObject *parent = 0);


    QStringListModel   *getMandatsModel(){ return m_Mandats_model;}
    /**
      * @fn QStringList &getStampsModel() const;
      * @brief  Возвращает указатель на заполненную модель список уровней секретности
      */
    QStringListModel   *getStampsModel()     { return m_Stamps_model;}
    QStandardItemModel *getDocCardModel()    { return m_DocCard_model;}
    /**
      * @fn QStandardItemModel * getTemplatesModel();
      * @brief Возвращает указатель на заполненную модель метаинформации о шаблоне
      */
    QStandardItemModel *getTemplatesModel()  { return m_Tempalates_model;}
    /**
      * @fn QStandardItemModel * getPrintersModel();
      * @brief Возвращает указатель на модель содержащую информацию о принтерах
      */
    QStandardItemModel *getPrintersModel()   { return m_Printers_model;}

    bool setWorkFile(const QString & i_file);
    QString getWorkFile() const{return m_Work_file;}
signals:
    void error(const QString &eCode,const QString &eMsg);

public slots:

private:
    QStringListModel   *m_Mandats_model;
    QStringListModel   *m_Stamps_model;
    QStandardItemModel *m_DocCard_model;
    QStandardItemModel *m_Tempalates_model;
    QStandardItemModel *m_Printers_model;
    QString             m_Work_file;

};

#endif // DATAMODULE_H
