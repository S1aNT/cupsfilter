#include "previewpage.h"


#include <QtGui/QVBoxLayout>
#include <QtGui/QRadioButton>
#include <QtGui/QCheckBox>
#include <QtGui/QMessageBox>
#include <QtGui/QSplitter>
#include <QtCore/QStringList>
#include <QRegExp>

PreViewPage::PreViewPage(QWidget *parent)
    : QWizardPage(parent)
    ,leftFrame(0)
    ,rightFrame(0)
    ,thumbSize(100,100)
{
    checkPicturesList    = new QCheckBox( this );
    checkPicturesList->setText(QObject::trUtf8("pic list "));
    checkPicturesList->setEnabled( false );
    checkPicturesList->setChecked( false );
    checkPicturesList->hide();

    setTitle(QObject::trUtf8("Предварительный просмотр сформированного документа."));

    int size = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
    QSize iconSize(size, size);

    leftFrame = new ViewPort(this);
    rightFrame = new ViewPort(this);

    leftFrame->showThumbNail(true,thumbSize);
    leftFrame->setMinimumSize(200,512);
    rightFrame->setMinimumSize(720,512);

    // Zoom slider layout
    QFrame *zoomSliderFrame = new QFrame();
    zoomSliderFrame->setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);

    QToolButton *zoomToFitIcon = new QToolButton;
    zoomToFitIcon->setAutoRepeat(true);
    zoomToFitIcon->setAutoRepeatInterval(33);
    zoomToFitIcon->setAutoRepeatDelay(0);
    zoomToFitIcon->setIcon(QPixmap(":/zoomtofit.xpm"));
    zoomToFitIcon->setIconSize(iconSize);
    zoomToFitIcon->setToolTip(
            QObject::trUtf8("Увеличить изображение до размеров экрана."));

    QToolButton *zoomToFullSizeIcon = new QToolButton;
    zoomToFullSizeIcon->setAutoRepeat(true);
    zoomToFullSizeIcon->setAutoRepeatInterval(33);
    zoomToFullSizeIcon->setAutoRepeatDelay(0);
    zoomToFullSizeIcon->setIcon(QPixmap(":/zoomtofull.xpm"));
    zoomToFullSizeIcon->setIconSize(iconSize);
    zoomToFullSizeIcon->setToolTip(QObject::trUtf8("Исходный размер изображения."));

    QToolButton *zoomInIcon = new QToolButton;
    zoomInIcon->setAutoRepeat(true);
    zoomInIcon->setAutoRepeatInterval(33);
    zoomInIcon->setAutoRepeatDelay(0);
    zoomInIcon->setIcon(QPixmap(":/zoomin.xpm"));
    zoomInIcon->setIconSize(iconSize);
    zoomInIcon->setToolTip(QObject::trUtf8("Уменьшить масштаб изображения."));

    QToolButton *zoomOutIcon = new QToolButton;
    zoomOutIcon->setAutoRepeat(true);
    zoomOutIcon->setAutoRepeatInterval(33);
    zoomOutIcon->setAutoRepeatDelay(0);
    zoomOutIcon->setIcon(QPixmap(":/zoomout.xpm"));
    zoomOutIcon->setIconSize(iconSize);
    zoomOutIcon->setToolTip(QObject::trUtf8("Увеличить масштаб изображения."));
    zoomSlider = new QSlider();
    zoomSlider->setMinimum(0);
    zoomSlider->setMaximum(500);
    zoomSlider->setValue(250);
    zoomSlider->setOrientation(Qt::Horizontal);
    zoomSlider->setTickPosition(QSlider::TicksRight);

    QHBoxLayout * zoomSliderFrame_layout = new QHBoxLayout();
    zoomSliderFrame->setLayout(zoomSliderFrame_layout);
    zoomSliderFrame_layout->addWidget(zoomToFullSizeIcon);
    zoomSliderFrame_layout->addWidget(zoomToFitIcon);
    zoomSliderFrame_layout->addWidget(zoomInIcon);
    zoomSliderFrame_layout->addWidget(zoomSlider);
    zoomSliderFrame_layout->addWidget(zoomOutIcon);


    QSplitter    *toolSplitter = new QSplitter();
    toolSplitter->setOrientation(Qt::Vertical);
    toolSplitter->addWidget(zoomSliderFrame);
    toolSplitter->addWidget(rightFrame);
    toolSplitter->setStretchFactor(1, 1);

    QSplitter    *mainSplitter = new QSplitter();
    mainSplitter->setOrientation(Qt::Horizontal);
    mainSplitter->addWidget(leftFrame);
    mainSplitter->addWidget(toolSplitter);
    mainSplitter->setStretchFactor(1, 1);


    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget ( checkPicturesList);
    layout->addWidget ( mainSplitter);

    setLayout(layout);

    registerField("checkPicturesList*",checkPicturesList);

    connect(leftFrame,SIGNAL(selectPixmap(int)),
            rightFrame,SLOT(showPage(int)));

    connect(zoomInIcon, SIGNAL(clicked()),
            this, SLOT(zoomIn())
            );
    connect(zoomOutIcon, SIGNAL(clicked()),
            this, SLOT(zoomOut())
            );
    connect(zoomSlider, SIGNAL(valueChanged(int)),
            rightFrame, SLOT(setupMatrix(int))
            );
    connect (zoomToFullSizeIcon,SIGNAL(clicked()),
             rightFrame,SLOT(zoomToFullSize()));
    connect (zoomToFitIcon,SIGNAL(clicked()),
             rightFrame,SLOT(zoomToFit()));

}

