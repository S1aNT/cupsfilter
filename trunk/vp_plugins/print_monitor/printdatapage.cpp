#include "printdatapage.h"


#include <QtGui/QLabel>
#include <QtGui/QCheckBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QRadioButton>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QLineEdit>
#include <QtGui/QFont>
#include <QtGui/QSpinBox>
#include <QtGui/QDateEdit>
#include <QtGui/QMessageBox>
#include <QtCore/QByteArray>
#include <QtCore/QDataStream>
#include <QtCore/QHash>

PrintDataPage::PrintDataPage(QWidget *parent)
    : QWizardPage(parent)
    , doc_pages_count(0)
    , first_split(false)
    , other_split(false)
{
    // Скрытые чекбоксы
    docConverted_checkBox = new QCheckBox(this);
    docSplitted_checkBox = new QCheckBox(this);

    docConverted_checkBox->hide();
    docSplitted_checkBox->hide();

    //this->resize(800,600);
    setTitle(QObject::trUtf8("Режим работы:"));
    QGridLayout *centralgridLayout = new QGridLayout(this);

    QFont font;
    font.setFamily(QString::fromUtf8("Times New Roman"));
    font.setPointSize(12);
    font.setBold(true);
    //font.setWeight(75);

    QGroupBox *centralGroupBox = new QGroupBox(this);
    centralGroupBox->setTitle(QObject::trUtf8("Название документа"));
    QHBoxLayout *hBoxLayout = new QHBoxLayout(centralGroupBox);

    docName_lineEd = new QLineEdit(centralGroupBox);
    docName_lineEd->setEnabled(true);
    hBoxLayout->addWidget(docName_lineEd);

    QGroupBox *commonGroupBox = new QGroupBox(this);
    commonGroupBox->setTitle(QObject::trUtf8("Общие атрибуты"));
    commonGroupBox->setEnabled(true);
    //commonGroupBox->setGeometry(QRect(5, 83, 301, 161));

    QGridLayout *gridLayout = new QGridLayout(commonGroupBox);

    QLabel *label = new QLabel(commonGroupBox);
    label->setFont(font);
    label->setText(QObject::trUtf8("Гриф"));

    secretCBox = new QComboBox(commonGroupBox);

    QLabel *label_10 = new QLabel(commonGroupBox);
    label_10->setFont(font);
    label_10->setText(QObject::trUtf8("Экз №"));

    QLabel *label_3 = new QLabel(commonGroupBox);
    label_3->setFont(font);
    label_3->setText(QObject::trUtf8("Пункт"));

    punktLineEd = new QLineEdit(commonGroupBox);

    QLabel *label_4 = new QLabel(commonGroupBox);
    label_4->setFont(font);
    label_4->setText(QObject::trUtf8("Номер МБ"));

    mbNumberLineEd = new QLineEdit(commonGroupBox);

    QLabel *label_15 = new QLabel(commonGroupBox);
    label_15->setFont(font);
    label_15->setText(QObject::trUtf8("Количество листов"));

    pagesCountLineEd = new QLineEdit(commonGroupBox);
    pagesCountLineEd->setReadOnly(true);

    e_currentSBox = new QSpinBox(commonGroupBox);
    e_currentSBox->setEnabled(false);
    e_currentSBox->setRange(1,5);

    e_totalSBox = new QSpinBox(commonGroupBox);
    e_totalSBox->setEnabled(false);
    e_totalSBox->setRange(1,5);

    QLabel *label_2 = new QLabel(commonGroupBox);
    //label_2->setFont(font);
    label_2->setAlignment(Qt::AlignCenter);
    label_2->setText(QObject::trUtf8("Всего"));

    total_copyes = new QCheckBox(commonGroupBox);
    total_copyes->setText(QObject::trUtf8("Печать всех экз."));
    total_copyes->setChecked(true);

    QGroupBox *middleGroupBox = new QGroupBox(this);
    middleGroupBox->setTitle(QObject::trUtf8("Шаблоны"));
    QHBoxLayout *hBoxLayout_2 = new QHBoxLayout(middleGroupBox);

    templatesCBox = new QComboBox(middleGroupBox);
    templatesCBox->setEnabled(true);
    hBoxLayout_2->addWidget(templatesCBox);

    gridLayout->addWidget(label,            0, 0, 1, 1);
    gridLayout->addWidget(secretCBox,       0, 1, 1, 9);
    gridLayout->addWidget(label_4,          1, 0, 1, 3);
    gridLayout->addWidget(mbNumberLineEd,   1, 4, 1, 6);

    gridLayout->addWidget(label_3,          2, 0, 1, 1);
    gridLayout->addWidget(punktLineEd,      2, 4, 1, 6);
    gridLayout->addWidget(label_15,         3, 0, 1, 6);
    gridLayout->addWidget(pagesCountLineEd, 3, 8, 1, 2);

    gridLayout->addWidget(label_10,         4, 0, 1, 2);
    gridLayout->addWidget(e_currentSBox,    4, 2, 1, 1);
    gridLayout->addWidget(e_totalSBox,      4, 9, 1, 1);
    gridLayout->addWidget(label_2,          4, 6, 1, 1);
    gridLayout->addWidget(total_copyes,     5, 0, 1, 6);
    gridLayout->addWidget(middleGroupBox,   6, 0, 1, 9);


    QGroupBox *lastPageGroupBoxChecked = new QGroupBox(this);
    lastPageGroupBoxChecked->setTitle(QObject::trUtf8("Штамп последней страницы"));
    QGridLayout *gridLayout_2 = new QGridLayout(lastPageGroupBoxChecked);

    QLabel *label_5 = new QLabel(lastPageGroupBoxChecked);
    //label_5->setFont(font);
    label_5->setText(QObject::trUtf8("Исполнитель"));

    executor_lineEd = new QLineEdit(lastPageGroupBoxChecked);
    executor_lineEd->setEnabled(true);

    QLabel *label_6 = new QLabel(lastPageGroupBoxChecked);
    //label_6->setFont(font);
    label_6->setText(QObject::trUtf8("Отпечатал"));

    pressman_lineEd = new QLineEdit(lastPageGroupBoxChecked);
    pressman_lineEd->setEnabled(true);

    QLabel *label_7 = new QLabel(lastPageGroupBoxChecked);
    //label_7->setFont(font);
    label_7->setText(QObject::trUtf8("Телефон"));

    QLabel *label_8 = new QLabel(lastPageGroupBoxChecked);
    //label_8->setFont(font);
    label_8->setText(QObject::trUtf8("Инв. №"));

    QLabel *label_9 = new QLabel(lastPageGroupBoxChecked);
    //label_9->setFont(font);
    label_9->setText(QObject::trUtf8("Дата"));

    invNumber_lineEd = new QLineEdit(lastPageGroupBoxChecked);
    invNumber_lineEd->setEnabled(true);

    telephone_lineEd = new QLineEdit(lastPageGroupBoxChecked);
    telephone_lineEd->setEnabled(true);
    telephone_lineEd->setInputMask(QString::fromUtf8(""));

    dateField_dateEd = new QDateEdit(lastPageGroupBoxChecked);
    dateField_dateEd->setEnabled(true);
    dateField_dateEd->setDateTime(QDateTime::currentDateTime());
    dateField_dateEd->setCalendarPopup(true);

    gridLayout_2->addWidget(label_5,          0, 0, 1, 1);
    gridLayout_2->addWidget(executor_lineEd,  0, 1, 1, 1);
    gridLayout_2->addWidget(label_6,          1, 0, 1, 1);
    gridLayout_2->addWidget(pressman_lineEd,  1, 1, 1, 1);
    gridLayout_2->addWidget(label_7,          2, 0, 1, 1);
    gridLayout_2->addWidget(telephone_lineEd, 2, 1, 1, 1);
    gridLayout_2->addWidget(label_8,          3, 0, 1, 1);
    gridLayout_2->addWidget(invNumber_lineEd, 3, 1, 1, 1);
    gridLayout_2->addWidget(label_9,          5, 0, 1, 1);
    gridLayout_2->addWidget(dateField_dateEd, 5, 1, 1, 1);

    QGroupBox *reciversListGroupBoxChecked = new QGroupBox(this);
    //reciversListGroupBoxChecked->setGeometry(QRect(7, 307, 659, 151));
    reciversListGroupBoxChecked->setCheckable(false);
    reciversListGroupBoxChecked->setChecked(false);
    reciversListGroupBoxChecked->setTitle(QObject::trUtf8("Список рассылки"));

    QGridLayout *gridLayout_4 = new QGridLayout(reciversListGroupBoxChecked);

    reciver_lineEd_1 = new QLineEdit(reciversListGroupBoxChecked);
    reciver_lineEd_1->setEnabled(false);

    reciver_lineEd_2 = new QLineEdit(reciversListGroupBoxChecked);
    reciver_lineEd_2->setEnabled(false);

    reciver_lineEd_3 = new QLineEdit(reciversListGroupBoxChecked);
    reciver_lineEd_3->setEnabled(false);

    reciver_lineEd_4 = new QLineEdit(reciversListGroupBoxChecked);
    reciver_lineEd_4->setEnabled(false);

    reciver_lineEd_5 = new QLineEdit(reciversListGroupBoxChecked);
    reciver_lineEd_5->setEnabled(false);

    copy_checkBox_1 = new QCheckBox(reciversListGroupBoxChecked);
    //copy_checkBox_1->setFont(font);
    copy_checkBox_1->setText(QObject::trUtf8("Экземпляр №1"));

    copy_checkBox_2 = new QCheckBox(reciversListGroupBoxChecked);
    //copy_checkBox_2->setFont(font);
    copy_checkBox_2->setText(QObject::trUtf8("Экземпляр №2"));

    copy_checkBox_3 = new QCheckBox(reciversListGroupBoxChecked);
    //copy_checkBox_3->setFont(font);
    copy_checkBox_3->setText(QObject::trUtf8("Экземпляр №3"));

    copy_checkBox_4 = new QCheckBox(reciversListGroupBoxChecked);
    //copy_checkBox_4->setFont(font);
    copy_checkBox_4->setText(QObject::trUtf8("Экземпляр №4"));

    copy_checkBox_5 = new QCheckBox(reciversListGroupBoxChecked);
    //copy_checkBox_5->setFont(font);
    copy_checkBox_5->setText(QObject::trUtf8("Экземпляр №5"));

    gridLayout_4->addWidget(copy_checkBox_1, 0, 0, 1, 1);
    gridLayout_4->addWidget(copy_checkBox_2, 2, 0, 1, 1);
    gridLayout_4->addWidget(copy_checkBox_3, 3, 0, 1, 1);
    gridLayout_4->addWidget(copy_checkBox_4, 4, 0, 1, 1);
    gridLayout_4->addWidget(copy_checkBox_5, 5, 0, 1, 1);

    gridLayout_4->addWidget(reciver_lineEd_1, 0, 1, 1, 1);
    gridLayout_4->addWidget(reciver_lineEd_2, 2, 1, 1, 1);
    gridLayout_4->addWidget(reciver_lineEd_3, 3, 1, 1, 1);
    gridLayout_4->addWidget(reciver_lineEd_4, 4, 1, 1, 1);
    gridLayout_4->addWidget(reciver_lineEd_5, 5, 1, 1, 1);

    centralgridLayout->addWidget( centralGroupBox             ,0,0,1,3);
    centralgridLayout->addWidget( commonGroupBox              ,1,0,1,1);
    centralgridLayout->addWidget( lastPageGroupBoxChecked     ,1,1,1,1 );
    centralgridLayout->addWidget( reciversListGroupBoxChecked ,2,0,1,3 );

    setLayout(centralgridLayout);
    // Поля выбираемые пользователем
    registerField("docName_lineEd*",docName_lineEd);
    registerField("secretCBox*", secretCBox);
    registerField("mbNumberLineEd*", mbNumberLineEd);
    registerField("punktLineEd*", punktLineEd);
    registerField("e_currentSBox",e_currentSBox);
    registerField("e_totalSBox",e_totalSBox);
    registerField("total_copyes",total_copyes);
    registerField("templatesCBox*",  templatesCBox);

    registerField("executor_lineEd*",executor_lineEd);
    //registerField("pressman_lineEd",pressman_lineEd);
    registerField("invNumber_lineEd*",invNumber_lineEd);
    registerField("telephone_lineEd*",telephone_lineEd);
    registerField("dateField_dateEd",dateField_dateEd);

    registerField("reciver_lineEd_1", reciver_lineEd_1);
    registerField("reciver_lineEd_2", reciver_lineEd_2);
    registerField("reciver_lineEd_3", reciver_lineEd_3);
    registerField("reciver_lineEd_4", reciver_lineEd_4);
    registerField("reciver_lineEd_5", reciver_lineEd_5);
    //Автоматически заполянямые поля
    registerField("pagesCountLineEd*",  pagesCountLineEd);
    registerField("docConverted_checkBox*",docConverted_checkBox);
    registerField("docSplitted_checkBox*",docSplitted_checkBox);
    connector();
    this->setCommitPage(true);
}

