#include "addtemplate.h"
#include "ui_addtemplate.h"
#include <QDateTime>
#include <QMessageBox>
#include <QFileDialog>

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


//-------------------------------------- protected slots
void AddTemplate::accept()
{
    QString fileName;
    QString new_templates_name = QObject::tr("%1/new_template.tmpl").arg(qApp->applicationDirPath());
    QString e_msg = QObject::trUtf8("Поле [ %1 ] не может быть пустым!");
    if (ui->name_lineEd->text().isEmpty()){
        e_msg.arg(QObject::trUtf8("Имя шаблона"));
        this->showInfo(e_msg);
    }else{
        fileName = QFileDialog::getSaveFileName(this, QObject::trUtf8("Сохранить шаблон как"),
                                    new_templates_name,
                                    tr("Шаблоны (*.tmpl *.TMPL)"));
        if (!fileName.isEmpty()){
            QDialog::accept();
        }
    }
}

// ------------------------------------- protected
void AddTemplate::showInfo(const QString & info)
{
    QMessageBox msgBox;
    QPushButton *abortButton;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle(QObject::trUtf8("Информационное сообщение"));
    abortButton=msgBox.addButton(QObject::trUtf8("Выход"), QMessageBox::RejectRole);
    msgBox.setText(info);
    msgBox.exec();
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
