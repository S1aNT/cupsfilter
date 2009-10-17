#include <QtGui>
#include "workreport.h"

workReport::workReport(QWidget *parent)
    : QDialog(parent)
{
    label = new QLabel(trUtf8("Название документа"));
    DocEdit = new QPlainTextEdit();
    DocEdit->setEnabled(false);


    label_stamp = new QLabel(trUtf8("Гриф секретности"));
    lineEdit = new QLineEdit();
    lineEdit->setEnabled(false);

    label_num = new QLabel(trUtf8("Номер экземпляра"));
    numEdit = new QLineEdit();
    numEdit->setEnabled(false);

    label_page = new QLabel(trUtf8("Число страниц"));
    pageEdit = new QLineEdit();
    pageEdit->setEnabled(false);


    label_printer = new QLabel(trUtf8("Распечатан на ..."));
    printerEdit = new QLineEdit();
    printerEdit->setEnabled(false);


//! [1]
    acceptButton = new QPushButton(tr("Подтвердить документ"));

    acceptButton->setDefault(true);

    moreButton = new QPushButton(tr("Забраковать документ"));
    moreButton->setToolTip(QObject::trUtf8("Нажмите для ввода допалнительных сведений об ошибке печати..."));
    moreButton->setCheckable(true);
//! [0]
    moreButton->setAutoDefault(false);



    QVBoxLayout *topLayout = new QVBoxLayout;
    topLayout->addWidget(label);
    topLayout->addWidget(DocEdit);
    topLayout->addStretch(1);

    QGridLayout *bodyLayout = new QGridLayout;

    bodyLayout->addWidget(label_stamp,0,0);
    bodyLayout->addWidget(lineEdit,0,1,1,-1);
    bodyLayout->addWidget(label_printer,1,0);
    bodyLayout->addWidget(printerEdit,1,2,1,-1);
    bodyLayout->addWidget(label_num,2,0);
    bodyLayout->addWidget(numEdit,2,2);
    bodyLayout->addWidget(label_page,3,0);
    bodyLayout->addWidget(pageEdit,3,2);
    bodyLayout->addWidget(acceptButton,4,0);
    bodyLayout->addWidget(moreButton,4,3);


    extension = new QWidget;

    singlePageRB = new QRadioButton();
    singlePageRB->setText(QObject::trUtf8("Брак отдельных страниц"));
    brakEdit = new QLineEdit();
    brakEdit->setEnabled(false);

    allDocRB = new QRadioButton();
    allDocRB->setText(QObject::trUtf8("Бракован весь документ"));


    brakDesEdit =new QPlainTextEdit();
    brakDesEdit->setEnabled(false);
    brakDesEdit->setToolTip(trUtf8("Кратко опишите причины браковки документа"));

    connect (singlePageRB,SIGNAL(toggled(bool)),brakEdit,SLOT(setEnabled(bool)));
    connect (allDocRB,SIGNAL(toggled(bool)),brakDesEdit,SLOT(setEnabled(bool)));

    QGridLayout  *extensionLayout = new    QGridLayout ;
    extensionLayout->setMargin(0);
    extensionLayout->addWidget(singlePageRB,0,0);
    extensionLayout->addWidget( brakEdit,0,1);
    extensionLayout->addWidget(allDocRB,1,0);
    extensionLayout->addWidget(brakDesEdit,2,0,1,-1);
    extension->setLayout(extensionLayout);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addLayout(topLayout, 0, 0);
    mainLayout->addLayout(bodyLayout, 1, 0);
    mainLayout->addWidget(extension, 2, 0, 1, 2);
    setLayout(mainLayout);

    setWindowTitle(QObject::trUtf8("Отчет о распечатанном документе"));

    connect(moreButton, SIGNAL(toggled(bool)), this, SLOT(sendErrorReport(bool)));
    connect (acceptButton,SIGNAL(clicked()),this,SLOT(saveResultToBase()));
    extension->hide();
}
//! [5]

void workReport::saveResultToBase()
{
    //TODO убрать после отладки
    QString title;
    QString message;
    title=QObject::trUtf8("Сообщение успешно сохраннено в базе отчетов");
    if (!saveResult){
	message=QObject::trUtf8("Подтверждение документа записано в базе отчетов");
    }else{

	message=QObject::trUtf8("Сообщение об ошибке, записано в базе отчетов");
    }

    QMessageBox::StandardButton reply;
     reply = QMessageBox::information(this,title, message);
     /*
     if (reply == QMessageBox::Ok)
	 informationLabel->setText(tr("OK"));
     else
	 informationLabel->setText(tr("Escape"));
    */

    QApplication::exit(0);
}
void workReport::sendErrorReport(bool state)
{
    qDebug() << Q_FUNC_INFO << state;
    extension->setVisible(state);
    saveResult=state;
    if (!state){
	acceptButton->setText(QObject::trUtf8("Подтвердить документ"));
    }else{
	acceptButton->setText(QObject::trUtf8("Сообщить об ошибке"));
    }
}
