#include "view.h"
#include "ui_view.h"


#define POINT_TO_CM(cm) ((cm)/28.3465058)
#define POINT_TO_MM(mm) ((mm)/2.83465058)     ////////  0.352777778
#define POINT_TO_DM(dm) ((dm)/283.465058)
#define POINT_TO_INCH(inch) ((inch)/72.0)

#define MM_TO_POINT(mm) ((mm)*2.83465058)
#define CM_TO_POINT(cm) ((cm)*28.3465058)     ///// 28.346456693
#define DM_TO_POINT(dm) ((dm)*283.465058)
#define INCH_TO_POINT(inch) ((inch)*72.0)
static const int ObjectName = 0;


View::View(QWidget *parent) :
    QFrame(parent),
    m_ui(new Ui::View)
{
    m_ui->setupUi(this);
    init();
    setFrameStyle(Sunken | StyledPanel);

    //MM_TO_POINT(210)  MM_TO_POINT(297)
    createScene();

    graphicsView = new QGraphicsView(scene);
    graphicsView->setRenderHint(QPainter::Antialiasing, false);
    graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    graphicsView->setBackgroundBrush(Qt::gray);

    int size = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
    QSize iconSize(size, size);

    QToolButton *zoomInIcon = new QToolButton;
    zoomInIcon->setAutoRepeat(true);
    zoomInIcon->setAutoRepeatInterval(33);
    zoomInIcon->setAutoRepeatDelay(0);
    zoomInIcon->setIcon(QPixmap(":/images/zoomin.png"));
    zoomInIcon->setIconSize(iconSize);
    QToolButton *zoomOutIcon = new QToolButton;
    zoomOutIcon->setAutoRepeat(true);
    zoomOutIcon->setAutoRepeatInterval(33);
    zoomOutIcon->setAutoRepeatDelay(0);
    zoomOutIcon->setIcon(QPixmap(":/images/zoomout.png"));
    zoomOutIcon->setIconSize(iconSize);
    zoomSlider = new QSlider;
    zoomSlider->setMinimum(0);
    zoomSlider->setMaximum(500);
    zoomSlider->setValue(250);
    zoomSlider->setTickPosition(QSlider::TicksRight);

    // Zoom slider layout
    QVBoxLayout *zoomSliderLayout = new QVBoxLayout;
    zoomSliderLayout->addWidget(zoomInIcon);
    zoomSliderLayout->addWidget(zoomSlider);
    zoomSliderLayout->addWidget(zoomOutIcon);

    // Label layout
    QHBoxLayout *labelLayout = new QHBoxLayout;
    printButton = new QToolButton;
    printButton->setIcon(QIcon(QPixmap(":/images/print.png")));
    printButton->setIconSize(iconSize);

    label = new QLabel();
    label->setText(QObject::trUtf8("Шаблоны"));
    //sys_button =new QRadioButton(QObject::trUtf8("стандартные"), this);
    //user_button =new QRadioButton(QObject::trUtf8("пользовательские"), this);
    //sys_button->setChecked(true);
    templatesCBox = new QComboBox();

    addTmplButton = new QToolButton;
    addTmplButton->setIcon(QPixmap(":/images/addtmpl.png"));
    addTmplButton->setIconSize(iconSize);

    delTmplButton = new QToolButton;
    delTmplButton->setIcon(QPixmap(":/images/deltmpl.png"));
    delTmplButton->setIconSize(iconSize);

    saveTmplButton = new QToolButton;
    saveTmplButton->setIcon(QPixmap(":/images/savetmpl.png"));
    saveTmplButton->setIconSize(iconSize);

    firstPageButton = new QToolButton;
    firstPageButton->setIcon(QPixmap(":/images/face_page.png"));
    firstPageButton->setIconSize(iconSize);
    firstPageButton->setToolTip(QObject::trUtf8("Выбор лицевой стороны шаблона"));
    secondPageButton = new QToolButton;
    secondPageButton->setIcon(QPixmap(":/images/over_face_page.png"));
    secondPageButton->setToolTip(QObject::trUtf8("Выбор обратной стороны шаблона"));
    secondPageButton->setIconSize(iconSize);
    //label_page = new QLabel();
    //label_page->setText(QObject::trUtf8("Размер страницы"));

    pageSizeCBox =new QComboBox(this);
    //QStringList list;// =QString("A4, A3,B5, Letter, Legal, Executive,A0, A1, A2, A5, A6, A7, A8, A9, B0, B1,B10, B2, B3, B4, B6, B7, B8, B9").split(",");
    pageSizeCBox->addItems(getPageSizeList());

    QButtonGroup *grpBtn = new QButtonGroup();
    portretButton = new QToolButton;
    portretButton->setIcon(QPixmap(":/images/portret.png"));
    portretButton->setIconSize(iconSize);
    portretButton->setToolTip(QObject::trUtf8("Выбор книжной ориентации шаблона"));
    portretButton->setCheckable(true);
    portretButton->setChecked(true);

    landscapeButton = new QToolButton;
    landscapeButton->setIcon(QPixmap(":/images/landscape.png"));
    landscapeButton->setIconSize(iconSize);
    landscapeButton->setToolTip(QObject::trUtf8("Выбор альбомной ориентации шаблона"));
    landscapeButton->setCheckable(true);

    grpBtn->addButton(portretButton,0);
    grpBtn->addButton(landscapeButton,1);

    labelLayout->addWidget(label);
   // labelLayout->addWidget(sys_button);
    //labelLayout->addWidget(user_button);
    labelLayout->addWidget(templatesCBox);
    labelLayout->addWidget(addTmplButton);
    labelLayout->addWidget(delTmplButton);
    labelLayout->addWidget(saveTmplButton);
    labelLayout->addWidget(firstPageButton);
    labelLayout->addWidget(secondPageButton);
    
    labelLayout->addWidget(pageSizeCBox);
    labelLayout->addWidget(portretButton);
    labelLayout->addWidget(landscapeButton);

    labelLayout->addStretch();
    labelLayout->addWidget(printButton);


    //Размещаем элементы согласно фен-шуй
    QGridLayout *topLayout = new QGridLayout;
    topLayout->addLayout(labelLayout, 0, 0);
    topLayout->addWidget(graphicsView, 1, 0);
    topLayout->addLayout(zoomSliderLayout, 1, 1);
    setLayout(topLayout);

    connect(zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(setupMatrix()));
    connect(zoomInIcon, SIGNAL(clicked()), this, SLOT(zoomIn()));
    connect(zoomOutIcon, SIGNAL(clicked()), this, SLOT(zoomOut()));
    connect(printButton, SIGNAL(clicked()), this, SLOT(print()));
    connect(pageSizeCBox ,SIGNAL(currentIndexChanged(QString)),this,SLOT(setPaperSize(QString)));
    connect(grpBtn,SIGNAL(buttonClicked(int)),this,SLOT(flipPage(int)));
    connect(templatesCBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(readFromStream(QString)));
    connect(saveTmplButton,SIGNAL(clicked()),this,SLOT(saveToStream()));
    //QPrintPreviewDialog *p = new QPrintPreviewDialog();
    //p->show();
    setupMatrix();

}


