#ifndef WORKFIELD_H
#define WORKFIELD_H

#include <QDialog>
#include <QPixmap>
#include <QMessageBox>
#include <QStringListModel>
#include <QStandardItemModel>

#include <QSignalMapper>
#include <QDataWidgetMapper>
#include <QFileDialog>

#include "previewwnd.h"
#include "teditor.h"
#include "tech_global.h"
#include "addtemplate.h"

using namespace SafeVirtualPrinter;

namespace Ui {
    class workField;
}

class workField : public QDialog{

    Q_OBJECT
    Q_ENUMS(WorkMode)

public:
    workField(QWidget *parent = 0);
    ~workField();
    void setPagesCount(int p_count);
    void setStampModel(QStringListModel *stamp_model);
    void setPageSizeModel(QStringListModel *page_size_model){p_size_mod =page_size_model;};
    void setUserName(const QString & user_name){userName=user_name;};
    void setModel (QStandardItemModel * model);
    void setMode (WorkMode mode_value);
    void setTemplatesDir(const QString &local,const QString &global);
signals:
    void checkMBInBase(const QString &mb_value,const QString &copyNum_value,WorkMode work_mode);
    void needAuthUserToPrinter();
    void needPrintPage();
    void dontNeedPrintPage();
    void needCreateEmptyTemplates(const QString & file_name,
                                  const QString & t_author,
                                  const QString & t_name,
                                  const QString & t_desc,
                                  const QString & p_size,
                                  bool p_orient,
                                  const QString & c_date,
                                  qreal m_top,
                                  qreal m_bottom,
                                  qreal m_right,
                                  qreal m_left);
public slots:
    void showInfoWindow(const QString &info);
    void showPreviewPage(QPixmap &preview_page);
private slots:
    void flipLabel(bool flip);
    void checkData(); // При нажатии на кнопку идет анализ введенных данных
    void selectTemplatesDir(bool mode); //Выбор локальный или глобальный каталог шаблонов
    void do_addTemplates(); // добавление пустого шаблона
    void setCurrentTemplates(QString temp);
    void showEditor();
protected:
    void changeEvent(QEvent *e);
private:
    Ui::workField *ui;
    QDataWidgetMapper *mapper;
    QSignalMapper *signalMapper;

    AddTemplate *addTmplDlg;
    TEditor * teditorDlg;

    QStringListModel *p_size_mod;
    QString userName;

    bool localORglobal; // Режим шаблонов локальные или глобальные
    QString local_templ_dir;
    QString global_templ_dir;
    QString currentTemplates;

    QList <QFileInfo> local_templates_path;
    QList <QFileInfo> global_templates_path;

    WorkMode mode;

    void setEnableField(bool e);

};

#endif // WORKFIELD_H
