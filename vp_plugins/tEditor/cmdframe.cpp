#include "cmdframe.h"
#include <QDebug>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSignalMapper>
#include <QMessageBox>


cmdFrame::cmdFrame(QWidget *parent) :
        QFrame(parent)
{
    signalMapper = new QSignalMapper(this);
    qDebug() << "signalMapper:" << signalMapper;
    verticalLayout = new QVBoxLayout();
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    verticalLayout->setContentsMargins(0, 0, 0, 0);
    addImgBtn = new QPushButton();
    addImgBtn->setText(tr("Изображение"));
    addImgBtn->setObjectName(QString::fromUtf8("addImgBtn"));

    connect(addImgBtn, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(addImgBtn, tr("Изображение"));


    verticalLayout->addWidget(addImgBtn);
    /*


    pushButton_2 = new QPushButton();
    pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

    verticalLayout->addWidget(pushButton_2);
    */
    this->setLayout(verticalLayout);
    connect(signalMapper,
            SIGNAL(mapped(const QString &)),
            this,
            SIGNAL(clicked(const QString &))
            //SLOT(test())
            );

}

void cmdFrame::test()
{
    QMessageBox msgBox;
     msgBox.setText("The document has been modified.");
     msgBox.exec();
     emit test_signal();
}

void cmdFrame::setCmdButton(QStringList c_list)
{
    QPushButton *pBt;
    for (int i=0;i<c_list.size();i++){
        pBt = new QPushButton();
        pBt->setText(c_list.at(i));
        //qDebug() << "signalMapper:" << signalMapper << "pBt" <<pBt;
        connect(pBt, SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(pBt, QString("%1").arg(c_list.at(i)));
        verticalLayout->addWidget(pBt);
    }

}

cmdFrame::~cmdFrame()
{

}

void cmdFrame::retranslateUi(QFrame *cmdFrame)
{
    cmdFrame->setWindowTitle(QApplication::translate("cmdFrame", "Frame", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(cmdFrame);
} // retranslateUi

void cmdFrame::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