void View::setPaperSize(const QString & text)
{
    qDebug() << Q_FUNC_INFO;
    QMapIterator<QString, int> i(page_size);
    while (i.hasNext()) {
	i.next();
	if (i.key()==text){
	    switch (i.value()){
		case QPrinter::A4:
		    page_width=210;
		    page_height=297;
		    break;
		case QPrinter::A3:
		    page_width=297;
		    page_height=420;
		    break;
	    }
	    qDebug() <<Q_FUNC_INFO << page_height <<page_width;

	}
    }
    //drawPage();
}

bool View::get_pageOrient()
{
    return page_orient;
}
void View::set_pageOrient(bool orient)
{
    page_orient=orient;
}
void View::flipPage(int Btn)
{
    qDebug() <<Q_FUNC_INFO<< Btn << page_orient <<get_pageOrient()<< page_height <<page_width;
    qDebug() << margin_left << margin_top << margin_left<<margin_right;

    if (Btn==0){
	if (page_orient){
	    page_orient=false;
	    std::swap(page_height,page_width);
	    drawPage();
	}
    }else{
	if (Btn==1){
	    if (!page_orient){
		page_orient=true;
		std::swap(page_height,page_width);
		drawPage();
	    }
	}

    }
}

void View::drawPage()
{
    //Рисует прямоугольник заданного размера (в границах сцены)

    qDebug() <<Q_FUNC_INFO<< page_orient << page_height <<page_width;
    qDebug() << margin_left << margin_top << margin_left<<margin_right;

    scene->clear();
    scene->setSceneRect(0, 0, MM_TO_POINT(page_width),MM_TO_POINT(page_height));
    qDebug() << scene->sceneRect();

    QGraphicsRectItem *paper_rect = new QGraphicsRectItem (QRectF(0,0, MM_TO_POINT(page_width),MM_TO_POINT(page_height)));
    qDebug() <<"paper_rect"<<MM_TO_POINT(page_width) << MM_TO_POINT(page_height);
    paper_rect->setPen(QPen(Qt::black));
    paper_rect->setBrush(QBrush(Qt::white));
    paper_rect->setZValue(-1000.0);
    paper_rect->setData(ObjectName, "Paper");
    scene->addItem(paper_rect);

    QGraphicsRectItem *border_rect = new QGraphicsRectItem (QRectF(margin_left, margin_top, MM_TO_POINT(page_width)-margin_left-margin_right,MM_TO_POINT(page_height)-margin_top-margin_bottom));
    qDebug() <<"margin_rect"<< margin_left << margin_top << margin_left<<margin_right;
    border_rect->setPen(QPen(Qt::black,2,Qt::DotLine));
    border_rect->setBrush(QBrush(Qt::white));
    border_rect->setOpacity(0.5);
    border_rect->setZValue(0);
    border_rect->setData(ObjectName, "Border");
    scene->addItem(border_rect);
    scene->update();

}
void View::createScene()
{
    scene = new QGraphicsScene();
    // default page size A4
    setPaperSize(QString("A4 (210 x 297 mm)"));
    drawPage();

    for (int i = 0; i < scene->items().size(); ++i){
	QGraphicsItem *item = scene->items().at(i);

	if ( item->data(ObjectName).toString()=="Paper"){
	    SimpleItem* pSimpleItem = new SimpleItem;
	    pSimpleItem->setPos(80, 40);
	    pSimpleItem->setText(QStringList()<<QObject::trUtf8("Секретно")<<"   Экз№13  ");
	    pSimpleItem->setFlag(QGraphicsItem::ItemIsMovable);
	    pSimpleItem->setParentItem(item);
	    //scene->addItem(pSimpleItem);

	    SimpleItem* pSimpleItem1 = new SimpleItem;
	    pSimpleItem1->setParentItem(item);
	    pSimpleItem1->setPos(10, 40);
	    pSimpleItem1->setText(QStringList()<<QObject::trUtf8("МБ №12/23-412")<<"ВТ-4134");
	    pSimpleItem1->setFlag(QGraphicsItem::ItemIsMovable);
	    //scene->addItem(pSimpleItem1);
	}
    }
}



