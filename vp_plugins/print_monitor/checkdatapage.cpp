#include "checkdatapage.h"

#include <QtGui/QVBoxLayout>
#include <QtGui/QRadioButton>
#include <QtGui/QCheckBox>
#include <QtGui/QButtonGroup>

CheckDataPage::CheckDataPage(QWidget *parent)
    :QWizardPage(parent)
{
    setTitle(QObject::trUtf8("Проверка корректности введеных пользователем данных"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/select_mode.png"));


    topLabel = new QLabel(
            QObject::trUtf8("На этом шаге, после прохождения автоматизированных проверок, "
                            "необходимо выбрать печать документа или один из режимов предпросмотра:"
                            "<ul><li>1.Печать документа без предварительного просмотра.</li>"
                            "<li>2.Просмотр перед печатью всех страниц документа. "
                            "Внимание данный режим может занять продолжительное время на обработку документа!</li>"
                            "<li>3.Просмотр перед печатью только основных страниц документа:</li></ul>"
                            "<ul><li>     лицевые стороны 1-х страниц выбранных экземпляров;</li>"
                            "<li>       лицевая сторона последующих страниц</li>"
                            "<li>       обратная сторона всех страниц кроме последней</li>"
                            "<li>       фонарик</li></ul>"));
    topLabel->setWordWrap(true);

    printWithoutPreview= new QRadioButton(this);

    printWithoutPreview->setText(
            QObject::trUtf8("Печать документа без предварительного просмотра."));

    previewAllPages = new QRadioButton( this );

    previewAllPages->setText(
            QObject::trUtf8("Просмотр перед печатью всех страниц документа."));

    previewPartPages = new QRadioButton(this);

    previewPartPages->setText(
            QObject::trUtf8("Просмотр перед печатью только основных страниц документа."));


    printWithoutPreview->setEnabled( false );
    printWithoutPreview->setChecked( false );

    previewPartPages->setEnabled( false );
    previewPartPages->setChecked( false );

    previewAllPages->setEnabled( false );
    previewAllPages->setChecked( false );


    rb_group = new QButtonGroup (this);
    rb_group->setExclusive(true);

    rb_group->addButton( previewAllPages );
    rb_group->addButton( previewPartPages );
    rb_group->addButton( printWithoutPreview );

    authUserToPrinter = new QCheckBox( this );
    authUserToPrinter->setEnabled( false );
    authUserToPrinter->setText(
            QObject::trUtf8("Авторизация пользователя на доступ к принтеру")
            );

    authUserToPrinter->setChecked( false);
    checkCorrectMB    = new QCheckBox( this );
    checkMergeDocWithTemplates    = new QCheckBox( this );
    checkCorrectMB->setEnabled( false );
    checkCorrectMB->setChecked( false );

    checkCorrectMB->setText(
            QObject::trUtf8("Проверка документа на существование в БД учета.")
            );

    checkMergeDocWithTemplates->setEnabled( false );
    checkMergeDocWithTemplates->setChecked( false);
    checkMergeDocWithTemplates->setText(
            QObject::trUtf8("Идет процесс формирования наложения шаблона на документ и формирования стр. предпросмотра")
            );


    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget ( topLabel );
    layout->addSpacing( 3 );
    layout->addWidget ( printWithoutPreview);
    layout->addWidget ( previewAllPages );
    layout->addWidget ( previewPartPages );
    layout->addWidget ( authUserToPrinter );
    layout->addWidget ( checkCorrectMB    );
    layout->addWidget ( checkMergeDocWithTemplates );

    setLayout(layout);

    registerField("checkCorrectMB*",checkCorrectMB);
    registerField("checkMergeDocWithTemplates*",checkMergeDocWithTemplates);
    registerField("authUserToPrinter*",authUserToPrinter);

    connect (printWithoutPreview,SIGNAL(toggled(bool)),
             this, SLOT ( startMergedDoc4Print(bool) )
             );
    connect (previewAllPages,SIGNAL(toggled(bool)),
             this, SLOT ( startMergedDoc4PreviewFull(bool) )
             );
    connect (previewPartPages,SIGNAL(toggled(bool)),
             this, SLOT ( startMergedDoc4PreviewPart(bool) )
             );

}



void CheckDataPage::setVisible(bool visible)
{
    QWizardPage::setVisible(visible);
    if (visible){
        rb_group->setExclusive(false);
        checkCorrectMB->setEnabled( false );
        checkCorrectMB->setChecked( false );

        printWithoutPreview->setEnabled( false );
        printWithoutPreview->setChecked( false );

        previewPartPages->setEnabled( false );
        previewPartPages->setChecked( false );

        previewAllPages->setEnabled( false );
        previewAllPages->setChecked( false );

        checkCorrectMB->setText(
                QObject::trUtf8("Visible Проверка документа на существование в БД учета.")
                );

        checkMergeDocWithTemplates->setEnabled( false );
        checkMergeDocWithTemplates->setChecked( false);
        checkMergeDocWithTemplates->setText(
                QObject::trUtf8("Идет процесс формирования наложения шаблона на документ и формирования стр. предпросмотра")
                );
        rb_group->setExclusive(true);
    }
}

void CheckDataPage::setCheckMergeDocWithTemplates( bool flag, const QString & info )
{
    checkMergeDocWithTemplates->setChecked( flag );
    checkMergeDocWithTemplates->setText( info );
}

void CheckDataPage::setAuthCheck( bool flag, const QString & info )
{
    authUserToPrinter->setChecked( flag );
    authUserToPrinter->setText( info );
    enablePreviewButton((checkCorrectMB->isChecked() &&
                         authUserToPrinter->isChecked()));
}

void CheckDataPage::setMbCheck( bool flag, const QString & info )
{
    checkCorrectMB->setChecked( flag );
    checkCorrectMB->setText( info );
    enablePreviewButton((checkCorrectMB->isChecked() &&
                         authUserToPrinter->isChecked()));
}

void CheckDataPage::enablePreviewButton(bool mode)
{    
    previewAllPages->setEnabled(mode);
    previewPartPages->setEnabled(mode);
    printWithoutPreview->setEnabled(mode);
}

void CheckDataPage::startMergedDoc4PreviewFull(bool status)
{
    if (status){
        qDebug() << Q_FUNC_INFO << "emit select_preview(true)";
        emit select_preview(true);
        enablePreviewButton(false);
    }
}
void CheckDataPage::startMergedDoc4PreviewPart(bool status)
{
    if (status){
        emit select_preview(false);
        enablePreviewButton(false);
    }
}
void CheckDataPage::startMergedDoc4Print(bool status)
{
    if (status){
        emit select_print();
        enablePreviewButton(false);
    }
}