void PrintDataPage::setPageSpit()
{
    if ((doc_pages_count == 1 && first_split ) ||
        (doc_pages_count >  1 && first_split && other_split  )){
        docSplitted_checkBox->setChecked(true);
        QMessageBox msgbox;
        msgbox.setText(QObject::trUtf8("Предпечатное разбиение документа на группы стр. завершенно!"));
        msgbox.exec();
    }
}

void PrintDataPage::setLabelText(const QString &l_txt)
{
    setSubTitle (l_txt);
}

bool PrintDataPage::validatePage ()
{
    QMessageBox msgbox;
    bool Ok = true;
    {
        if (pressman_lineEd->text().isEmpty()){
            pressman_lineEd->setText(executor_lineEd->text());
        }

        if (total_copyes->isChecked()){
            // Проверим что вкл. все поля в списке рассылки и список заполнен!
            for (int i=1;i<6;i++){

                if (!checkReciver(i)){
                    msgbox.setText(
                            QObject::trUtf8("Адресат №%1, при данных условиях не может быть не заполненным!")
                            .arg(i,0,10)
                            );
                    msgbox.exec();
                    Ok &= false;
                }
            }
        }else{
            // Проверка что номер экземпляра не превышает число всего экз.

            if (e_totalSBox->value() < e_currentSBox->value()){
                msgbox.setText(
                        QObject::trUtf8("Общее число - [%1] экз. документа  не может быть меньше, чем печатаемый - [%2] экз.")
                        .arg(e_totalSBox->value(),0,10)
                        .arg(e_currentSBox->value(),0,10)
                        );
                msgbox.exec();
                Ok &= false;
            }

            if (!checkReciver(e_currentSBox->value())){
                msgbox.setText(
                        QObject::trUtf8("Адресат №%1, при данных условиях не может быть не заполненным!")
                        .arg(e_currentSBox->value(),0,10)
                        );
                msgbox.exec();
                Ok &= false;
            }
        }
    }
    if (Ok){
        emit field_checked();
    }
    return Ok;
}

