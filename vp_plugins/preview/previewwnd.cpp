#include "previewwnd.h"

#include <QtGui/QHBoxLayout>

#include <QtGui/QSplitter>
#include <QtGui/QSlider>
#include <QtGui/QToolButton>

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QtCore/QStringList>
#include <QtCore/QDir>

#include <QFileDialog>

PreviewWnd::PreviewWnd(QWidget *parent) :
        QWidget(parent)
        ,leftFrame(0)
        ,rightFrame(0)
        ,thumbSize(100,100)
{
    int size = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
    QSize iconSize(size, size);

    leftFrame = new ViewPort(this);
    rightFrame = new ViewPort(this);

    leftFrame->showThumbNail(true,thumbSize);
    leftFrame->setMinimumSize(200,600);
    rightFrame->setMinimumSize(800,600);

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

    QHBoxLayout * m_layout = new QHBoxLayout();

    m_layout->addWidget(mainSplitter);
    this->setLayout(m_layout);

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

    //connect(graphicsView->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(setResetButtonEnabled()));
    //connect(graphicsView->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(setResetButtonEnabled()));
}



//------------------------------ PRIVATE SLOTS ---------------------------------
void PreviewWnd::zoomIn()
{
    zoomSlider->setValue(zoomSlider->value() - 1);
}

void PreviewWnd::zoomOut()
{
    zoomSlider->setValue(zoomSlider->value() + 1);
}


//--------------------------------- PRIVATE FUNCTION ---------------------------

void PreviewWnd::loadPixmapFromFolder(const QString &folder)
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::DirectoryOnly);
    QStringList folder_s;
    if (dialog.exec())
        folder_s = dialog.selectedFiles();
    //dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg)"));

    QPixmap img_item;
    for (int i=0;i<folder_s.size();i++){
        imageFilesList << findFiles(folder_s.at(i));
    }
    // сформируем изображения
    for (int i = 0; i < imageFilesList.size(); ++i){
        if (img_item.load(imageFilesList.at(i))){
            imageFullItems.insert(i,img_item);
            imageThumbItems.insert(i,img_item.scaled(thumbSize,
                                                     Qt::KeepAspectRatio,
                                                     Qt::SmoothTransformation));
        }
    }
    if (leftFrame){
        leftFrame->setPixMapList(imageThumbItems);
    }
    if (rightFrame){
        rightFrame->setPixMapList(imageFullItems);
    }


}


QStringList PreviewWnd::findFiles(const QString& path)
{
    QStringList files;

    QDir dir = QDir::current();
    if(!path.isEmpty())
        dir = QDir(path);

    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        files.append(dir.absoluteFilePath(fileInfo.fileName()));
    }

    return files;
}
