#include "teditor.h"
#include "ui_teditor.h"



TEditor::TEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TEditor)
{
    ui->setupUi(this);
    signalMapper = new QSignalMapper(this);

    view_1 = new View(QObject::trUtf8("Лицевая сторона 1-го листа"));
    ui->tabWidget->addTab(view_1,QObject::trUtf8("1-я страница"));
    view_2 = new View(QObject::trUtf8("Лицевая сторона 2-го листа"));
    ui->tabWidget->addTab(view_2,QObject::trUtf8("2-я страница"));
    view_3 = new View(QObject::trUtf8("Обратная сторона 2-го листа"));
    ui->tabWidget->addTab(view_3,QObject::trUtf8("3-я страница"));
    view_4 = new View(QObject::trUtf8("Фонарик"));
    ui->tabWidget->addTab(view_4,QObject::trUtf8("4-я страница"));

    connect(view_1, SIGNAL(addBaseElementToPage()), signalMapper, SLOT(map()));
    connect(view_2, SIGNAL(addBaseElementToPage()), signalMapper, SLOT(map()));
    connect(view_3, SIGNAL(addBaseElementToPage()), signalMapper, SLOT(map()));
    connect(view_4, SIGNAL(addBaseElementToPage()), signalMapper, SLOT(map()));

    signalMapper->setMapping(view_1,1 );
    signalMapper->setMapping(view_2,2);
    signalMapper->setMapping(view_3,3 );
    signalMapper->setMapping(view_4,4 );

    connect(signalMapper,
            SIGNAL(mapped(int)),
            this,
            SIGNAL(addBaseElementToPage(int))
            );

    connect (ui->saveButton,
             SIGNAL(clicked()),
             this,
             SIGNAL(saveTemplates())
            );
    connect (ui->saveButton,
             SIGNAL(clicked()),
             this,
             SLOT(accept())
            );


}

void TEditor::setScene(QGraphicsScene *scene_1,QGraphicsScene *scene_2,
                       QGraphicsScene *scene_3,QGraphicsScene *scene_4)
{
    view_1->gr_view()->setScene(scene_1);
    view_2->gr_view()->setScene(scene_2);
    view_3->gr_view()->setScene(scene_3);
    view_4->gr_view()->setScene(scene_4);
    this->exec(); // Покажем окно редактора
}





TEditor::~TEditor()
{
    delete ui;
}

void TEditor::changeEvent(QEvent *e)
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
