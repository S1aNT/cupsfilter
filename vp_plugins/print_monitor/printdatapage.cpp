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
#include <QtCore/QPair>
#include <QSizePolicy>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QDataWidgetMapper>
#include <QPushButton>



#include "mytypes.h"
using namespace VPrn;
PrintDataPage::PrintDataPage(QWidget *parent)
    : QWidget(parent)    
    , first_split(false)
    , other_split(false)
    , m_model(0)
    , m_cards_model (0)
{
    QFont font;
    font.setFamily(QString::fromUtf8("Times New Roman"));
    font.setPointSize(14);
    font.setBold(true);
    //font.setWeight(75);

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

    this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::MinimumExpanding);
    this->setMinimumSize(320,240);
    //this->setMaximumSize(800,600);

    // Скрытые чекбоксы
    docConverted_checkBox = new QCheckBox(this);
    docConverted_checkBox->hide();
    this->setWindowTitle(QObject::trUtf8("Заполнение карточки документа"));

    QGridLayout *centralgridLayout = new QGridLayout(this);


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
    label_4->setText(QObject:: trUtf8("Номер МБ"));

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

    QLabel *label_tmplCB = new QLabel(commonGroupBox);
    label_tmplCB->setFont(font);
    label_tmplCB->setText(QObject::trUtf8("Шаблоны"));

    templatesCBox = new QComboBox(commonGroupBox);
    templatesCBox->setEnabled(true);

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
    gridLayout->addWidget(label_tmplCB,     6, 0, 1, 1);
    gridLayout->addWidget(templatesCBox,    7, 0, 1, 10);


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
    connector();
}

void PrintDataPage::setTemplatesModel ( QStandardItemModel *model)
{
    m_model = model;
    templatesCBox->setModel( m_model );
    templatesCBox->setModelColumn( VPrn::metaInfo_name );    
}

void PrintDataPage::setSecListModel(QStringListModel *m_list_model)
{
    secretCBox->setModel(m_list_model);
    secretCBox->setCurrentIndex(-1);
}

void PrintDataPage::setCardDocModel( QStandardItemModel *model)
{

    m_cards_model = model;
    qDebug() << "m_cards_model ROW:COL"
            << m_cards_model->rowCount()    <<  " : "
            << m_cards_model->columnCount();

    //Настройка маппера
    mapper->setModel(m_cards_model);

    mapper->addMapping(docName_lineEd,VPrn::cards_DOC_NAME);
    mapper->addMapping(secretCBox, VPrn::cards_STAMP, "currentIndex");
    mapper->addMapping(punktLineEd,VPrn::cards_PUNKT);
    mapper->addMapping(mbNumberLineEd,VPrn::cards_MB_NUMBER);
    mapper->addMapping(pagesCountLineEd,VPrn::cards_PAGE_COUNT);
    mapper->addMapping(e_currentSBox,VPrn::cards_CURRENT_COPY);
    mapper->addMapping(e_totalSBox,VPrn::cards_COPY_COUNT);
    mapper->addMapping(total_copyes,VPrn::cards_SELECT_ALL_COPY);
    mapper->addMapping(templatesCBox, VPrn::cards_TEMPLATE_NAME, "currentIndex");

    mapper->addMapping(executor_lineEd,VPrn::cards_EXECUTOR);
    mapper->addMapping(pressman_lineEd,VPrn::cards_PRINTMAN);
    mapper->addMapping(invNumber_lineEd,VPrn::cards_INV_NUMBER);
    mapper->addMapping(telephone_lineEd,VPrn::cards_PHONE);
    mapper->addMapping(dateField_dateEd,VPrn::cards_PRINT_DATE);

    mapper->addMapping(reciver_lineEd_1,VPrn::cards_RECIVER_1);
    mapper->addMapping(reciver_lineEd_2,VPrn::cards_RECIVER_2);
    mapper->addMapping(reciver_lineEd_3,VPrn::cards_RECIVER_3);
    mapper->addMapping(reciver_lineEd_4,VPrn::cards_RECIVER_4);
    mapper->addMapping(reciver_lineEd_5,VPrn::cards_RECIVER_5);

    // Грязный хак
    QObject::connect(m_cards_model, SIGNAL(itemChanged(QStandardItem*)),
                     mapper, SLOT (toLast())
                     );

}

