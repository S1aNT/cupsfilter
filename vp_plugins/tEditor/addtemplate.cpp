#include "addtemplate.h"
#include "ui_addtemplate.h"
#include <QDateTime>
#include <QMessageBox>
#include <QFileDialog>

#define MM_TO_POINT(mm) ((mm)*2.83465058)

AddTemplate::AddTemplate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTemplate)
{
    ui->setupUi(this);
    this->creation_date = QDateTime::currentDateTime ().toString("dd.MM.yyyy hh:mm:ss");
    ui->date_lineEd->setText(creation_date);
    //FIXME не работает обновление шрифта bold -> normal

    boldFont = QFont("Times", 14, QFont::Bold);
    normalFont = QFont("Times", 14, QFont::Normal);

    /*
    // Значения по умолчанию
    page_size = QString("A4 (210 x 297 mm)");
    templ_name = QObject::trUtf8("Новый шаблон");
    templ_desc = QObject::trUtf8("Описание шаблона");

    page_height   = MM_TO_POINT(297);
    page_width    = MM_TO_POINT(210);
    margin_top    = MM_TO_POINT(10);
    margin_bottom = MM_TO_POINT(10);
    margin_left   = MM_TO_POINT(35);
    margin_right  = MM_TO_POINT(15);

    page_orient = true;
*/
    connect (ui->portretBtn,
             SIGNAL(clicked()),
             this,
             SLOT(set_portret())
             );
    connect (ui->landscapeBtn,
             SIGNAL(clicked()),
             this,
             SLOT(set_landscape())
             );
    connect (ui->pageSizeCBox,
             SIGNAL(currentIndexChanged(QString)),
             this,
             SLOT(setCurrentPageSize(QString))
             );
    connect (ui->name_lineEd,
             SIGNAL(textChanged(QString)),
             this,
             SLOT(setTemplatesName(QString))
             );
    connect (ui->descTextEdit,
             SIGNAL(textChanged()),
             this,
             SLOT(setTemplatesDesc())
             );

    connect (ui->margin_topSpBox,
             SIGNAL(editingFinished()),
             this,
             SLOT(setMarginTop())
             );
    connect (ui->margin_bottomSBox,
             SIGNAL(editingFinished()),
             this,
             SLOT(setMarginBottom())
             );
    connect (ui->margin_leftSpBox,
             SIGNAL(editingFinished()),
             this,
             SLOT(setMarginLeft())
             );
    connect (ui->margin_rightSpBox,
             SIGNAL(editingFinished()),
             this,
             SLOT(setMarginRight())
             );

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
void AddTemplate::setMarginTop()
{
    margin_top = MM_TO_POINT(ui->margin_topSpBox->value());
}
void AddTemplate::setMarginBottom()
{
    margin_bottom = MM_TO_POINT(ui->margin_bottomSBox->value());
}
void AddTemplate::setMarginLeft()
{
    margin_left = MM_TO_POINT(ui->margin_leftSpBox->value());
}
void AddTemplate::setMarginRight()
{
    margin_right = MM_TO_POINT(ui->margin_rightSpBox->value());
}
void AddTemplate::accept()
{
    QString fileName;

    QString e_msg = QObject::trUtf8("Поле [ %1 ] не может быть пустым!");
    if (ui->name_lineEd->text().isEmpty()){
        e_msg.arg(QObject::trUtf8("Имя шаблона"));
        this->showInfo(e_msg);
    }else{
        //TODO сделать транслитерацию имени шаблона или оставить по русски для записи его в файл?
        QString new_templates_name = QObject::trUtf8("%1/%2.tmpl").arg(local_dir,templ_name);
        fileName = QFileDialog::getSaveFileName(this, QObject::trUtf8("Сохранить шаблон как"),
                                    new_templates_name,
                                    tr("Шаблоны (*.tmpl *.TMPL)"));
        if (!fileName.isEmpty()){
            //QMessageBox::information(this,"file",fileName);
            QString user =  ui->author_lineEd->text();
            emit needCreateEmptyTemplates(fileName,user,templ_name,templ_desc,
                                          page_size,
                                          page_orient,creation_date,
                                          margin_top,margin_bottom,
                                          margin_right,margin_left);
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

// -------------------------------------- сеттеры ------------------------------

void AddTemplate::setTemplatesDesc()
{
    templ_desc = ui->descTextEdit->toPlainText();
}

void AddTemplate::setTemplatesName(const QString & name)
{
    if (!name.isEmpty()){
        templ_name = name;
    }
}

void AddTemplate::setCurrentPageSize(const QString &psize)
{
    page_size =psize;
}

void AddTemplate::set_portret()
{
    ui->portretBtn->setFont(boldFont);
    ui->landscapeBtn->setFont(normalFont);
    this->page_orient = true;
}

void AddTemplate::set_landscape()
{
    ui->portretBtn->setFont(normalFont);
    ui->landscapeBtn->setFont(boldFont);
    this->page_orient = false;
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
