#include "wizardpages.h"

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
#include <QtGui/QLCDNumber>
#include <QtGui/QSpinBox>
#include <QtGui/QDateEdit>

IntroPage::IntroPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(QObject::trUtf8("Инициализация..."));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/watermark.png"));

    topLabel = new QLabel(QObject::trUtf8("Этот мастер поможет Вам сделать <i>твердую копию</i> "
                                          "документа используя <b>Защищенный виртуальный принтер</b>&trade")
                          );
    topLabel->setWordWrap(true);

    loadPlugin_step          = new QCheckBox(this);
    loadPlugin_step->setText(QObject::trUtf8("Загрузка плагинов"));
    loadPlugin_step->setEnabled(false);

    connect2LocalServer_step = new QCheckBox(this);
    connect2LocalServer_step->setText(QObject::trUtf8("Установление связи с GateKeeper"));
    connect2LocalServer_step->setEnabled(false);

    connect2RemoteDemon_step = new QCheckBox(this);
    connect2RemoteDemon_step->setText(QObject::trUtf8("Установление связи с СУРД"));
    connect2RemoteDemon_step->setEnabled(false);

    reciveUserName_step      = new QCheckBox(this);
    reciveUserName_step->setText(QObject::trUtf8("Получение логина пользователя"));
    reciveUserName_step->setEnabled(false);

    reciveUserMandat_step    = new QCheckBox(this);
    reciveUserMandat_step->setText(QObject::trUtf8("Получение мандата пользователя"));
    reciveUserMandat_step->setEnabled(false);

    reciveSecLabel_step      = new QCheckBox(this);
    reciveSecLabel_step->setText(QObject::trUtf8("Получение списка уровней секретности связанных с мандатом"));
    reciveSecLabel_step->setEnabled(false);

    recivePrintersList_step  = new QCheckBox(this);
    recivePrintersList_step->setText(QObject::trUtf8("Получение списка принтеров, доступных пользователю"));
    recivePrintersList_step->setEnabled(false);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(topLabel);
    layout->addSpacing(3);
    layout->addWidget(loadPlugin_step);
    layout->addWidget(connect2LocalServer_step);
    layout->addWidget(connect2RemoteDemon_step);
    layout->addWidget(reciveUserName_step);
    layout->addWidget(reciveUserMandat_step);
    layout->addWidget(reciveSecLabel_step);
    layout->addWidget(recivePrintersList_step);
    setLayout(layout);
    // Регистрируем  поля
    registerField("loadPlugin*",          loadPlugin_step);
    registerField("connect2LocalServer*", connect2LocalServer_step);
    registerField("connect2RemoteDemon*", connect2RemoteDemon_step);
    registerField("reciveUserName*",      reciveUserName_step);
    registerField("reciveUserMandat*",    reciveUserMandat_step);
    registerField("reciveSecLabel*",      reciveSecLabel_step);
    registerField("recivePrintersList*",  recivePrintersList_step);
}


void IntroPage::setPluginsLoad()
{
    loadPlugin_step->setChecked(true);
}

void IntroPage::setLocalSrvRegistr()
{
    connect2LocalServer_step->setChecked(true);
}

void IntroPage::setRemoteDemonRegistr()
{
    connect2RemoteDemon_step->setChecked(true);
}

void IntroPage::setReciveUserName()
{
    reciveUserName_step->setChecked(true);
}

void IntroPage::setReciveUserMandat()
{
    reciveUserMandat_step->setChecked(true);
}

void IntroPage::setRecivePrintersList()
{
    recivePrintersList_step->setChecked(true);
}

void  IntroPage::setReciveSecLevelList()
{
    reciveSecLabel_step->setChecked(true);
}

SelectPage::SelectPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(QObject::trUtf8("Выбор режима работы ..."));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/select_mode.png"));


    topLabel = new QLabel(QObject::trUtf8("На этом шаге необхоимо выбрать Принтер и один из режимов работы:"
                                          "<ul><li>Пометка зарегистрованного документа как БРАК</li>"
                                          "<li>Учет листов в БД</li><li>Печать на учтенных листах</li>"
                                          "<li>Печать с автоматическим учетом листов</li></ul>"
                                          ));
    topLabel->setWordWrap(true);

    printersCBox = new QComboBox(this);

    markBrakDoc = new QRadioButton(this);
    markBrakDoc->setText(QObject::trUtf8("Пометка зарегистрированного и распечатанного документа как брак"));
    markBrakDoc->setEnabled(false);
    accountingDoc = new QRadioButton(this);
    accountingDoc->setText(QObject::trUtf8("Учет листов в БД"));

    printDoc      = new QRadioButton(this);
    printDoc->setText(QObject::trUtf8("Печать на учтенных листах"));

    both_step      = new QRadioButton(this);
    both_step->setText(QObject::trUtf8("Печать на учтенных листах, с автоматичеcкой регистрацией в БД"));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(topLabel);
    layout->addSpacing(3);
    layout->addWidget(printersCBox);
    layout->addWidget(markBrakDoc);
    layout->addWidget(accountingDoc);
    layout->addWidget(printDoc);
    layout->addWidget(both_step);

    setLayout(layout);    
    registerField("markBrakDoc",  markBrakDoc);
    registerField("accountingDoc", accountingDoc);
    registerField("printDoc",printDoc);
    registerField("both_step",both_step);
}

