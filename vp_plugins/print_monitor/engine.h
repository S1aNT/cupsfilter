#ifndef ENGINE_H
#define ENGINE_H
#include <QtCore/QObject>
#include <QtCore/QFileInfo>

QT_FORWARD_DECLARE_CLASS ( QStringList )
QT_FORWARD_DECLARE_CLASS ( QLocalSocket )
QT_FORWARD_DECLARE_CLASS ( QStandardItemModel )
QT_FORWARD_DECLARE_CLASS ( QStringListModel )

class mySocketClient;
class Message;

#include "mytypes.h"

using namespace VPrn;

class Engine :public QObject
{
    Q_OBJECT
public:

    Engine(QObject*parent = 0,const QString &link = QString(),
           const QString &gk_bin = QString());

    /**
      * @fn ~Engine();
      * @brief В деструкторе проверяем живость соденения и если оно есть прибиваем его
      */
    ~Engine();
   /**
      * @fn void setDocCardModel
      * @fn void setPrintersModel
      * @fn void setStampsModel
      * @fn void setTemplatesModel
      * @fn void setMandatsModel
      * @short Запомним указатели на рабочие модели
    */
    void setDocCardModel    (QStandardItemModel *pDC_model   ) {m_DC_model   = pDC_model; }
    void setPrintersModel   (QStandardItemModel *pPrn_model  ) {m_Prn_model  = pPrn_model;}
    void setTemplatesModel  (QStandardItemModel *pTmpl_model ) {m_Tmpl_model = pTmpl_model;}
    void setMandatsModel    (QStringListModel *pMandats_model ){m_Mandats_model = pMandats_model;}
    void setStampsModel     (QStringListModel *pStamp_model )  {m_Stamp_model = pStamp_model;}
    /**
      @fn void prepareFileToPrint(const QString & file_name);
      @brief Отпаравляет запрос на GateKeeper с требованием конвертировать ps -> pdf
      */
    void prepareFileToPrint(const QString & file_name);

signals:
    void error(const QString &eCode,const QString &eMsg);
    // Результат наложения шаблона на документ
    void MergeDocWithTemplates( bool flag,const QString &info);
    // список путей к сформированным  страницам предпросмотра
    //void PicturesList (QStringList page_list);
    // Список путей уже сформированным документам готовым для печати
    //void ReadyForPrintPages(QByteArray page_list);
    // Получено требование завершить работу
    void reciveGoodBayMsg(const QString &shutdown_info);
    // Требуется передать демону дополнительные параметы документа для его регистрации
    // в бд учета
    //void needRegisterDocInBase();
    //Документ зарегистрирован в БД УЧЕТА
    //void RegisterDocInBase(bool flag,const QString &info);
    // Результат авторизации доступа к устройству
    //void authToDevice(bool flag,const QString &info);

    void infoMessage(const QString &info);
    void gk_notReady(const QString &ext_info); // локальный сервер не готов к работе
    // Требуется авторизовать пользователя есть имя, и связь с демоном
    void needAuthUser(const QString &login_mandat_list);
    void gk_fullReady(const QString &login,const QString &mandat);

    // Регистрация на локальном сервере
    void LocalSrvRegistr();
    void RemoteDemonRegistr();
    void ReciveUserName();
    void ReciveUserMandat();
    //Локальный сервер вернул список принтеров доступных данному пользователю
    void RecivePrintersList();
    void ReciveSecLevelList();
    // Документ успешно конвертирован в pdf
    void doc_converted();
    // Подсчитали количество страниц
    void getPagesCount (int p_count);

public slots:
    /**
      * @fn void launchAndConnect();
      * @brief проверяет факт запуска GateKeeper (Запускает его если не запущен)
      * и устаналивает с ним долгие и доверительные отношения
      */
    void launchAndConnect();
    void setAuthData(const QString &login,const QString &mandat);

private slots:
    void do_checkPointChanged(MyCheckPoints r_cpoint);
    /**
      * @fn void parseMessage(const Message r_msg);
      * @brief Обработка полученных сообщений от локального сервера
      */
    void parseMessage(const Message &r_msg);

private:
    QStandardItemModel *m_DC_model;
    QStandardItemModel *m_Prn_model;
    QStandardItemModel *m_Tmpl_model;

    QStringListModel   *m_Mandats_model;
    QStringListModel   *m_Stamp_model;
    mySocketClient     *m_LocalClient;
    bool stopLaunch;
    QString currentUserName;
    QString currentUserMandat;
    QString link_name;
    QString gatekeeper_bin;

    // @var client_uuid;       Уникальный номер полученный от GateKeeper
    QString client_uuid;

    //-------------------------------------------------------------------------------------
    void sendMessage2LocalSrv(const Message &s_msg);

    void afterConnectSteps();   
    void savePrintersListToModel(const QString &prn_list);
    void saveTemplatesListToModel(QByteArray tmpl_list);
};
#endif // ENGINE_H

