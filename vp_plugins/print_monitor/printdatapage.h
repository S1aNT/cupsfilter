#ifndef PRINTDATAPAGE_H
#define PRINTDATAPAGE_H

#include "mytypes.h"
using namespace VPrn;


#include <QtGui/QWizardPage>
QT_FORWARD_DECLARE_CLASS ( QRadioButton )

QT_FORWARD_DECLARE_CLASS ( QByteArray )

QT_FORWARD_DECLARE_CLASS ( QCheckBox )
QT_FORWARD_DECLARE_CLASS ( QComboBox )
QT_FORWARD_DECLARE_CLASS ( QGroupBox )
QT_FORWARD_DECLARE_CLASS ( QLineEdit )
QT_FORWARD_DECLARE_CLASS ( QDateEdit )
QT_FORWARD_DECLARE_CLASS ( QStandardItemModel )
QT_FORWARD_DECLARE_CLASS ( QSpinBox )
QT_FORWARD_DECLARE_CLASS ( QLabel )

class PrintDataPage : public QWizardPage
{
    Q_OBJECT
public:
    PrintDataPage(QWidget *parent = 0);

    void setLabelText(const QString &l_txt);
    void setMode(int m_mode);
    /**
      * @fn void setModel ( QStandardItemModel *model);
      * @brief установим модель и настроим ее связи с полями
      */
    void setModel ( QStandardItemModel *model);
    /**
      * @fn QByteArray getAllFieldData();
      * @brief Собирает данные всех полей и формирует из них массив пригодный
      * для передачи вначале локальному серверу, а затем соответственно и демону
      */
    QByteArray getAllFieldData();

    /**
      * @fn QString getSQL_mb_check();
      * @brief Собирает данные только полей необходимых для ускоренной проверки
      * по БД учета  и формирует из них массив пригодный
      * для передачи вначале локальному серверу, а затем соответственно и демону
      */
    QString getSQL_mb_check() const;

    /**
      * @fn QString getSQL_mb_register();
      * @brief Собирает данные только полей необходимых для ускоренной проверки
      * по БД учета  и формирует из них массив пригодный
      * для передачи вначале локальному серверу, а затем соответственно и демону
      */
    QString getSQL_mb_register() const;

     /**
      * @fn bool validatePage();
      * @brief Данная функция проверяет корректность заполнения полей
      * проверка последней минутки
      */
    bool validatePage ();
signals:
    /**
      * @fn void field_checked();
      * @brief формальная проверка полей завершена, пусть теперь вышестоящий  узел
      * проверит можно ли печатать такой документ
      */
    void field_checked();

public slots:

    /**
      * @fn void setSecList(const QStringList &s_list);
      * @brief Заполним поле список уровней секретности
      */
    void setSecList(const QStringList &s_list);
       /**
    * @fn void setDocConverted();
    * @brief ставит галочку в обязательном CheckBox документ сконвертирован  в pdf
    */
    void setDocConverted();
   /**
     * @fn void setPageCounted(int pages);
     * @brief Заполняет поле число страниц
    */
    void setPageCounted(int pages);


    void on_firstPageSplitted();
    void on_otherPageSplitted();
private slots:

    void setStamp     (const QString & s){select_stamp = s;}
    /**
      * @fn findTemplatesFilePathInModel(int combo_ind)
      * @brief Есть id выбранного пользователем шаблона, необходимо найти в
      * модели и запомнить полный путь к быранному шаблону
      */
    void findTemplatesFilePathInModel(int combo_ind);

private:
    QComboBox *secretCBox;
    QComboBox *templatesCBox;

    QLineEdit *punktLineEd;
    QLineEdit *mbNumberLineEd;
    QLineEdit *pagesCountLineEd;
    QSpinBox  *e_currentSBox;
    QSpinBox  *e_totalSBox;

    QLineEdit *executor_lineEd;
    QLineEdit *docName_lineEd;
    QLineEdit *pressman_lineEd;
    QLineEdit *invNumber_lineEd;
    QLineEdit *telephone_lineEd;
    QDateEdit *dateField_dateEd;

    QLineEdit *reciver_lineEd_1;
    QLineEdit *reciver_lineEd_2;
    QLineEdit *reciver_lineEd_3;
    QLineEdit *reciver_lineEd_4;
    QLineEdit *reciver_lineEd_5;

    QCheckBox *copy_checkBox_1;
    QCheckBox *copy_checkBox_2;
    QCheckBox *copy_checkBox_3;
    QCheckBox *copy_checkBox_4;
    QCheckBox *copy_checkBox_5;

    QCheckBox *total_copyes; // Печать всех экз.
    QCheckBox *docConverted_checkBox;
    QCheckBox *docSplitted_checkBox;

    int doc_pages_count;
    bool first_split;
    bool other_split;


    /**
      * @var select_templates; Текущий выбранный пользователем шаблон
      * @var select_stamp;     Текущий выбранный пользователем уровень серетности
      * @var t_fileName;;      Полный путь к файлу выбранного пользователем шаблона
      */
    QString select_templates;
    QString select_stamp;
    QString t_fileName;
    QByteArray fields_data;

    QStandardItemModel *m_model; // указатель на модель метаинформации о шаблоне
    /**
      * @fn void setPageSpit();
      * @brief Взводит флажок подготовка документа к печати в зависимости
      * от числа страниц. При старте метода происдоит проверка,если число
      * стр. в документе == 1, и first_split == true , то -> взводим флажок
      * если число стр. в документе >1, и first_split == true
      * && other_split == true, то -> взводим флажок
      */
    void setPageSpit();
    /**
      * @fn void connector();
      * @brief Соединяет сигналы и слоты
      */
    void connector();
    /**
      * @fn bool checkReciver(int r);
      * @brief Проверяет указанный адрес рассылки
      */
    bool checkReciver(int r);
};

#endif // PRINTDATAPAGE_H