void PreViewPage::showPicturesList(QStringList png_list)
{
    if (png_list.size() >0 ){
        checkPicturesList->setChecked(true);

        // сформируем изображения
        QPixmap img_item;
        for (int i = 0; i < png_list.size(); ++i){
            if (img_item.load(png_list.at(i))){
                // Из пути имени файла вызерезаем номер экз. и тип страницы
                // spoolDir/client_uuid/t_firstpage.pdf

                QRegExp rx("/.+/.+/.+/.+/(.+)-copy/(.+)_(\\d{1,}).png");
                qDebug() << png_list.at( i );

                if(rx.indexIn( png_list.at( i ) ) != -1){
                    // Наш файлик можно обрабатывать
                    QString copy_num = rx.cap(1);
                    QString page_type  = rx.cap(2);
                    QString page_number  = rx.cap(3);

                    QString str_type;

                    if (page_type.compare("firstpage") == 0){
                        str_type = QString("<center>Экз.№%1<br/>%2[%3]</center>")
                                  .arg(copy_num,
                                       QObject::trUtf8("Лицевая сторона 1-й лист."),
                                       page_number );
                    }
                    if (page_type.compare("lastpage") == 0){
                        str_type = QString("Экз.№%1.%2[%3]")
                                   .arg(copy_num,
                                        QObject::trUtf8("Фонарик"),
                                        page_number );
                    }
                    if (page_type.compare("otherpage") == 0){
                        str_type = QString("Экз.№%1.%2[%3]")
                                   .arg(copy_num,
                                        QObject::trUtf8("Лицевая сторона 2-й и далее листы.")
                                        ,page_number  );
                    }

                    if (page_type.compare("oversidepage") == 0){
                        str_type = QString("Экз.№%1.%2[%3]")
                                   .arg(copy_num,
                                        QObject::trUtf8("Обратная сторона."),
                                        page_number  );
                    }
                    descImagesList.insert(i, str_type );
                }

                imageFullItems.insert(i,img_item);
                imageThumbItems.insert(i,img_item.scaled(thumbSize,
                                                         Qt::KeepAspectRatio,
                                                         Qt::SmoothTransformation));
            }
        }
        if (leftFrame){
            leftFrame->setDescList(descImagesList);
            leftFrame->setPixMapList(imageThumbItems);

        }
        if (rightFrame){
            rightFrame->setPixMapList(imageFullItems);
        }
    }
}

bool PreViewPage::validatePage()
{

    emit printCurrentDoc();
    return true;
}

void PreViewPage::setVisible(bool visible)
{

    QWizardPage::setVisible(visible);

        if (visible) {
            this->setButtonText(QWizard::NextButton,QObject::trUtf8("Печать документа"));
            wizard()->setButtonText(QWizard::CustomButton1, QObject::trUtf8("Рестарт"));
            wizard()->setOption(QWizard::HaveCustomButton1, true);
            connect(wizard(), SIGNAL(customButtonClicked(int)),
                    this, SLOT(restartButtonClicked()));
        } else {
            wizard()->setOption(QWizard::HaveCustomButton1, false);
            disconnect(wizard(), SIGNAL(customButtonClicked(int)),
                       this, SLOT(restartButtonClicked()));
        }

}
//------------------------------ PRIVATE SLOTS ---------------------------------
void PreViewPage::zoomIn()
{
    zoomSlider->setValue(zoomSlider->value() - 1);
}

void PreViewPage::zoomOut()
{
    zoomSlider->setValue(zoomSlider->value() + 1);
}

void PreViewPage::restartButtonClicked()
{
    QString mb     = wizard()->field("mbNumberLineEd").toString();
    int cur_copy   = wizard()->field("e_currentSBox").toInt();
    int total_copy = wizard()->field("e_totalSBox").toInt();
    bool all_copy  = wizard()->field("total_copyes").toBool();

    imageFullItems.clear();
    imageThumbItems.clear();
    descImagesList.clear();
    imageFilesList.clear();

    checkPicturesList->setChecked(false);
    emit UserDemands2Restart(mb,cur_copy,total_copy,all_copy);

    wizard()->setStartId(VPrn::Page_Select);
    wizard()->restart();
}
