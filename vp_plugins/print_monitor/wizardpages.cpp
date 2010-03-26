#include "wizardpages.h"

#include <QtGui/QLabel>
#include <QtGui/QCheckBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QRadioButton>

IntroPage::IntroPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(QObject::trUtf8("Инициализация..."));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/watermark.png"));

    topLabel = new QLabel(QObject::trUtf8("Этот мастер поможет Вам сделать <i>твердую копию</i> "
                             "документа используя <b>Защищенный виртуальный принтер</b>&trade")
                          );
    topLabel->setWordWrap(true);


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

int IntroPage::nextId() const
{
    bool Ok = loadPlugin_step->isChecked() &&
              connect2LocalServer_step->isChecked() &&
              connect2RemoteDemon_step->isChecked() &&
              reciveUserName_step->isChecked() &&
              reciveUserMandat_step->isChecked() &&
              reciveSecLabel_step->isChecked() &&
              recivePrintersList_step->isChecked();

    if (Ok) {
        return VPrn::Page_Select;
    } else {
        return -1;
    }
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
    topLabel = new QLabel(QObject::trUtf8("На этом шаге необхоимо выбрать один режимов работы:"
                             "<ul><li>Учет листов в БД</li><li>Печать на учтенных листах</li>"
                             "<li>Печать с автоматическим учетом листов</li>"
                             "<li>Пометка зарегистрованного документа как БРАК</li></ul>"
                             ));
    topLabel->setWordWrap(true);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(topLabel);
    layout->addSpacing(3);
    setLayout(layout);
}

int SelectPage::nextId() const
{
    return VPrn::Page_Select;
}