void PrintDataPage::showIndex(int ind)
{
    QMessageBox msg;
    msg.setText(QObject::tr("Index %1").arg(ind,0,10));
    msg.exec();
}

void PrintDataPage::needRestart()
{
    m_cards_model->clear();
    templatesCBox->setCurrentIndex(-1);
    secretCBox->setCurrentIndex(-1);
}

bool PrintDataPage::enableNext()
{
    QMessageBox msgbox;
    bool Ok = true;
    {
        Ok &= !docName_lineEd->text().isEmpty();
        Ok &= !executor_lineEd->text().isEmpty();

        if (pressman_lineEd->text().isEmpty()){
            pressman_lineEd->setText(executor_lineEd->text());
            Ok &= pressman_lineEd->isModified();
        }

        if (secretCBox->currentIndex() == -1){
            Ok &= false;
        }

        if (templatesCBox->currentIndex() == -1){
            Ok &= false;
        }

        Ok &= !mbNumberLineEd->text().isEmpty();
        Ok &= !pagesCountLineEd->text().isEmpty();
        Ok &= !invNumber_lineEd->text().isEmpty();
        Ok &= !telephone_lineEd->text().isEmpty();
        Ok &= !dateField_dateEd->text().isEmpty();


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
        //mapper->submit();
    }
    return Ok;
}

bool PrintDataPage::checkReciver(int r)
{
    bool Ok = true;
    {
        switch (r){
        case 1:
            // Ok = reciver_lineEd_1->isModified();
            if (reciver_lineEd_1->text().isEmpty()){
                Ok &= false;
            }
            break;
        case 2:
            //Ok = reciver_lineEd_2->isModified();
            if (reciver_lineEd_2->text().isEmpty()){
                Ok &= false;
            }
            break;
        case 3:
            //Ok = reciver_lineEd_3->isModified();
            if (reciver_lineEd_3->text().isEmpty()){
                Ok &= false;
            }
            break;
        case 4:
            //Ok = reciver_lineEd_4->isModified();
            if (reciver_lineEd_4->text().isEmpty()){
                Ok &= false;
            }
            break;
        case 5:
            //Ok = reciver_lineEd_5->isModified();
            if (reciver_lineEd_5->text().isEmpty()){
                Ok &= false;
            }
            break;
        }
    }
    return Ok;
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
    /*
    int cur_copyes;
    int all_copyes;
    if (total_copyes->isChecked()){// стоит отметка все экз. пишем 0!!!
        cur_copyes = 0;
        all_copyes = 5;
    }else{
        cur_copyes = e_currentSBox->value();
        all_copyes = e_totalSBox->value();
    }


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
    //Запишем пару выбранный пользователем экземпляр,всего экземпляров
    out << qMakePair(cur_copyes,all_copyes);

    // Запишем выбранный пользователем шаблон
    if (t_fileName.isEmpty()){
        // Грязный хак. Причина: Комбобокс всегда выбирает первый элемент
        // и не высылает сигнал его снеы, так нечего менять (если шаблон всего один)
        findTemplatesFilePathInModel ( templatesCBox->currentIndex());
    }
    out << t_fileName;
    // save hash
    out << m_tagValue;

    return fields_data;
    */
}

void PrintDataPage::setDocConverted()
{
    docConverted_checkBox->setChecked(true);  
}


//-------------------------------- PRIVATE FUNCTIONS ---------------------------
void PrintDataPage::connector()
{
    // Запомним шаблон
    connect (templatesCBox, SIGNAL( currentIndexChanged(int) ),
             this,          SLOT  ( findTemplatesFilePathInModel(int) )
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

//--------------------------------- PRIVATE SLOTS ------------------------------
void PrintDataPage::findTemplatesFilePathInModel(int combo_ind)
{
    QModelIndex i_fname = m_model->index(combo_ind,VPrn::metaInfo_fname);
    t_fileName = m_model->data( i_fname, Qt::EditRole).toString() ;
}
