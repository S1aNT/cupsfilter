#include "workfield.h"
#include "ui_workfield.h"

workField::workField(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::workField)
{
    ui->setupUi(this);
    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    addTmplDlg = new AddTemplate(this);
    addTmplDlg->setWindowFlags(Qt::Dialog |
                               Qt::CustomizeWindowHint |
                               Qt::WindowTitleHint |
                               Qt::WindowCloseButtonHint |
                               Qt::WindowSystemMenuHint);


    connect (ui->editTemplatesTBtn,
             SIGNAL(clicked()),
             this,
             SLOT(showEditor())
             );
    connect (ui->paperAccountsOutSide,
             SIGNAL(toggled(bool)),
             this,
             SLOT(flipLabel(bool))
             );
    connect (ui->previewBtn,
             SIGNAL(clicked()),
             this,
             SLOT(checkData())
             );
    connect (ui->switch_Local_GlolbalBtn,
             SIGNAL(clicked(bool)),
             this,
             SLOT(selectTemplatesDir(bool))
             );
    connect (ui->templatesCbox,
             SIGNAL(currentIndexChanged(QString)),
             this,
             SLOT(setCurrentTemplates(QString)));
    connect (ui->addTemplatesTBtn,
             SIGNAL(clicked()),
             this,
             SLOT(do_addTemplates())
             );
    connect (addTmplDlg,
             SIGNAL(needCreateEmptyTemplates(QString,QString,QString,QString,
                                             QString,bool,QString,
                                             qreal,qreal,qreal,qreal)),
             this,
             SIGNAL(needCreateEmptyTemplates(QString,QString,QString,QString,
                                             QString,bool,QString,
                                             qreal,qreal,qreal,qreal))
             );
}

workField::~workField()
{
    delete ui;
}

void workField::setTemplatesDir(const QString &local,const QString &global)
{
    local_templ_dir = local;
    global_templ_dir = global;
}

void workField::setPagesCount(int p_count)
{
    ui->pageCountEd->setText(QString::number(p_count,10));
}

void workField::setStampModel(QStringListModel *stamp_model)
{
    ui->secretCBox->setModel(stamp_model);
}

void workField::setMode(WorkMode mode_value)
{
    mode = mode_value;
}

