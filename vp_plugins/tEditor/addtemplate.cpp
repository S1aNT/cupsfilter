#include "addtemplate.h"
#include "ui_addtemplate.h"
#include "tinfoeditmodel.h"
#include "mytypes.h"
#include "editpagesmodel.h"

#include <QtCore/QDateTime>
#include <QtGui/QMessageBox>
#include <QtGui/QFont>
#include <QtGui/QFileDialog>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlError>
#include <QtSql/QSqlTableModel>




using namespace VPrn;

AddTemplate::AddTemplate(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddTemplate)
    , pSizeModel(0)
    , tInfoModel(0)
    , m_pagesModel(0)
{
    ui->setupUi(this);

    boldFont = QFont("Tahoma", 8, QFont::Bold);
    normalFont = QFont("Tahoma", 8, QFont::Normal);
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

    connect (ui->pageSizeCBox,
             SIGNAL(currentIndexChanged(QString)),
             this,
             SLOT(setCurrentPageSize(QString))
             );
}


void AddTemplate::default_init()
{   
    if (tInfoModel && pSizeModel && m_pagesModel){
        //Заполним выпадающий список из модели
        ui->pageSizeCBox->setModel(pSizeModel);
        ui->pageSizeCBox->setModelColumn(pSize_page);
        ui->pageSizeCBox->setCurrentIndex(-1);
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
    //Настроим чекбоксы страницы шаблона
    for (int i=0;i<m_pagesModel->rowCount();i++){
        int p_number   = m_pagesModel->data(m_pagesModel->index(i,VPrn::PD_p_number)).toInt();
        int p_visible  = m_pagesModel->data(m_pagesModel->index(i,VPrn::PD_p_visible)).toInt();
        //QString p_name = m_pagesModel->data(m_pagesModel->index(i,VPrn::PD_p_name)).toString();
        bool state;
        if (p_visible==1){
            state = true;
        }else{
            state = false;
        }
        switch (p_number){
        case 0:
            ui->FPnumberChBox_1->setChecked(state);
            break;
        case 1:
            ui->FPnumberChBox_2->setChecked(state);
            break;
        case 2:
            ui->FPnumberChBox_3->setChecked(state);
            break;
        case 3:
            ui->FPnumberChBox_4->setChecked(state);
            break;
        case 4:
            ui->FPnumberChBox_5->setChecked(state);
            break;
        case 5:
            ui->SPnumberChBox->setChecked(state);
            break;
        case 6:
            ui->ThnumberChBox->setChecked(state);
            break;
        case 7:
            ui->FvnumberChBox->setChecked(state);
            break;
        }
    }

    for (int i=0;i<tInfoModel->columnCount();i++){
        QModelIndex cellIndex = tInfoModel->index(tInfoModel->rowCount()-1,i);
        if (cellIndex.isValid()){
            QVariant cellData = tInfoModel->data(cellIndex,Qt::EditRole);

            switch (i){
            case tInfo_pageID:
                {

                    /*
                      bool cnv_ok;
                        qDebug() << "\nField N "  << i
                                << "\nQVariant " << cellData
                                << "\nQString "  << cellData.toString()
                                << "\nInt "      << cellData.toInt()
                                << "\nDateTime " << QDateTime::fromTime_t(cellData.toUInt())
                                << "\nQString.toInt() " << cellData.toString().toInt(&cnv_ok,10);
                                */
                    int indx = translatePSizeID2ModelRow(cellData.toInt());
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

void AddTemplate::setData4Models()
{
    if (tInfoModel && pSizeModel && m_pagesModel) {
        for (int i=0;i<m_pagesModel->rowCount();i++){

            int p_number   = m_pagesModel->data(m_pagesModel->index(i,VPrn::PD_p_number)).toInt();
            int p_visible;//  = m_pagesModel->data(m_pagesModel->index(i,VPrn::PD_p_visible)).toInt();
            switch (p_number){
            case 0:
                if (ui->FPnumberChBox_1->isChecked()){
                    p_visible =1;
                }else{
                    p_visible =0;
                }
                break;
            case 1:
                if (ui->FPnumberChBox_2->isChecked()){
                    p_visible =1;
                }else{
                    p_visible =0;
                }
                break;
            case 2:
                if (ui->FPnumberChBox_3->isChecked()){
                    p_visible =1;
                }else{
                    p_visible =0;
                }
                break;
            case 3:
                if (ui->FPnumberChBox_4->isChecked()){
                    p_visible =1;
                }else{
                    p_visible =0;
                }
                break;
            case 4:
                if (ui->FPnumberChBox_5->isChecked()){
                    p_visible =1;
                }else{
                    p_visible =0;
                }
                break;
            case 5:
                if (ui->SPnumberChBox->isChecked()){
                    p_visible =1;
                }else{
                    p_visible =0;
                }
                break;
            case 6:
                if (ui->ThnumberChBox->isChecked()){
                    p_visible =1;
                }else{
                    p_visible =0;
                }
                break;
            case 7:
                if (ui->FvnumberChBox->isChecked()){
                    p_visible =1;
                }else{
                    p_visible =0;
                }
                break;
            }

            m_pagesModel->setData(m_pagesModel->index(i,VPrn::PD_p_visible),p_visible,Qt::EditRole);
        }

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
int AddTemplate::translatePSizeID2ModelRow(int psize_id)
{
    int currentId;
    int modelRow = -1;
    {
        for(int i=0; i<pSizeModel->rowCount(); i++){
            currentId = pSizeModel->data(pSizeModel->index(i,VPrn::pSize_id)).toInt();
            if (currentId == psize_id){
                modelRow = i;
                break;
            }
        }
    }
    return modelRow;
}

int AddTemplate::getIndexInPSizeModel(const QString pSizeHuman)
{
    QString psize_hum;
    int psize_id = -1;
    {
        if (!pSizeHuman.isEmpty()){
            for(int i=0; i<pSizeModel->rowCount(); i++){
                psize_hum = pSizeModel->data(pSizeModel->index(i,VPrn::pSize_page)).toString();
                if (psize_hum.compare(pSizeHuman,Qt::CaseInsensitive) == 0 ){
                    psize_id   = pSizeModel->data(pSizeModel->index(i,VPrn::pSize_id)).toInt();
                    break;
                }
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
