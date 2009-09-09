#include "asktheuser.h"
#include "ui_asktheuser.h"

askTheUser::askTheUser(QWidget *parent) :
    QDialog(parent),
m_ui(new Ui::askTheUser)
{
    m_ui->setupUi(this);

    /*
connect (m_ui,
		    SIGNAL(),
		 this,
		    SLOT()
		    );
*/
}



askTheUser::~askTheUser()
{
    delete m_ui;
}

void askTheUser::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