void workField::setModel (QStandardItemModel * model)
{
    //Свяжем  элементы диалогового окна с моделью через mapper
    mapper->setModel(model);
    mapper->toLast();

    for (int i = 0; i < model->columnCount(); i++)
    {
        QStandardItem * header_item = model->horizontalHeaderItem(i);
        QString header = header_item->data(Qt::EditRole).toString();

        if (header.compare(QObject::trUtf8("МБ")) == 0)
        {
            mapper->addMapping(ui->mbNumberLineEd, i);
        }
        else if (header.compare(QObject::trUtf8("Название док-та")) == 0)
        {
            mapper->addMapping(ui->docName_plainTextEdit, i);
        }
        else if (header.compare(QObject::trUtf8("Гриф")) == 0)
        {
            mapper->addMapping(ui->secretCBox, i, "currentIndex");
        }
        else if (header.compare(QObject::trUtf8("Пункт перечня"))==0)
        {
            mapper->addMapping(ui->punktLineEd, i);
        }
        else if (header.compare(QObject::trUtf8("Номер копии"))==0)
        {
            mapper->addMapping(ui->copyNumberLineEd, i);
        }else if (header.compare(QObject::trUtf8("Кол-во листов"))==0)
        {
            mapper->addMapping(ui->pageCountEd, i);
        }
        else if (header.compare(QObject::trUtf8("last_page_stamp"))==0)
        {
            mapper->addMapping(ui->lastPageGroupBoxChecked, i,"checked");
        }
        else if (header.compare(QObject::trUtf8("Исполнитель"))==0)
        {
            mapper->addMapping(ui->executor_lineEd, i);
        }else if (header.compare(QObject::trUtf8("Отпечатал"))==0)
        {
            mapper->addMapping(ui->pressman_lineEd, i);
        } else if (header.compare(QObject::trUtf8("Телефон"))==0)
        {
            mapper->addMapping(ui->telephone_lineEd, i);
        } else if (header.compare(QObject::trUtf8("Инв. N"))==0)
        {
            mapper->addMapping(ui->invNumber_lineEd, i);
        }  else if (header.compare(QObject::trUtf8("Дата распечатки"))==0)
        {
            mapper->addMapping(ui->dateField_dateEd, i);
        }
        else if (header.compare(QObject::trUtf8("recivers_list"))==0)
        {
            mapper->addMapping(ui->reciversListGroupBoxChecked, i,"checked");
        }else if (header.compare(QObject::trUtf8("Получатель N1"))==0)
        {
            mapper->addMapping(ui->reciverOne_lineEd, i);
        }else if (header.compare(QObject::trUtf8("Получатель N2"))==0)
        {
            mapper->addMapping(ui->reciverTwo_lineEd, i);
        }else if (header.compare(QObject::trUtf8("Получатель N3"))==0)
        {
            mapper->addMapping(ui->reciverThree_lineEd, i);
        }else if (header.compare(QObject::trUtf8("Получатель N4"))==0)
        {
            mapper->addMapping(ui->reciverFour_lineEd, i);
        }else if (header.compare(QObject::trUtf8("Получатель N5"))==0)
        {
            mapper->addMapping(ui->reciverFive_lineEd, i);
        }
        /*else if (header.compare(QObject::trUtf8("doc_status"))==0)
         {
             // Мапим скрытое поле статус документа
             mapper->addMapping(&documentStatus, i);
         }
         */
    }

}
//********************************** public slots ******************************************
void workField::showEditor()
{
    //Отправим запрос на преобразование шаблона в набор сцен
    emit convertTemplatesToScenes(this->currentTemplates);

    //int ret = teditorDlg->exec();
}

void workField::showInfoWindow(const QString &info)
{
    QMessageBox msgBox;
    QPushButton *okButton;

    //QString info_txt = QObject::trUtf8("Сервер безопасности уполномочен заявить!");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle(QObject::trUtf8("Информационное сообщение"));
    //msgBox.setInformativeText(info_txt);
    okButton=msgBox.addButton(QObject::trUtf8("Выход"), QMessageBox::AcceptRole);
    msgBox.setText(info);
    msgBox.exec();
}

void workField::showPreviewPage(QPixmap &preview_page)
{
    PreViewWnd *wnd = new PreViewWnd(this);
    connect (wnd,SIGNAL(needPrintPage()),this,SIGNAL(needPrintPage()));
    connect (wnd,SIGNAL(dontNeedPrintPage()),this,SIGNAL(dontNeedPrintPage()));
    wnd->showPage(preview_page);
}

//********************************** private slots ******************************************
void workField::do_addTemplates()
{
    int ret;
    addTmplDlg->setUserName(userName);
    addTmplDlg->setPageSize(p_size_mod);
    addTmplDlg->setLocalTemplatesDir(local_templ_dir);
    ret = addTmplDlg->exec();
    if (ret == QDialog::Accepted){
        QString msg = QObject::trUtf8("Не забудьте отредактировать шаблон перед использованием");
        this->showInfoWindow(msg);
    }

    /*
    Редактор показываем только когда пользователь выеберет этот шаблон
    для редактирования
    */
}

void workField::setCurrentTemplates(QString temp)
{
    QString f_name;
    if (localORglobal){
        for (int i=0;i<local_templates_path.size();i++){
            QFileInfo fi=local_templates_path.at(i);
            if (fi.fileName()==temp){
                f_name = fi.absoluteFilePath();
                break;
            }
        }
        ui->editTemplatesTBtn->setEnabled(true);
    }else{
        for (int i=0;i<global_templates_path.size();i++){
            QFileInfo fi=global_templates_path.at(i);
            if (fi.fileName()==temp){
                f_name = fi.absoluteFilePath();
                break;
            }
        }
        ui->editTemplatesTBtn->setEnabled(false);
    }
    currentTemplates = f_name; // Запомним выбор пользователя
}