bool PrintDataPage::checkReciver(int r)
{
    bool Ok = false;
    {
        switch (r){
        case 1:
            Ok = reciver_lineEd_1->isModified();
            if (reciver_lineEd_1->text().isEmpty()){
                Ok &= false;
            }
            break;
        case 2:
            Ok = reciver_lineEd_2->isModified();
            if (reciver_lineEd_2->text().isEmpty()){
                Ok &= false;
            }
            break;
        case 3:
            Ok = reciver_lineEd_3->isModified();
            if (reciver_lineEd_3->text().isEmpty()){
                Ok &= false;
            }
            break;
        case 4:
            Ok = reciver_lineEd_4->isModified();
            if (reciver_lineEd_4->text().isEmpty()){
                Ok &= false;
            }
            break;
        case 5:
            Ok = reciver_lineEd_5->isModified();
            if (reciver_lineEd_5->text().isEmpty()){
                Ok &= false;
            }
            break;
        }
    }
    return Ok;
}

void PrintDataPage::setMode(int m_mode)
{
    switch (m_mode){
    case 0:
        //setPixmap(QWizard::WatermarkPixmap, QPixmap(":/set_date.png"));
        break;
    case 1:
        //setPixmap(QWizard::WatermarkPixmap, QPixmap(":/gears.png"));
        break;
    case 2:
        //setPixmap(QWizard::WatermarkPixmap, QPixmap(":/gears.png"));
        break;

    }
}

