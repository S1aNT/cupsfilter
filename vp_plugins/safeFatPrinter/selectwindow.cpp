#include "selectwindow.h"
#include "ui_selectwindow.h"

SelectWindow::SelectWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::SelectWindow)
{
    ui->setupUi(this);


    SpiderInTheMiddle = new Mediator(this);
    UMDlg = new getUserNameMandatDlg(this);


    this->setWindowFlags(Qt::Dialog |  Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::WindowSystemMenuHint);

    connect (SpiderInTheMiddle,SIGNAL(error (QString )),this,SLOT(showCritError(QString)));
    connect (SpiderInTheMiddle,SIGNAL(pluginMessage(QString &)),this,SIGNAL(pluginMessage(QString &)));

    connect (SpiderInTheMiddle,SIGNAL(needShowAuthWindow(QString &)),this,SLOT(showAuthWindow(QString&)));
    //connect (SpiderInTheMiddle,SIGNAL(needShowSelectWindow()),this,SLOT(showSelectWindow()));
    //connect (SpiderInTheMiddle,SIGNAL(pluginLoad(const QString &,int, const QColor &)),this,SIGNAL(pluginLoad(const QString &,int, const QColor &)));
    //connect (qApp,SIGNAL(aboutToQuit()),this,SLOT(cleanUp()));
    connect (ui->printerCBox,SIGNAL(currentIndexChanged(QString)),SpiderInTheMiddle,SLOT(authToPrinter(QString)));
}

void SelectWindow::showCritError(QString e_msg)
{
    QMessageBox msgBox;
    QPushButton *abortButton;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setInformativeText(QObject::trUtf8("Для решения этой проблемы обратитесь к администратору безопасности!"));
    abortButton=msgBox.addButton(QObject::trUtf8("Выход"), QMessageBox::RejectRole);
    msgBox.setText(e_msg);
    QObject::connect(&msgBox,SIGNAL(rejected()),qApp,SLOT(quit()));
    msgBox.exec();
}

void SelectWindow::showAuthWindow(QString &user_name)
{
    QString msg;
    UMDlg->setUserName(user_name);
    UMDlg->setMandatModel(SpiderInTheMiddle->mandat_model());
    UMDlg->move(this->calcCenter());
    int ret = UMDlg->exec();
    if (ret == QDialog::Accepted){
        SpiderInTheMiddle->setUserMandat(UMDlg->getCurrentMandat());

        msg= QObject::trUtf8("Авторизация с консоли");
        this->statusBar()->showMessage(msg);
    }else{
        close();
        //msg= QObject::trUtf8("Ошибка получения имени пользователя или его мандата");
    }

}
SelectWindow::~SelectWindow()
{
    delete ui;
}

void SelectWindow::loadPlugin(const QString &app_dir)
{
    SpiderInTheMiddle->loadPlugin(app_dir);
}

void SelectWindow::setFileToWork(QString &in_file)
{
    mainFileName=in_file;
}

void SelectWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

QPoint SelectWindow::calcCenter()
{
    QDesktopWidget desktop;
    QPoint centerWindow;

    QRect rect = desktop.availableGeometry(desktop.primaryScreen());
    //получаем прямоугольник с размерами как у экрана
    centerWindow = rect.center(); //получаем координаты центра экрана
    centerWindow.setX(centerWindow.x() - (this->width()/2));
    centerWindow.setY(centerWindow.y() - (this->height()/2));
    return centerWindow;
}
