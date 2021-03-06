#include "mytypes.h"
#include "checkdatapage.h"

#include <QtGui/QVBoxLayout>
#include <QtGui/QRadioButton>
#include <QtGui/QCheckBox>
#include <QtGui/QButtonGroup>

using namespace VPrn;

CheckDataPage::CheckDataPage(QWidget *parent)
    : QWidget(parent)
    , step (false)
{
    this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::MinimumExpanding);
    this->setWindowTitle(QObject::trUtf8("Проверка корректности введеных пользователем данных"));

    QLabel *topLabel = new QLabel(
            QObject::trUtf8("На этом шаге необходимо выбрать <b>Печать документа</b> или один из 3-х режимов предпросмотра:")
            );
    topLabel->setWordWrap(true);

    QLabel *label_1 = new QLabel(
            QObject::trUtf8("1.Печать документа без предварительного просмотра."));
    label_1->setWordWrap(true);
    QLabel *label_2 = new QLabel(
            QObject::trUtf8("2.Просмотр перед печатью всех страниц документа."
                            " <br><small><b>Внимание</b> данный режим может занять продолжительное время на обработку документа!</small>"
                            ));
    label_2->setWordWrap(true);
    QLabel *label_3 = new QLabel(
            QObject::trUtf8("3.Просмотр перед печатью только основных страниц документа:<ul>"
                            "<li>  лицевые стороны 1-х страниц выбранных экземпляров;</li>"
                            "<li>  лицевая сторона последующих страниц;</li>"
                            "<li>  обратная сторона всех страниц кроме последней;</li>"
                            "<li>  фонарик.</li></ul>"));
    label_3->setWordWrap(true);

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
    authUserToPrinter->setVisible(false);

    checkCorrectMB    = new QCheckBox( this );
    checkCorrectMB->setEnabled( false );
    checkCorrectMB->setChecked( false );
    checkCorrectMB->setText(
            QObject::trUtf8("Проверка документа на существование в БД учета.")
            );
    checkCorrectMB->setVisible(false);

    checkMergeDocWithTemplates    = new QCheckBox( this );
    checkMergeDocWithTemplates->setEnabled( false );
    checkMergeDocWithTemplates->setChecked( false);
    checkMergeDocWithTemplates->setText(
            QObject::trUtf8("Идет процесс формирования наложения шаблона на документ и формирования стр. предпросмотра")
            );
    checkMergeDocWithTemplates->setVisible(false);

    QVBoxLayout *verticalLayout = new QVBoxLayout();
    verticalLayout->addWidget(topLabel);
    verticalLayout->addStretch(0);
    verticalLayout->addWidget(label_1);
    verticalLayout->addStretch(0);
    verticalLayout->addWidget(label_2);
    verticalLayout->addStretch(0);
    verticalLayout->addWidget(label_3);
    verticalLayout->addStretch(0);

    verticalLayout->addWidget(printWithoutPreview);
    verticalLayout->addWidget(previewAllPages );
    verticalLayout->addWidget(previewPartPages);
    verticalLayout->addWidget(authUserToPrinter);
    verticalLayout->addWidget(checkCorrectMB);
    verticalLayout->addWidget(checkMergeDocWithTemplates);
    verticalLayout->addStretch(0);
    setLayout(verticalLayout);


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

void CheckDataPage::setEnableNext(bool flag)
{
    step = flag;
}

void CheckDataPage::needRestart()
{
    step = false;
    checkMergeDocWithTemplates->setChecked( step );
    checkMergeDocWithTemplates->setVisible( step );
    authUserToPrinter->setChecked( step );
    authUserToPrinter->setVisible( step );
    checkCorrectMB->setChecked( step );
    checkCorrectMB->setVisible( step );
    enablePreviewButton( step );
}

void CheckDataPage::setCheckMergeDocWithTemplates( bool flag, const QString & info )
{
    checkMergeDocWithTemplates->setChecked( flag );
    checkMergeDocWithTemplates->setText( info );
    checkMergeDocWithTemplates->setVisible(true);
    step = flag;
}

void CheckDataPage::setAuthCheck( bool flag, const QString & info )
{
    authUserToPrinter->setChecked( flag );
    authUserToPrinter->setText( info );
    enablePreviewButton((checkCorrectMB->isChecked() &&
                         authUserToPrinter->isChecked()));
    authUserToPrinter->setVisible(true);
}

void CheckDataPage::setMbCheck( bool flag, const QString & info )
{
    checkCorrectMB->setChecked( flag );
    checkCorrectMB->setText( info );
    enablePreviewButton((checkCorrectMB->isChecked() &&
                         authUserToPrinter->isChecked()));
    checkCorrectMB->setVisible(true);
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
        emit select_mode(VPrn::pre_FullMode);
        enablePreviewButton(false);
        step = false;
    }
}
void CheckDataPage::startMergedDoc4PreviewPart(bool status)
{
    if (status){
        emit select_mode(VPrn::pre_PartMode);
        enablePreviewButton(false);
        step = false;
    }
}
void CheckDataPage::startMergedDoc4Print(bool status)
{
    if (status){
        emit select_mode(VPrn::pre_ClearPrintMode);
        enablePreviewButton(false);
        step = true;
    }
}