void PrintDataPage::setTemplatesFileName(const QString &fn)
{
    t_fileName = fn;
}
QString PrintDataPage::getSQL_mb_check() const
{
    QString query;

    query = tr("SELECT FROM WHERE %1 and %2 and %3 and %4")
            .arg( docName_lineEd->text() )
            .arg( secretCBox->currentText() )
            .arg( mbNumberLineEd->text() )
            .arg( dateField_dateEd->dateTime().toTime_t() )
            ;
    // Проверка на номер экз
    if (!total_copyes->isChecked()){
        query.append(tr("номер экз ='%1'")
                     .arg(e_currentSBox->value(),0,10) ); /// @todo дописать запрос
    }

    return query;
}

QString PrintDataPage::getSQL_mb_register() const
{
    QString query;
    return query;
}

QByteArray PrintDataPage::getAllFieldData()
{
    /**
      * @warning Если меняешь названия тут, не забудь поменять их
      * в @fn void Tmpl_sql_plugin::init(const QString & spool,const QString & sid)
      * @file tmp_sql_plugin.cpp
      */
    int cur_copyes;
    if (total_copyes->isChecked()){// стоит отметка все экз. пишем 0!!!
        cur_copyes = 0;
    }else{
        cur_copyes = e_currentSBox->value();
    }
    int all_copyes = e_totalSBox->value();

    QHash <QString, QString> m_tagValue;
    //формируем хеш значений
    m_tagValue.insert( QObject::trUtf8("Название док-та"),docName_lineEd->text());
    m_tagValue.insert( QObject::trUtf8("Гриф"), select_stamp );
    m_tagValue.insert( QObject::trUtf8("МБ"),mbNumberLineEd->text() );
    m_tagValue.insert( QObject::trUtf8("Пункт перечня"),punktLineEd->text() );
    m_tagValue.insert( QObject::trUtf8("Кол-во листов"), tr("%1").arg(doc_pages_count,0,10) );
    m_tagValue.insert( QObject::trUtf8("Номер экз."), tr("%1").arg(cur_copyes,0,10) );
    m_tagValue.insert( QObject::trUtf8("Кол-во экз."), tr("%1").arg(all_copyes,0,10) );
    m_tagValue.insert( QObject::trUtf8("Исполнитель"), executor_lineEd->text());
    m_tagValue.insert( QObject::trUtf8("Отпечатал"), pressman_lineEd->text());
    m_tagValue.insert( QObject::trUtf8("Телефон"),  telephone_lineEd->text());
    m_tagValue.insert( QObject::trUtf8("Инв. N"), invNumber_lineEd->text());
    m_tagValue.insert( QObject::trUtf8("Дата распечатки"),dateField_dateEd->date().toString());

    if (!reciver_lineEd_1->text().isEmpty()){
        m_tagValue.insert(QObject::trUtf8("Получатель N1"), reciver_lineEd_1->text());
    }
    if (!reciver_lineEd_2->text().isEmpty()){
        m_tagValue.insert(QObject::trUtf8("Получатель N2"), reciver_lineEd_2->text());
    }
    if (!reciver_lineEd_3->text().isEmpty()){
        m_tagValue.insert(QObject::trUtf8("Получатель N3"), reciver_lineEd_3->text());
    }
    if (!reciver_lineEd_4->text().isEmpty()){
        m_tagValue.insert(QObject::trUtf8("Получатель N4"), reciver_lineEd_4->text());
    }
    if (!reciver_lineEd_5->text().isEmpty()){
        m_tagValue.insert(QObject::trUtf8("Получатель N5"), reciver_lineEd_5->text());
    }


    //Очистим массивчик
    fields_data.clear();

    // Заполним его в формате ключ-значение
    QDataStream out(&fields_data, QIODevice::WriteOnly );
    out.setVersion(QDataStream::Qt_3_0);
    // Запишем выбранный пользователем шаблон
    out << t_fileName;
    // save hash
    out << m_tagValue;
    return fields_data;
}

