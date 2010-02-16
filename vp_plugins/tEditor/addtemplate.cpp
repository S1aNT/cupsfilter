#include "addtemplate.h"
#include "ui_addtemplate.h"
#include "tinfoeditmodel.h"

#include <QDateTime>
#include <QMessageBox>
#include <QFileDialog>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlError>
#include <QtSql/QSqlTableModel>
#include <QDataWidgetMapper>
#include "mytypes.h"

using namespace VPrn;

AddTemplate::AddTemplate(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddTemplate)
    , pSizeModel(0)
    , tInfoModel(0)
    //, tInfoModel2(0)
{
    ui->setupUi(this);

    boldFont = QFont("Tahoma", 8, QFont::Bold);
    normalFont = QFont("Tahoma", 8, QFont::Normal);

    // Создамем мапперы
    pSizeDWMapper = new QDataWidgetMapper(this);
    pSizeDWMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);//QDataWidgetMapper::AutoSubmit);

    tInfoDWMapper = new QDataWidgetMapper(this);
    tInfoDWMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    this->connector();

}

void AddTemplate::connector()
{
    connect (ui->portretBtn,
             SIGNAL(clicked()),
             this,
             SLOT(setPortret())
             );
    connect (ui->landscapeBtn,
             SIGNAL(clicked()),
             this,
             SLOT(setLandscape())
             );

    /*
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
*/
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
    if (tInfoModel && pSizeModel){
        getData4Models();

        //tInfoDWMapper->setModel(tInfoModel);
        //Настроим маппинг полей модели
        //tInfoDWMapper->addMapping(ui->t_name_lineEd,tInfo_name);
        //tInfoDWMapper->addMapping(ui->descTextEdit,tInfo_desc);
        //tInfoDWMapper->toFirst();

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
    /*
    QModelIndex psizeIDIndex = tInfoModel->index(tInfoModel->rowCount()-1,tInfo_mtop);
    if (!tInfoModel->setData(psizeIDIndex,ui->margin_topSpBox->value(),Qt::EditRole)){
        emit error (SQLCommonError,
                    tr("При записи в поле [tInfo_mtop] произошла ошибка. %1")
                    .arg(tInfoModel->lastError().text()));
    }
    */
}
void AddTemplate::setMarginBottom()
{
    /*
    QModelIndex psizeIDIndex = tInfoModel->index(tInfoModel->rowCount()-1,tInfo_mbottom);
    if (!tInfoModel->setData(psizeIDIndex,ui->margin_bottomSBox->value(),Qt::EditRole)){
        emit error (SQLCommonError,
                    tr("При записи в поле [tInfo_mbottom] произошла ошибка. %1")
                    .arg(tInfoModel->lastError().text()));
    }
    */
}
void AddTemplate::setMarginLeft()
{/*
    QModelIndex psizeIDIndex = tInfoModel->index(tInfoModel->rowCount()-1,tInfo_mleft);
    if (!tInfoModel->setData(psizeIDIndex,ui->margin_leftSpBox->value(),Qt::EditRole)){
        emit error (SQLCommonError,
                    tr("При записи в поле [tInfo_mleft] произошла ошибка. %1")
                    .arg(tInfoModel->lastError().text()));
    }
    */
}
void AddTemplate::setMarginRight()
{
    /*
    QModelIndex psizeIDIndex = tInfoModel->index(tInfoModel->rowCount()-1,tInfo_mright);
    if (!tInfoModel->setData(psizeIDIndex,ui->margin_rightSpBox->value(),Qt::EditRole)){
        emit error (SQLCommonError,
                    tr("При записи в поле [tInfo_mright] произошла ошибка. %1")
                    .arg(tInfoModel->lastError().text()));
    }
    */
}
void AddTemplate::accept()
{
    QString fileName;
    this->setData4Models();

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


// ------------------------------------- private
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

void AddTemplate::getData4Models()
{
    if (tInfoModel && pSizeModel) {
        for (int i=0;i<tInfoModel->columnCount();i++){
            QModelIndex cellIndex = tInfoModel->index(tInfoModel->rowCount()-1,i);
            if (cellIndex.isValid()){
                QVariant cellData = tInfoModel->data(cellIndex,Qt::EditRole);
                {
                    bool cnv_ok;
                    qDebug() << "\nField N "  << i
                            << "\nQVariant " << cellData
                            << "\nQString "  << cellData.toString()
                            << "\nInt "      << cellData.toInt()
                            << "\nDateTime " << QDateTime::fromTime_t(cellData.toUInt())
                            << "\nQString.toInt() " << cellData.toString().toInt(&cnv_ok,10);
                }
                switch (i){
                case tInfo_name:
                    ui->t_name_lineEd->setText(cellData.toString());
                    break;
                case tInfo_desc:
                    ui->descTextEdit->setPlainText(cellData.toString());
                    break;
                case tInfo_angle:
                    if (cellData.toInt() == 0){
                        this->setPortret();
                    }else{
                        this->setLandscape();
                    }
                    break;
                case tInfo_ctime:
                    /*
                    qDebug() << Q_FUNC_INFO
                            << "cellData = " << cellData
                            << "cellData string = " << cellData.toDateTime().toString();
*/
                    {
                        QDateTime dt = QDateTime::fromTime_t(cellData.toUInt());
                        ui->cdate_lineEd->setText(dt.toString());
                    }
                    break;
                case tInfo_mtime:
                    {
                        QDateTime dt = QDateTime::fromTime_t(cellData.toUInt());
                        QString mt = dt.toString();

                        if (mt.isEmpty()){
                            mt = QDateTime::currentDateTime().toString();
                        }
                        //qDebug() << Q_FUNC_INFO << mt;
                        ui->mdate_lineEd->setText(mt);
                    }
                    break;
                case tInfo_author:
                    ui->author_lineEd->setText(cellData.toString());
                    break;
                case tInfo_mtop:
                    /*
                    qDebug() << Q_FUNC_INFO
                            << "cellData = " << cellData
                            << cellData.toString().toInt();
                            */
                    ui->margin_topSpBox->setValue(cellData.toString().toInt());
                    break;
                case tInfo_mbottom:
                    ui->margin_bottomSBox->setValue(cellData.toInt());
                    break;
                case tInfo_mleft:
                    ui->margin_leftSpBox->setValue(cellData.toInt());
                    break;
                case tInfo_mright:
                    ui->margin_rightSpBox->setValue(cellData.toInt());
                    break;
                }
            }
        }
    }
}

void AddTemplate::setData4Models()
{
    if (tInfoModel && pSizeModel) {
        for (int i=0;i<tInfoModel->columnCount();i++){
            QModelIndex cellIndex = tInfoModel->index(tInfoModel->rowCount()-1,i);
            if (cellIndex.isValid()){
                QVariant cellData;// = tInfoModel->data(cellIndex,Qt::EditRole);
                switch (i){
                case tInfo_name:
                    cellData = ui->t_name_lineEd->text();
                    break;
                case tInfo_desc:
                    cellData = ui->descTextEdit->toPlainText();
                    break;
                case tInfo_angle:
                    if (ui->portretBtn->isChecked()){
                        cellData = 0;
                    }else{
                        cellData =90;
                    }
                    break;
                case tInfo_pageID:
                    cellData = currentPSizeId;
                    break;
                case tInfo_mtime:
                    cellData = QDateTime::currentDateTime().toTime_t();
                    break;
                case tInfo_author:
                    cellData = ui->author_lineEd->text();
                    break;
                case tInfo_mtop:
                    cellData = ui->margin_topSpBox->value();
                    break;
                case tInfo_mbottom:
                    cellData = ui->margin_bottomSBox->value();
                    break;
                case tInfo_mleft:
                    cellData = ui->margin_leftSpBox->value();
                    break;
                case tInfo_mright:
                    cellData = ui->margin_rightSpBox->value();
                    break;
                }
                if (cellData.isValid()){
                    if (!tInfoModel->setData(cellIndex,cellData,Qt::EditRole)){
                        emit error (SQLCommonError,
                                    tr("При записи в поле [%1] произошла ошибка. %2")
                                    .arg(i).arg(tInfoModel->lastError().text()));
                    }
                }
            }
        }
    }
}

// ------------------------------------- private slots

// -------------------------------------- сеттеры ------------------------------

void AddTemplate::setTemplatesDesc()
{ 
    /*
    QString desc = ui->descTextEdit->toPlainText();

    if (!desc.isEmpty()){
        QModelIndex psizeIDIndex = tInfoModel->index(tInfoModel->rowCount()-1,tInfo_desc);
        if (!tInfoModel->setData(psizeIDIndex,desc,Qt::EditRole)){
            emit error (SQLCommonError,
                        tr("При записи в поле [tInfo_desc] произошла ошибка. %1")
                        .arg(tInfoModel->lastError().text()));
        }
    }
    */
}

void AddTemplate::setTemplatesName(const QString & name)
{
    /*
    if (!name.isEmpty()){
        QModelIndex psizeIDIndex = tInfoModel->index(tInfoModel->rowCount()-1,tInfo_name);
        if (!tInfoModel->setData(psizeIDIndex,name,Qt::EditRole)){
            emit error (SQLCommonError,
                        tr("При записи в поле [tInfo_name] произошла ошибка. %1")
                        .arg(tInfoModel->lastError().text()));
        }
    }
    */
}

void AddTemplate::setCurrentPageSize(const QString &psize)
{
    // Ищем ID по значению psize;
    currentPSizeId = getIndexInPSizeModel(psize);
    /*
    if (id != -1){
        qDebug() << Q_FUNC_INFO
                << "tInfoModel->rowCount() = " << tInfoModel->rowCount()
                << "tInfo_pageID = " << tInfo_pageID;

        QModelIndex psizeIDIndex = tInfoModel->index(tInfoModel->rowCount()-1,tInfo_pageID);
        if (!tInfoModel->setData(psizeIDIndex,id,Qt::EditRole)){
            emit error (SQLCommonError,
                        tr("При записи в поле [tInfo_pageID] произошла ошибка. %1")
                        .arg(tInfoModel->lastError().text()));
        }
    }
    */
}

void AddTemplate::setPortret()
{
    ui->p_orient_label->setFont(boldFont);
    ui->l_orient_label->setFont(normalFont);
    /*
    QModelIndex psizeIDIndex = tInfoModel->index(tInfoModel->rowCount()-1,tInfo_angle);
    if (!tInfoModel->setData(psizeIDIndex,0,Qt::EditRole)){
        emit error (SQLCommonError,
                    tr("При записи в поле [tInfo_angle] произошла ошибка. %1")
                    .arg(tInfoModel->lastError().text()));
    }
    */
}

void AddTemplate::setLandscape()
{
    ui->p_orient_label->setFont(normalFont);
    ui->l_orient_label->setFont(boldFont);
    /*
    QModelIndex psizeIDIndex = tInfoModel->index(tInfoModel->rowCount()-1,tInfo_angle);
    if (!tInfoModel->setData(psizeIDIndex,90,Qt::EditRole)){
        emit error (SQLCommonError,
                    tr("При записи в поле [tInfo_angle] произошла ошибка. %1")
                    .arg(tInfoModel->lastError().text()));
    }
    */
}

//------------------------------------------------------------------------------
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
                        //break;
                        return rez;
                    }
                }
            }

        }
    }
    return rez;
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