QStringList View::getPageSizeList()
{
    QMapIterator<QString, int> i(page_size);
    QStringList list;
    while (i.hasNext()) {
	i.next();
	list <<i.key();
    }
    return list;
}
void View::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void View::print()
{
#ifndef QT_NO_PRINTER
    QPrinter printer;
    QPrintDialog dialog(&printer, this);
    if (dialog.exec() == QDialog::Accepted) {
	QPainter painter(&printer);
	QRectF rect;
	double onepoint = printer.logicalDpiY() / 72.0;
	scene->render(&painter);
	//graphicsView->render(&painter);
    }
#endif
}

void View::setupMatrix()
{
    qreal scale = qPow(qreal(2), (zoomSlider->value() - 250) / qreal(50));
    QMatrix matrix;
    matrix.scale(scale, scale);
    graphicsView->setMatrix(matrix);

}
void View::init()
{
    page_orient= false;

    margin_top = MM_TO_POINT(15);
    margin_bottom=MM_TO_POINT(15);
    margin_left=MM_TO_POINT(35);
    margin_right=MM_TO_POINT(15);

    qDebug() <<Q_FUNC_INFO<< page_orient <<margin_left << margin_top << margin_left<<margin_right;

    page_size.insert(QString("A4 (210 x 297 mm)"), QPrinter::A4);
    page_size.insert(QString("A3 (297 x 420 mm)"), QPrinter::A3);
    page_size.insert(QString("A0 (841 x 1189 mm)"), QPrinter::A0);
    page_size.insert(QString("A1 (594 x 841 mm)"), QPrinter::A1);
    page_size.insert(QString("A2 (420 x 594 mm)"), QPrinter::A2);
    page_size.insert(QString("A5 (148 x 210 mm)"), QPrinter::A5);
    page_size.insert(QString("A6 (105 x 148 mm)"), QPrinter::A6);
    page_size.insert(QString("A7 (74 x 105 mm)"), QPrinter::A7);
    page_size.insert(QString("A8 (52 x 74 mm)"), QPrinter::A8);
    page_size.insert(QString("A9 (37 x 52 mm)"), QPrinter::A9);
    page_size.insert(QString("B0 (1000 x 1414 mm)"), QPrinter::B0);
    page_size.insert(QString("B1 (707 x 1000 mm)"), QPrinter::B1);
    page_size.insert(QString("B2 (500 x 707 mm)"), QPrinter::B2);
    page_size.insert(QString("B3 (353 x 500 mm)"), QPrinter::B3);
    page_size.insert(QString("B4 (250 x 353 mm)"), QPrinter::B4);
    page_size.insert(QString("B5 (176 x 250 mm)"), QPrinter::B5);
    page_size.insert(QString("B6 (125 x 176 mm)"), QPrinter::B6);
    page_size.insert(QString("B7 (88 x 125 mm)"), QPrinter::B7);
    page_size.insert(QString("B8 (62 x 88 mm)"), QPrinter::B8);
    page_size.insert(QString("B9 (44 x 62 mm)"), QPrinter::B9);
    page_size.insert(QString("B10 (31 x 44 mm)"), QPrinter::B10);
    page_size.insert(QString("C5E (163 x 229 mm)"), QPrinter::C5E);
    page_size.insert(QString("DLE (110 x 220 mm)"), QPrinter::DLE);
    page_size.insert(QString("Executive (191 x 254 mm)"), QPrinter::Executive);
    page_size.insert(QString("Folio (210 x 330 mm)"), QPrinter::Folio);
    page_size.insert(QString("Ledger (432 x 279 mm)"), QPrinter::Ledger);
    page_size.insert(QString("Legal (216 x 356 mm)"), QPrinter::Legal);
    page_size.insert(QString("Letter (216 x 279 mm)"), QPrinter::Letter);
    page_size.insert(QString("Tabloid (279 x 432 mm)"), QPrinter::Tabloid);

}
void View::setTemplatesList(QStringList & t_list)
{
    templatesCBox->addItems(t_list);
}

