#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QDebug>
#include <QMainWindow>
#include <QStack>

#include "itmpl_sql_plugin.h"
#include "igs_plugin.h"
#include "mytypes.h"


using namespace VPrn;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
    void test();
private slots:
    void errorInfo(pluginsError errCode,QString error_message);
    //void doJobFinish(const QString & client_uuid,VPrn::Jobs job_id ,int code ,const QString & out_msg);
    void do_docReady4work (const QString &client_uuid,int pCount);
    void do_docReady4print (const QString &client_uuid);
    void do_docReady4preview (const QString &client_uuid);
private:
    bool loadPlugins();
    void readGlobal(const QString &app_dir);

    void printTempl();

    /**
      * @brief Поиск файлов в каталогах spoolDir/client_uuid/i-copy
      * отвечающих заданному фильтру
      */
    QStringList findFiles(const QString &client_uuid,const QStringList &filters);

    /**
     * @brief Мета задача формирование шаблонов для каждого экземпляра и объединение
     * документа с шаблонами, по завершению метазадачииспускается
     * сигнал ДОКУМЕНТ_К_ПЕЧАТИ_ГОТОВ
     */
    void test_GK_mergeWithTemplate(const QString &client_uuid,QByteArray data);
    void test_GK_Print(const QString &client_uuid);
    /**
     * @brief  Список метазадач (Хранитель) client_uuid_ job_type,№задачи
     */
    QStack <QString> clients_MetaJobPng;
    QByteArray test_prepareUserData();
    QString c_uuid;

    // Приватные данные
    Igs_plugin       *myGs_plugin;
    Itmpl_sql_plugin *myTmpl_plugin;    
    bool m_Ready;

    // Переменные из установок
    QString spoolDir;
    QString ticket_fname;
    QString local_t_path; // Путь к локальным шаблонам пользователя
    QString global_t_path;
    QString gsBin;
    QString pdftkBin;

};

#endif