void SelectPage::setPrinterList(const QString &plist)
{
    QStringList remote_printer;
    QStringList devices_info;
    QString pline;
    remote_printer = plist.split("###;:;");
    for (int i = 0; i < remote_printer.size(); i++) {

        pline = remote_printer.at(i);
        pline.replace("###","");
        devices_info = pline.split(";:;");
        printer_device_list.insert(devices_info.at(0),devices_info.at(1));
        pline = devices_info.at(0);
        printersCBox->addItem(pline.section(".",1,1));// Имя принтера после точки

    }

}

PrintDataPage::PrintDataPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(QObject::trUtf8("Режим работы:"));
    QGridLayout *centralgridLayout = new QGridLayout(this);

    QFont font;
    font.setFamily(QString::fromUtf8("Times New Roman"));
    font.setPointSize(12);
    font.setBold(true);
    font.setWeight(75);

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

    e_numberSBox_2 = new QSpinBox(commonGroupBox);
    e_numberSBox_2->setMinimum(1);
    e_numberSBox_2->setMaximum(5);

    e_numberSBox = new QSpinBox(commonGroupBox);
    e_numberSBox->setMinimum(1);
    e_numberSBox->setMaximum(5);

    QLabel *label_2 = new QLabel(commonGroupBox);
    label_2->setFont(font);
    label_2->setAlignment(Qt::AlignCenter);
    label_2->setText(QObject::trUtf8("Всего"));

    docConverted_checkBox = new QCheckBox(commonGroupBox);
    docConverted_checkBox->setFont(font);
    docConverted_checkBox->setChecked(false);
    docConverted_checkBox->setEnabled(false);
    docConverted_checkBox->setText(QObject::trUtf8("Конвертация документа в pdf...set"));


    gridLayout->addWidget(label,            0, 0, 1, 1);
    gridLayout->addWidget(secretCBox,       0, 1, 1, 9);
    gridLayout->addWidget(label_10,         4, 0, 1, 2);
    gridLayout->addWidget(e_numberSBox_2,   4, 2, 1, 1);
    gridLayout->addWidget(e_numberSBox,     4, 9, 1, 1);
    gridLayout->addWidget(label_2,          4, 6, 1, 1);
    gridLayout->addWidget(label_3,          6, 0, 1, 1);
    gridLayout->addWidget(punktLineEd,      6, 4, 1, 6);
    gridLayout->addWidget(label_4,          7, 0, 1, 3);
    gridLayout->addWidget(mbNumberLineEd,   7, 4, 1, 6);
    gridLayout->addWidget(label_15,         8, 0, 1, 6);
    gridLayout->addWidget(pagesCountLineEd, 8, 8, 1, 2);
    gridLayout->addWidget(docConverted_checkBox, 9, 0, 1, 9);

    QGroupBox *lastPageGroupBoxChecked = new QGroupBox(this);
    lastPageGroupBoxChecked->setTitle(QObject::trUtf8("Штамп последней страницы"));
    QGridLayout *gridLayout_2 = new QGridLayout(lastPageGroupBoxChecked);

    QLabel *label_5 = new QLabel(lastPageGroupBoxChecked);
    label_5->setFont(font);
    label_5->setText(QObject::trUtf8("Исполнитель"));

    executor_lineEd = new QLineEdit(lastPageGroupBoxChecked);
    executor_lineEd->setEnabled(true);

    QLabel *label_6 = new QLabel(lastPageGroupBoxChecked);
    label_6->setFont(font);
    label_6->setText(QObject::trUtf8("Отпечатал"));

    pressman_lineEd = new QLineEdit(lastPageGroupBoxChecked);
    pressman_lineEd->setEnabled(true);

    QLabel *label_7 = new QLabel(lastPageGroupBoxChecked);
    label_7->setFont(font);
    label_7->setText(QObject::trUtf8("Телефон"));

    QLabel *label_8 = new QLabel(lastPageGroupBoxChecked);
    label_8->setFont(font);
    label_8->setText(QObject::trUtf8("Инв. №"));

    QLabel *label_9 = new QLabel(lastPageGroupBoxChecked);
    label_9->setFont(font);
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

    QGroupBox *middleGroupBox = new QGroupBox(this);
    middleGroupBox->setTitle(QObject::trUtf8("Шаблоны"));
    QHBoxLayout *hBoxLayout_2 = new QHBoxLayout(middleGroupBox);

    templatesCBox = new QComboBox(middleGroupBox);
    templatesCBox->setEnabled(true);
    hBoxLayout_2->addWidget(templatesCBox);

    QGroupBox *reciversListGroupBoxChecked = new QGroupBox(this);
    //reciversListGroupBoxChecked->setGeometry(QRect(7, 307, 659, 151));
    reciversListGroupBoxChecked->setCheckable(false);
    reciversListGroupBoxChecked->setChecked(false);
    reciversListGroupBoxChecked->setTitle(QObject::trUtf8("Список рассылки"));

    QGridLayout *gridLayout_4 = new QGridLayout(reciversListGroupBoxChecked);

    reciverOne_lineEd = new QLineEdit(reciversListGroupBoxChecked);
    reciverOne_lineEd->setEnabled(true);

    reciverTwo_lineEd = new QLineEdit(reciversListGroupBoxChecked);
    reciverTwo_lineEd->setEnabled(false);

    reciverThree_lineEd = new QLineEdit(reciversListGroupBoxChecked);
    reciverThree_lineEd->setEnabled(false);

    reciverFour_lineEd = new QLineEdit(reciversListGroupBoxChecked);
    reciverFour_lineEd->setEnabled(false);

    reciverFive_lineEd = new QLineEdit(reciversListGroupBoxChecked);
    reciverFive_lineEd->setEnabled(false);

    copy_checkBox_1 = new QCheckBox(reciversListGroupBoxChecked);
    copy_checkBox_1->setFont(font);
    copy_checkBox_1->setText(QObject::trUtf8("Экземпляр №1"));

    copy_checkBox_2 = new QCheckBox(reciversListGroupBoxChecked);
    copy_checkBox_2->setFont(font);
    copy_checkBox_2->setText(QObject::trUtf8("Экземпляр №2"));

    copy_checkBox_3 = new QCheckBox(reciversListGroupBoxChecked);
    copy_checkBox_3->setFont(font);
    copy_checkBox_3->setText(QObject::trUtf8("Экземпляр №3"));

    copy_checkBox_4 = new QCheckBox(reciversListGroupBoxChecked);
    copy_checkBox_4->setFont(font);
    copy_checkBox_4->setText(QObject::trUtf8("Экземпляр №4"));

    copy_checkBox_5 = new QCheckBox(reciversListGroupBoxChecked);
    copy_checkBox_5->setFont(font);
    copy_checkBox_5->setText(QObject::trUtf8("Экземпляр №5"));

    gridLayout_4->addWidget(copy_checkBox_1, 0, 0, 1, 1);
    gridLayout_4->addWidget(copy_checkBox_2, 2, 0, 1, 1);
    gridLayout_4->addWidget(copy_checkBox_3, 3, 0, 1, 1);
    gridLayout_4->addWidget(copy_checkBox_4, 4, 0, 1, 1);
    gridLayout_4->addWidget(copy_checkBox_5, 5, 0, 1, 1);

    gridLayout_4->addWidget(reciverOne_lineEd,  0, 1, 1, 1);
    gridLayout_4->addWidget(reciverTwo_lineEd,  2, 1, 1, 1);
    gridLayout_4->addWidget(reciverThree_lineEd,3, 1, 1, 1);
    gridLayout_4->addWidget(reciverFour_lineEd, 4, 1, 1, 1);
    gridLayout_4->addWidget(reciverFive_lineEd, 5, 1, 1, 1);

    centralgridLayout->addWidget( centralGroupBox             ,0,0,1,3);
    centralgridLayout->addWidget( commonGroupBox              ,1,0,1,1);
    centralgridLayout->addWidget( lastPageGroupBoxChecked     ,1,1,1,1 );
    centralgridLayout->addWidget( middleGroupBox              ,2,0,1,3);
    centralgridLayout->addWidget( reciversListGroupBoxChecked ,3,0,1,3 );

    setLayout(centralgridLayout);
    registerField("pagesCountLineEd*",  pagesCountLineEd);
    registerField("docConverted_checkBox*",docConverted_checkBox);
}

void PrintDataPage::setLabelText(const QString &l_txt)
{
    setSubTitle (l_txt);
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

void PrintDataPage::setDocConverted()
{

}

void PrintDataPage::setPageCounted(int pages)
{

}
