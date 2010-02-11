#include "addtemplate.h"
#include "ui_addtemplate.h"
#include "tinfoeditmodel.h"

#include <QDateTime>
#include <QMessageBox>
#include <QFileDialog>
#include <QtSql/QSqlQueryModel>
#include <QDataWidgetMapper>
#include "tech_global.h"

using namespace SafeVirtualPrinter;

AddTemplate::AddTemplate(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::AddTemplate)
{
    ui->setupUi(this);

    boldFont = QFont("Tahoma", 8, QFont::Bold);
    normalFont = QFont("Tahoma", 8, QFont::Normal);

    // Создамем мапперы
    pSizeDWMapper = new QDataWidgetMapper(this);
    tInfoDWMapper = new QDataWidgetMapper(this);

    this->connector();
}

void AddTemplate::connector()
{
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

    connect (ui->t_name_lineEd,
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

    connect(ui->pageSizeCBox,
            SIGNAL(currentIndexChanged(int)),
            pSizeDWMapper,
            SLOT(setCurrentIndex(int))
            );

    connect (ui->pageSizeCBox,
             SIGNAL(currentIndexChanged(QString)),
             this,
             SLOT(setCurrentPageSize(QString))
             );
}

void AddTemplate::default_init()
{

}


void AddTemplate::setEnableGUI(bool mode)
{
    work_mode = mode;

    QString text;
    ui->t_nameGBox->setEnabled(mode);
    ui->t_descGBox->setEnabled(mode);
    ui->page_orientGBox->setEnabled(mode);
    ui->page_sizeGBox->setEnabled(mode);
    ui->marginGBox->setEnabled(mode);
    if (!mode){
        text = tr("Закрыть");

    }else{
        text = tr("Отмена");
    }
    ui->addBtn->setVisible(mode);
    ui->cancelBtn->setText(text);
}

//************************************* public *********************************
void AddTemplate::setInfoModel(TemplateInfoEditModel *model)
{
    if (model){
        tInfoModel = model;
        //Настроим маппинг полей модели
        tInfoDWMapper->addMapping(ui->t_name_lineEd,tInfo_name);
    }
}

void AddTemplate::setPageSizeModel(QSqlQueryModel *model)
{
    if (model){
        pSizeModel = model;
        pSizeDWMapper->setModel(pSizeModel);
        //Заполним выпадающий список из модели
        ui->pageSizeCBox->setModel(pSizeModel);
        ui->pageSizeCBox->setModelColumn(pSize_page);

        //Настроим маппинг полей модели
        pSizeDWMapper->addMapping(ui->p_widthLEd,pSize_width);
        pSizeDWMapper->addMapping(ui->p_heightLEd,pSize_height);
        pSizeDWMapper->toFirst();
    }
}

//------------------------------------- end public------------------------------

/*
void AddTemplate::setTemplatesInfo(Templ_info templ_Info)
{
    tInfo = templ_Info;

    // Получили информацию о шаблоне загоним ее в нужные поля
    ui->t_name_lineEd->setText(tInfo.t_name());
    ui->descTextEdit->setPlainText(tInfo.t_desc());

    ui->portretBtn->setChecked(tInfo.page_orient());

    ui->pageSizeCBox->setCurrentIndex(ui->pageSizeCBox->findText(tInfo.p_size()));

    ui->author_lineEd->setText(tInfo.t_author());
    ui->date_lineEd->setText(tInfo.date_time());

    tInfo.setDate_time(QDateTime::currentDateTime ().toString("dd.MM.yyyy hh:mm:ss"));
    ui->date_lineEd->setText(tInfo.date_time());

    //FIXME надо подумать как правильнее делать
    // возникает замкнутый круг setValue меняет tInfo

    ui->margin_topSpBox->setValue(POINT_TO_MM(tInfo.m_top()));
    ui->margin_bottomSBox->setValue(POINT_TO_MM(tInfo.m_bottom()));
    ui->margin_leftSpBox->setValue(POINT_TO_MM(tInfo.m_left()));
    ui->margin_rightSpBox->setValue(POINT_TO_MM(tInfo.m_right()));
}

*/
/*
void AddTemplate::setUserName(const QString & name)
{
    //FIX: по идее это уже есть в инфо шаблона
    if (!name.isEmpty()){
        // Обновим имя пользователя
        tInfo.setT_author(name) ;
        ui->author_lineEd->setText(tInfo.t_author());
    }
}

void AddTemplate::setPageSize(QStringListModel *page_size_model)
{
    ui->pageSizeCBox->setModel(page_size_model);
    //ui->p_heightSpBox->setValue(page_height);
    //ui->p_widthSpBox->setValue(page_width);
}
*/

//-------------------------------------- protected slots
void AddTemplate::setMarginTop()
{
    //tInfo.setM_top(MM_TO_POINT(ui->margin_topSpBox->value()));
}
void AddTemplate::setMarginBottom()
{
    //tInfo.setM_bottom (MM_TO_POINT(ui->margin_bottomSBox->value()));
}
void AddTemplate::setMarginLeft()
{
    //tInfo.setM_left(MM_TO_POINT(ui->margin_leftSpBox->value()));
}
void AddTemplate::setMarginRight()
{
    //tInfo.setM_right(MM_TO_POINT(ui->margin_rightSpBox->value()));
}
void AddTemplate::accept()
{
    QString fileName;

    QString e_msg = QObject::trUtf8("Поле [ %1 ] не может быть пустым!");
    if (ui->t_name_lineEd->text().isEmpty()){
        e_msg.arg(QObject::trUtf8("Имя шаблона"));
        this->showInfo(e_msg);
    }else{
        fileName = QFileDialog::getSaveFileName(this, QObject::trUtf8("Сохранить шаблон как"),
                                                local_dir,
                                                tr("Шаблоны (*.tmpl *.TMPL)"));
        if (!fileName.isEmpty()){
            if (work_mode){
                emit needCreateEmptyTemplates(fileName);
                QDialog::accept();
            }else{
                QDialog::close();
            }
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
    // if (tInfo){
    //tInfo.setT_desc(ui->descTextEdit->toPlainText());
    // }

}

void AddTemplate::setTemplatesName(const QString & name)
{

    if (!name.isEmpty()){
        //if (tInfo){
        //tInfo.setT_name(name);
        //}
    }
}

void AddTemplate::setCurrentPageSize(const QString &psize)
{
    // Ищем ID по значению psize;
    int id = getIndexInPSizeModel(psize);
    if (id != -1){
        QModelIndex psizeIDIndex = tInfoModel->index(tInfoModel->rowCount(), tInfo_pageID);
        tInfoModel->setData(psizeIDIndex,id,Qt::EditRole);
    }
}

int AddTemplate::getIndexInPSizeModel(const QString pSizeHuman)
{
    QString header;
    QString fnd = tr("Размер листа");
    int rez= -1;
    if (!pSizeHuman.isEmpty()){
        for (int i=0; i<pSizeModel->columnCount(); i++){
            header = pSizeModel->headerData(i,Qt::Horizontal,
                                            Qt::DisplayRole).toString();
            if (header == fnd){
                // Цикл по всем элементам колонки Размер листа
                for(int j=0; j<pSizeModel->rowCount(); j++){
                    QModelIndex index = pSizeModel->index(j,i);
                    QString cell = pSizeModel->data(index,
                                                    Qt::DisplayRole).toString();
                    if (cell == pSizeHuman){
                        rez = j;
                        break;
                    }
                }
            }
        }
    }
    return rez;
}

void AddTemplate::set_portret()
{
    ui->p_orient_label->setFont(boldFont);
    ui->l_orient_label->setFont(normalFont);
    //if (tInfo){
    //tInfo.setPage_orient(true);
    //}

}

void AddTemplate::set_landscape()
{
    ui->p_orient_label->setFont(normalFont);
    ui->l_orient_label->setFont(boldFont);
    //if (tInfo){
    //tInfo.setPage_orient(false);
    //}
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