void PrintDataPage::setSecList(const QStringList &s_list)
{
    secretCBox->addItems(s_list);
    secretCBox->setCurrentIndex(-1);
}

void PrintDataPage::setTemplatesList(const QStringList &t_list)
{
    templatesCBox->addItems(t_list);
    templatesCBox->setCurrentIndex(-1);
}

void PrintDataPage::setDocConverted()
{
    docConverted_checkBox->setChecked(true);
    QMessageBox msgbox;
    msgbox.setText(QObject::trUtf8("Предпечатное преобразование документа завершенно!"));
    msgbox.exec();
}

void PrintDataPage::setPageCounted(int pages)
{
    doc_pages_count = pages;
    pagesCountLineEd->setText(QObject::trUtf8("%1 листов.").arg(pages,0,10));
}

void PrintDataPage::on_firstPageSplitted()
{
    first_split = true;
    setPageSpit();
}

void PrintDataPage::on_otherPageSplitted()
{
    other_split = true;
    setPageSpit();
}

//-------------------------------- PRIVATE FUNCTIONS ---------------------------
void  PrintDataPage::connector()
{
    // Запомним шаблон
//    connect (templatesCBox, SIGNAL(currentIndexChanged(QString)),
//             this, SLOT(setTemplates(QString))
//             );
    connect (templatesCBox, SIGNAL(currentIndexChanged(QString)),
             this, SIGNAL(templatesChanged(QString))
             );

    // Запомним выбранный пользователем уровень секретности
    connect (secretCBox, SIGNAL(currentIndexChanged(QString)),
             this, SLOT(setStamp(QString))
             );

    // Выбор режима печати экз. или все экз.
    connect(total_copyes, SIGNAL(toggled(bool)),
            e_currentSBox,SLOT(setDisabled(bool))
            );
    connect(total_copyes, SIGNAL(toggled(bool)),
            e_totalSBox,  SLOT(setDisabled(bool))
            );

    // поля ввода адресатов
    connect(copy_checkBox_1,  SIGNAL(toggled(bool)),
            reciver_lineEd_1, SLOT(setEnabled(bool))
            );
    connect(copy_checkBox_2,  SIGNAL(toggled(bool)),
            reciver_lineEd_2, SLOT(setEnabled(bool))
            );
    connect(copy_checkBox_3,  SIGNAL(toggled(bool)),
            reciver_lineEd_3, SLOT(setEnabled(bool))
            );
    connect(copy_checkBox_4,  SIGNAL(toggled(bool)),
            reciver_lineEd_4, SLOT(setEnabled(bool))
            );
    connect(copy_checkBox_5,  SIGNAL(toggled(bool)),
            reciver_lineEd_5, SLOT(setEnabled(bool))
            );

}
