#include "addtemplate.h"
#include "ui_addtemplate.h"
#include <QDateTime>


AddTemplate::AddTemplate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTemplate)
{
    ui->setupUi(this);
    ui->date_lineEd->setText(QDateTime::currentDateTime ().toString("dd.MM.yyyy hh:mm:ss"));
    boldFont = QFont("Times", 10, QFont::Bold);
    normalFont = QFont("Times", 10, QFont::Normal);

    connect (ui->portretBtn,SIGNAL(clicked()),this,SLOT(set_portret()));
    connect (ui->landscapeBtn,SIGNAL(clicked()),this,SLOT(set_landscape()));
    connect (ui->pageSizeCBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(setCurrentPageSize(QString)));
}

void AddTemplate::setUserName(const QString & name)
{
    if (!name.isEmpty()){
        ui->author_lineEd->setText(name);
    }
}

void AddTemplate::setPageSize(QStringListModel *page_size_model)
{
    ui->pageSizeCBox->setModel(page_size_model);
    //ui->p_heightSpBox->setValue(page_height);
    //ui->p_widthSpBox->setValue(page_width);
}

// --------------------------------------Гетеры
QString AddTemplate::getTemplName()
{

}

QString AddTemplate::getTemplDesc()
{

}

QString AddTemplate::getPageSize()
{

}

QString AddTemplate::getCreationDate()
{

}

qreal AddTemplate::getMarginTop()
{

}

qreal AddTemplate::getMarginBottom()
{

}

qreal AddTemplate::getMarginLeft()
{

}

qreal AddTemplate::getMarginRight()
{

}

bool AddTemplate::getPageOrient()
{

}

// ------------------------------------- private slots

void AddTemplate::setCurrentPageSize(const QString &psize)
{
    page_size =psize;
}

void AddTemplate::set_portret()
{
    ui->portretBtn->setFont(boldFont);
    ui->landscapeBtn->setFont(normalFont);
}

void AddTemplate::set_landscape()
{
    ui->portretBtn->setFont(normalFont);
    ui->landscapeBtn->setFont(boldFont);
}

AddTemplate::~AddTemplate()
{
    delete ui;
}

void AddTemplate::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