void workField::selectTemplatesDir(bool mode)
{
    QString title_str;
    QString find_dir;
    QString path;

    localORglobal = mode;

    if (mode){
        ui->tmplLabel->setText(QObject::trUtf8("Личные шаблоны"));
        title_str =QObject::trUtf8("Выберите ранее сохраненные индивидуальные шаблоны");
        path = local_templ_dir;
    }else{
        // Читаем глобальные шаблоны которые предварительно закачаны в нужный каталог
        ui->tmplLabel->setText(QObject::trUtf8("Глобальные шаблоны"));
        title_str =QObject::trUtf8("Выберите глобальные шаблоны");
        path = global_templ_dir;
    }

    QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly | QFileDialog::ReadOnly;
    find_dir = QFileDialog::getExistingDirectory(this, title_str,path, options);

    if (!find_dir.isEmpty()){
        ui->templatesCbox->clear();

        QStringList filters;
        filters << "*.tmpl" << "*.TMPL";

        QDir dir(find_dir);
        dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
        dir.setNameFilters(filters);
        QFileInfoList list = dir.entryInfoList();
        for (int i = 0; i < list.size(); ++i) {
            QFileInfo fileInfo = list.at(i);
            ui->templatesCbox->addItem(fileInfo.fileName());
            if (mode){
                local_templates_path.append(fileInfo);
            }else{
                global_templates_path.append(fileInfo);
            }
        }
        // Установим шаблон файл которого выбран текущим
        QString cur_tmpl = ui->templatesCbox->currentText();
        if (!cur_tmpl.isEmpty()){
            this->setCurrentTemplates(cur_tmpl);
        }
    }
}

void workField::flipLabel(bool flip)
{
    QString msg;
    if (flip){
        msg=QObject::trUtf8("Сохранение в БД карточки документа");
    }else{
        msg=QObject::trUtf8("Печать учетной метки на обратной стороне листа");
    }
    ui->previewBtn->setText(msg);
}

void workField::checkData()
{
    // Проверим что поле МБ и номер экземпляра не пустые
    QString mb = ui->mbNumberLineEd->text();
    QString copyNum = ui->copyNumberLineEd->text();
    QString msg;
    QString info_txt;
    QString e_msg;
    QMessageBox msgBox;
    QPushButton *abortButton;

    if (!mb.isEmpty()){
        if (!copyNum.isEmpty()){
            // Запретим все элементы
            this->setEnableField(false);
            ui->previewBtn->setEnabled(false);
            // Запишем данные в строку статуса
            msg = QObject::trUtf8("Проверка наличия в БД документа с МБ=[%1] и номером экземпляра =[%2]").arg(mb).arg(copyNum);
            ui->AnsLabel->setText(msg);
            emit needAuthUserToPrinter();
            //emit checkMBInBase(mb,copyNum,this->mode);
            //Отправим на печать
        }else{
            e_msg = QObject::trUtf8("Номер экземпляра не может быть не заполнен!");
        }
    }else{
        e_msg = QObject::trUtf8("Поле МБ должно быть заполнено");
    }
    if (!e_msg.isEmpty()){
        info_txt = QObject::trUtf8("Данное поле не может быть пустым");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setInformativeText(info_txt);
        abortButton=msgBox.addButton(QObject::trUtf8("Выход"), QMessageBox::RejectRole);
        msgBox.setText(e_msg);
        msgBox.exec();
    }
}

//*******************************************************************************************
//                              private function

void workField::setEnableField(bool e)
{
    ui->titleGroupBox->setEnabled(e);
    ui->commonGroupBox->setEnabled(e);
    ui->lastPageGroupBoxChecked->setEnabled(e);
    ui->reciversListGroupBoxChecked->setEnabled(e);
    ui->paperAccountsOutSide->setEnabled(e);
}

//*******************************************************************************************
void workField::changeEvent(QEvent *e)
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
