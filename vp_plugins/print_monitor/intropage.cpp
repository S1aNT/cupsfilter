#include "intropage.h"

#include <QtGui/QLabel>
#include <QtGui/QCheckBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QPixmap>

#include "mytypes.h"
using namespace VPrn;

IntroPage::IntroPage(QWidget *parent)
    : QWidget(parent)
{
    this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::MinimumExpanding);
    this->setWindowTitle(QObject::trUtf8("Инициализация подсистем Защищенного принтера."));
    QHBoxLayout *horizontalLayout = new QHBoxLayout();
    QVBoxLayout *verticalLayout = new QVBoxLayout();
    //Формируем левую часть блока
    QLabel *label = new QLabel();
    label->setPixmap(QPixmap(":/watermark.png"));

    horizontalLayout->addWidget(label);    
    horizontalLayout->addStretch(0);
    horizontalLayout->addLayout(verticalLayout);
    //Формируем правую часть блока
    QLabel *topLabel = new QLabel(QObject::trUtf8("Этот мастер поможет Вам сделать <i>твердую копию</i> "
                                                  "документа используя <b>Защищенный виртуальный принтер</b>&trade")
                                  );
    topLabel->setWordWrap(true);

    inputFile_step          = new QCheckBox(this);
    inputFile_step->setText(QObject::trUtf8("Задан файл для печати"));
    inputFile_step->setEnabled(false);

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


    verticalLayout->addStretch(0);
    verticalLayout->addWidget(topLabel);
    verticalLayout->addSpacing(3);

    verticalLayout->addWidget(inputFile_step);
    //verticalLayout->addStretch(0);
    verticalLayout->addWidget(connect2LocalServer_step);
    //verticalLayout->addStretch(0);
    verticalLayout->addWidget(connect2RemoteDemon_step);
    //verticalLayout->addStretch(0);
    verticalLayout->addWidget(reciveUserName_step);
    //verticalLayout->addStretch(0);
    verticalLayout->addWidget(reciveUserMandat_step);
    //verticalLayout->addStretch(0);
    verticalLayout->addWidget(reciveSecLabel_step);
    //verticalLayout->addStretch(0);
    verticalLayout->addWidget(recivePrintersList_step);
    verticalLayout->addStretch(0);
    setLayout(horizontalLayout);

}


bool IntroPage::enableNext()
{
    bool Ok = true;
    {
        Ok &= inputFile_step->isChecked();
        Ok &= connect2LocalServer_step->isChecked();
        Ok &= connect2RemoteDemon_step->isChecked();
        Ok &= reciveUserName_step->isChecked();
        Ok &= reciveUserMandat_step->isChecked();
        Ok &= recivePrintersList_step->isChecked();
        Ok &= reciveSecLabel_step->isChecked();
    }
    return Ok;
}

void IntroPage::setInputFile()
{
    inputFile_step->setChecked(true);
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