View::~View()
{
    delete m_ui;
}
void View::zoomIn()
{
    zoomSlider->setValue(zoomSlider->value() + 1);
}

void View::zoomOut()
{
    zoomSlider->setValue(zoomSlider->value() - 1);
}

void View::saveToStream()
{
    QString template_name;
    template_name=templatesCBox->currentText();
    QFile file(template_name+".tmpl");
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);   // we will serialize the data into the file
    out.setVersion(QDataStream::Qt_4_5);
    out << page_height << page_width <<page_orient;
    out << margin_top << margin_bottom << margin_left << margin_right;
    out << scene->items().size();
    QString t_str;

     for (int i = 0; i < scene->items().size(); ++i){
	 QGraphicsItem *item = scene->items().at(i);
	 t_str=item->data(ObjectName).toString();

	 if ( t_str=="Paper"||t_str==QString("Border")){
	     out <<item->pos() <<item->data(ObjectName).toString();
	 }else{
	    SimpleItem* item =(SimpleItem* )scene->items().at(i);
	    out << item->pos() << item->getFont() <<item->getColor() << item->getText();
	 }
    }
    file.close();
}
void View::readFromStream(QString template_name)
{
    elemList.clear();
    QFile file(template_name+".tmpl");
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_5);
    //TODO save and read magic word !!!
    int items_count;
    in >> page_height >> page_width >>page_orient;
    in >> margin_top >> margin_bottom >> margin_left >> margin_right;
    in >> items_count;
    for (int i=0;i <items_count;i++){
	SimpleItem* pItem = new SimpleItem;
	QPointF ps;
	QFont fnt;
	QColor col;
	QStringList pList;
	in >>ps >>fnt >>col >>pList;
	pItem->setPos(ps);
	pItem->setText(pList);
	elemList.append(pItem);
    }
    file.close();
}
