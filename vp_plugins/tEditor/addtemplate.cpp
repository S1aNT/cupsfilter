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
#include <QTableView>

#include "mytypes.h"

void createView(const QString &title, QAbstractItemModel * model )
{
    static int offset = 0;

    QTableView *view = new QTableView;
    view->setModel(model);
    view->setWindowTitle(title);
    view->move(100 + offset, 100 + offset);
    view->verticalHeader()->setVisible(true);
    offset += 20;
    view->show();
}

using namespace VPrn;

AddTemplate::AddTemplate(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddTemplate)
    , pSizeModel(0)
    , tInfoModel(0)
{
    ui->setupUi(this);

    boldFont = QFont("Tahoma", 8, QFont::Bold);
    normalFont = QFont("Tahoma", 8, QFont::Normal);

    // Создамем мапперы
    //pSizeDWMapper = new QDataWidgetMapper(this);
    //pSizeDWMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);//QDataWidgetMapper::AutoSubmit);

    //tInfoDWMapper = new QDataWidgetMapper(this);
    //tInfoDWMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

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
    connect(ui->pageSizeCBox,
            SIGNAL(currentIndexChanged(int)),
            pSizeDWMapper,
            SLOT(setCurrentIndex(int))
            );
*/
    connect (ui->pageSizeCBox,
             SIGNAL(currentIndexChanged(QString)),
             this,
             SLOT(setCurrentPageSize(QString))
             );
}


void AddTemplate::default_init()
{   
    if (tInfoModel && pSizeModel){

        //createView(QObject::tr("PageSize Query Model"), pSizeModel);

        //pSizeDWMapper->setModel(pSizeModel);
        //Заполним выпадающий список из модели
        ui->pageSizeCBox->setModel(pSizeModel);
        ui->pageSizeCBox->setModelColumn(pSize_page);
        ui->pageSizeCBox->setCurrentIndex(-1);

        //Настроим маппинг полей модели
        //pSizeDWMapper->addMapping(ui->p_widthLEd,pSize_width);
        //pSizeDWMapper->addMapping(ui->p_heightLEd,pSize_height);
        //pSizeDWMapper->toFirst();
        getData4Models();
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


//-------------------------------------- protected slots

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

                switch (i){
                case tInfo_pageID:
                    {
                        bool cnv_ok;
                        qDebug() << "\nField N "  << i
                                 << "\nQVariant " << cellData
                                 << "\nQString "  << cellData.toString()
                                 << "\nInt "      << cellData.toInt()
                                 << "\nDateTime " << QDateTime::fromTime_t(cellData.toUInt())
                                 << "\nQString.toInt() " << cellData.toString().toInt(&cnv_ok,10);
                        int indx = translatePSizeID2CBoxIndex(cellData.toInt());
                        ui->pageSizeCBox->setCurrentIndex(indx);
                    }
                    break;
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
                    /// Имеем текстовое значение размера страницы, надо получить ID из модели

                    cellData = getIndexInPSizeModel(currentHumanPSize);//currentPSizeId;
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
void AddTemplate::setCurrentPageSize(const QString &psize)
{
    // Ищем ID по значению psize;
    //currentPSizeId = getIndexInPSizeModel(psize);
    currentHumanPSize = psize;
}

void AddTemplate::setPortret()
{
    ui->p_orient_label->setFont(boldFont);
    ui->l_orient_label->setFont(normalFont);
}

void AddTemplate::setLandscape()
{
    ui->p_orient_label->setFont(normalFont);
    ui->l_orient_label->setFont(boldFont);
}

//------------------------------------------------------------------------------
int AddTemplate::translatePSizeID2CBoxIndex(int psize_id)
{
    /// Поиск в
    int CBoxIndex = -1;
    {
        QString column_psize = tr("Размер листа");
        QString column_id = tr("Id");
        QString header;
        int find_id;
        QString psize_hum;

        for(int i=0; i<pSizeModel->rowCount(); i++){
            for (int j=0; j<pSizeModel->columnCount(); j++){
                header = pSizeModel->headerData(j,Qt::Horizontal,
                                                Qt::DisplayRole).toString();
                QModelIndex index = pSizeModel->index(i,j);
                QVariant cell = pSizeModel->data(index,Qt::DisplayRole);

                if (header.compare(column_id,Qt::CaseInsensitive)==0){
                    find_id = cell.toInt();
                }
                if (header.compare(column_psize,Qt::CaseInsensitive)==0){
                    psize_hum = cell.toString();
                }

            }
            if (find_id == psize_id){
                ///Поиск в комбобоксе индекса соответствующего  строке psize_hum
                CBoxIndex = i;
            }
        }
    }
    return CBoxIndex;
}

int AddTemplate::getIndexInPSizeModel(const QString pSizeHuman)
{
    QString header;
    QString column_psize = tr("Размер листа");
    QString column_id = tr("Id");
    QString psize_hum;
    int psize_id = -1;

    if (!pSizeHuman.isEmpty()){

        for(int i=0; i<pSizeModel->rowCount(); i++){
            for (int j=0; j<pSizeModel->columnCount(); j++){
                header = pSizeModel->headerData(j,Qt::Horizontal,
                                                Qt::DisplayRole).toString();
                QModelIndex index = pSizeModel->index(i,j);
                QVariant cell = pSizeModel->data(index,Qt::DisplayRole);
                if (header.compare(column_psize,Qt::CaseInsensitive)==0){
                    psize_hum = cell.toString();
                }else{
                    if (header.compare(column_id,Qt::CaseInsensitive)==0){
                        psize_id = cell.toInt();
                    }
                }
            }
            if (!psize_hum.isEmpty() && psize_hum.compare(pSizeHuman)){
                return psize_id;
            }
        }

    }
    return psize_id;
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

void AddTemplate::closeEvent ( QCloseEvent * event )
{
  event->ignore();
  this->hide();
}
