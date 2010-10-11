#include "finishpage.h"
#include "mytypes.h"

#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QPlainTextEdit>


using namespace VPrn;

FinishPage::FinishPage(QWidget *parent)
    :QWidget(parent)
{
    this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::MinimumExpanding);
    this->setMinimumSize(QSize(640, 480));
    this->setMaximumSize(QSize(1024, 768));

    this->setWindowTitle(QObject::trUtf8("Запись результатов печати в БД учета документов"));

    QLabel *topLabel = new QLabel(
            QObject::trUtf8("На этом шаге необходимо установить статус документа <br><center><b>Успешно распечатан</b> или <b>Брак</b></center>"
                            "<br><small><b>Внимание</b> согласно приказу МО №010 бракуется экз. документа целиком</small>")
            );
    topLabel->setWordWrap(true);
    myInfoEdit = new QPlainTextEdit(this);
    myInfoEdit->setReadOnly(true);

    QVBoxLayout *verticalLayout = new QVBoxLayout();
    verticalLayout->addWidget(topLabel);
    verticalLayout->addStretch(0);
    verticalLayout->addWidget(myInfoEdit);
    setLayout(verticalLayout);
}

void FinishPage::addInfoMsg(const QString &info)
{
    myInfoEdit->appendPlainText(info);
}
