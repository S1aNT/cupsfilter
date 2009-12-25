#include "workfield.h"
#include "ui_workfield.h"
#include <QProcess>

workField::workField(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::workField)
{
    ui->setupUi(this);
    //hide_stamp = new QLineEdit(this);
    ui->dateField_dateEd->setDate(QDate::currentDate());
    //hide_stamp->setVisible(false);
    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    addTmplDlg = new AddTemplate(this);
    addTmplDlg->setWindowFlags(Qt::Dialog |
                               Qt::CustomizeWindowHint |
                               Qt::WindowTitleHint |
                               Qt::WindowCloseButtonHint |
                               Qt::WindowSystemMenuHint);

    connect (ui->secretCBox,
             SIGNAL(currentIndexChanged(QString)),
             this,
             SLOT(setStampField(QString))
             );
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
             SIGNAL(needCreateEmptyTemplates(QString &)),
             this,
             SIGNAL(do_needCreateEmptyTemplates(QString &))
             );
}

workField::~workField()
{
    delete ui;
}

void workField::setTemplatesModel(QStringListModel *global,
                                  QStringListModel *local)
{
    local_templ_model = local;
    global_templ_model = global;

    ui->templatesCbox->setModel(global_templ_model);
    this->currentTemplates = ui->templatesCbox->currentText();
}

void workField::setPagesCount(int p_count)
{
    ui->pageCountEd->setText(QString::number(p_count,10));
}

void workField::setStampModel(QStringListModel *stamp_model)
{
    ui->secretCBox->setModel(stamp_model);
}

void workField::setMode(int mode_value)
{
    mode = mode_value;
    QString title;
    switch (mode){
    case 0: //Accounting
        title = QObject::trUtf8("Режим работы: [Учет листов]");
        break;
    case 1: //PrintOverAccountPaper
        title = QObject::trUtf8("Режим работы: [Печать на учтенных листах]");
        ui->paperAccountsOutSide->setVisible(false);
        ui->previewBtn->setText(tr("Печать на учтенных листах"));
        break;
    case 2: //PrintWithAccounting
        title = QObject::trUtf8("Режим работы: [Печать документа с автоматическим учетом листов]");
        ui->paperAccountsOutSide->setVisible(false);
        ui->previewBtn->setText(tr("Печать + учтет листов"));
        break;
    }
    this->setWindowTitle(title);

}

void workField::setStampField(QString field)
{
    int col;
    for (int i = 0; i < w_model->columnCount(); i++)
    {

        QStandardItem * header_item = w_model->horizontalHeaderItem(i);
        QString header = header_item->data(Qt::EditRole).toString();
        if (header.compare(QObject::trUtf8("Гриф"))==0){
            col = i;
        }
    }
    QModelIndex index =w_model->index(0, col, QModelIndex());
    w_model->setData(index,QVariant(field),Qt::EditRole);
}

void workField::setModel (QStandardItemModel * model)
{
    //QTableView * tv = new QTableView();
    //tv->setModel(model);
    //tv->show();

    w_model =model;
    //Свяжем  элементы диалогового окна с моделью через mapper
    mapper->setModel(w_model);
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
        else if (header.compare(QObject::trUtf8("stamp_index")) == 0)
        {
            mapper->addMapping(ui->secretCBox, i, "currentIndex");
            //mapper->addMapping(ui->secretCBox->lineEdit(), i);

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
        }/*
        else if (header.compare(QObject::trUtf8("Гриф"))==0)
         {
             // Мапим скрытое поле
             mapper->addMapping(hide_stamp, i);
         }
*/
    }

}
//********************************** public slots ******************************************
void workField::showEditor()
{
    /*
     * Отправим запрос на преобразование шаблона в набор сцен
     * при этом автоматически сохраним модель документа в xml
     */
    emit convertTemplatesToScenes(this->currentTemplates);
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

void workField::doPrintAllowed()
{
    this->setEnableField(true);
    // Запишем данные в строку статуса
    QString msg = QObject::trUtf8("Статус: Сервер безопастности разрешил печать.");
    ui->AnsLabel->setText(msg);
    // TODO это надо убрать отсюда
    // Требование распечатать документ используя выбранный шаблон
    emit needPrintPage(this->currentTemplates);

}

//********************************** private slots ******************************************
void workField::do_addTemplates()
{
    // Вызов tEditor
    QProcess proc;
    QString  m_Command = tr("%1/tEditor.exe").arg(qApp->applicationDirPath());
    QStringList m_Args;
    m_Args.append("new");
    proc.start( m_Command, m_Args );
    if (!proc.waitForStarted()) {
        qDebug()<< QString("Unable to launch application %1").arg(m_Command);
    }
    /*
    int ret;
    //addTmplDlg->setUserName(userName);
    addTmplDlg->setPageSize(p_size_mod);
    //addTmplDlg->setLocalTemplatesDir(local_templ_dir);
    ret = addTmplDlg->exec();
    if (ret == QDialog::Accepted){
        QString msg = QObject::trUtf8("Не забудьте отредактировать шаблон перед использованием");
        this->showInfoWindow(msg);
    }


    Редактор показываем только когда пользователь выеберет этот шаблон
    для редактирования

    */
}

void workField::setCurrentTemplates(QString temp)
{
    currentTemplates = temp; // Запомним выбор пользователя
    QString msg = QObject::trUtf8("Статус: Выбран шаблон %1").arg(temp);
    ui->AnsLabel->setText(msg);
    ui->editTemplatesTBtn->setEnabled(localORglobal);
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
        ui->templatesCbox->setModel(local_templ_model);
    }else{
        // Читаем глобальные шаблоны которые предварительно закачаны в нужный каталог
        ui->tmplLabel->setText(QObject::trUtf8("Общие шаблоны"));
        title_str =QObject::trUtf8("Выберите глобальные шаблоны");
        ui->templatesCbox->setModel(global_templ_model);
    }
    this->currentTemplates = ui->templatesCbox->currentText();
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
            if (!currentTemplates.isEmpty()){
                // Запретим все элементы
                this->setEnableField(false);
                ui->previewBtn->setEnabled(false);
                // Запишем данные в строку статуса
                msg = QObject::trUtf8("Проверка наличия в БД документа с МБ=[%1] и номером экземпляра =[%2]").arg(mb).arg(copyNum);
                ui->AnsLabel->setText(msg);
                emit needAuthUserToPrinter();
                //emit checkMBInBase(mb,copyNum,this->mode);

            }else{
                e_msg = QObject::trUtf8("Шаблон документа должен быть выбран!");
            }

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
