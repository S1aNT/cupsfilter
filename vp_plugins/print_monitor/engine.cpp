#include "engine.h"
#include "mysocketclient.h"
#include "message.h"

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QDir>
//#include <QtCore/QFileInfoList>
#include <QtCore/QProcess>
#include <QtCore/QMetaType>
#include <QtCore/QTimer>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>

#include <QtGui/QStandardItem>


Engine::Engine(QObject *parent,const QString &link,const QString &gk_bin)
    : QObject(parent)
    , m_DC_model(0)
    , m_Prn_model(0)
    , m_Tmpl_model(0)
    , m_Mandats_model(0)
    , m_Stamp_model(0)
    , m_LocalClient(0)
    , stopLaunch(false)
    , currentUserName(QString())
    , currentUserMandat(QString())
    , link_name(QString())
    , gatekeeper_bin(QString())
{
    qRegisterMetaType<VPrn::MessageType>("MessageType");
    link_name      = link;
    gatekeeper_bin = gk_bin;    
}


Engine::~Engine()
{
    if (m_LocalClient){
        m_LocalClient->disconnectFromServer();
    }
}

void Engine::prepareFileToPrint(const QString & file_name)
{
    // Запись в сокет сообщения требую  преобразовать ps -> pdf
    Message msg(this);
    msg.setType(VPrn::Que_Convert2Pdf);
    msg.setMessageData( file_name.toUtf8() ); // Пробразуем в QByteArray
    sendMessage2LocalSrv(msg);
}

//-------------------------------- PUBLIC SLOTS  -------------------------------
void Engine::setAuthData(const QString &login,const QString &mandat)
{
    currentUserName = login;
    currentUserMandat = mandat;
    emit ReciveUserName();
    emit ReciveUserMandat();

    // Запись в сокет сообщения запрос доступных уровней секретности для
    Message msg(this);
    msg.setType(VPrn::Que_SEC_LEVEL);
    msg.setMessageData( currentUserMandat.toUtf8() ); // Пробразуем в QByteArray
    sendMessage2LocalSrv(msg);
}

void Engine::launchAndConnect()
{
    if (!link_name.isEmpty() &&  QFile::exists(gatekeeper_bin)){
        if (!m_LocalClient){

            m_LocalClient = new mySocketClient();
            connect (m_LocalClient, SIGNAL(checkPointChanged(MyCheckPoints)),
                     this, SLOT(do_checkPointChanged(MyCheckPoints))
                     );
            connect (m_LocalClient, SIGNAL(messageReady( const Message &)),
                     this, SLOT(parseMessage(const Message &))
                     );
        }
        // Устанавливаем соединение с локальным сервером
        m_LocalClient->connectToServer(link_name);

    }else{
        emit error("configError",
                   QObject::trUtf8("Ошибка в файле конфигурации. Проверьте параметры:\n"
                                   " Раздел [SERVICE] параметр link_name\n"
                                   " Раздел [USED_DIR_FILE] параметр gatekeeper_bin\n%1")
                   .arg(QString(Q_FUNC_INFO))
                   );
    }
}

//-------------------------------- PRIVATE SLOTS -------------------------------
void Engine::do_checkPointChanged(MyCheckPoints r_cpoint)
{
    Message msg(this);
    if (r_cpoint == VPrn::loc_Connected){
        emit LocalSrvRegistr();

        //Запрос авторизации
        msg.setType(VPrn::Que_Register);
        //msg.setMessageData( clientName.toUtf8() ); // Пробразуем в QByteArray
        sendMessage2LocalSrv(msg);
    }
    if (r_cpoint ==  VPrn::loc_ServerNotFound && !stopLaunch){
        QProcess proc;
        // Запуск процесса старта сервера
        if (proc.startDetached (gatekeeper_bin)){
            // Попытка номер 2
            stopLaunch = true;
        }else{
            emit error("appNotStarted", proc.errorString() );
        }
        QTimer::singleShot(3000,this,SLOT(launchAndConnect()));
    }
}

void Engine::parseMessage(const Message &r_msg)
{
    Message msg(this);
    msg.clear();
    QString str;

    MessageType mType = r_msg.type();
    switch (mType){
    case VPrn::Ans_Register: {
            // Клиент зарегистрирован сохраним полученный от GateKeeper уникальный номер
            client_uuid.clear();
            client_uuid.append(r_msg.messageData());
            emit LocalSrvRegistr();
            // Запрос у локального сервера его состояния возможные ответы:
            // Не готов к работе (Подробности в теле сообщения)
            // Готов к работе (Есть auth данные пользователя, есть связь с демоном)
            // Частично готов (Есть имя пользователя,есть связь с демоном, нет мандата
            msg.setType(VPrn::Que_ServerStatus);
            sendMessage2LocalSrv(msg);
        }
        break;
    case VPrn::Ans_SrvStatusFullReady:
        {
            str.append(r_msg.messageData());
            // Разберем ответ сервера, в формате : имя;:;мандат
            QRegExp rx("(.+);:;(.+)");
            //rx.setMinimal(true);
            if(rx.indexIn(str) != -1){
                currentUserName   = rx.cap(1);
                currentUserMandat = rx.cap(2);
                //emit gk_fullReady(currentUserName,currentUserMandat);
                //Запрос уровней секретности для мандата
                setAuthData(currentUserName,currentUserMandat );
                afterConnectSteps();
            }else{
                emit error("localSocketError",
                           QObject::trUtf8("Ошибка при получении имени и мандата текщего пользователя")
                           .arg(QString(Q_FUNC_INFO))
                           );
            }
        }
        break;
    case VPrn::Ans_SrvStatusNotReady:{
            str.append(r_msg.messageData());
            emit gk_notReady(QObject::trUtf8("GateKeeper не готов к работе по причине [%1]")
                             .arg(str)
                             );
        }
        break;
    case VPrn::Ans_SrvStatusPartReady:{
            str.append(r_msg.messageData());
            emit needAuthUser(str);
            afterConnectSteps();
        }
        break;
    case VPrn::Ans_TemplateNotFound:
        str.append( r_msg.messageData() );
        emit MergeDocWithTemplates( false, str);
        break;
    case VPrn::Ans_GiveMeTemplatesList:{
            // Получили данные запишем в модель
            saveTemplatesListToModel( r_msg.messageData() );
        }
        break;
    case Ans_SourceDocNotFound:{
            // Исходный документ не найден или не верного формата,
            // в теле сообщения подробности
            str.append( r_msg.messageData() );
            emit MergeDocWithTemplates( false, str);
        }
        break;
    case VPrn::Ans_CreateFormatedDoc:{

            emit MergeDocWithTemplates( true,
                                        QObject::trUtf8("Применение шаблона к текущему документу, успешно завершено!"));
        }
        break;
    case VPrn::Ans_ConvertFormatedDocToPng:{
            //emit PicturesList( r_msg.messageDataList() );
        }
        break;
    case VPrn::GoodBay:{
            str.append(r_msg.messageData());
            emit reciveGoodBayMsg(str);
        }
        break;
    case VPrn::Ans_MB_NOT_EXIST:{
            //Документ не зарегистрирован в БД учета необходима его регистрация, и можно
            // формировать стр. предпросмотра
            //emit needRegisterDocInBase();
        }
        break;
    case VPrn::Ans_MB_EXIST_AND_BRAK:{
        }
        break;
    case VPrn::Ans_MB_EXIST_AND_NOT_BRAK:{
        }
        break;
    case VPrn::Ans_MB_LIST:{
        }
        break;
    case VPrn::Ans_RegisterDocInBase:{
            //Документ зарегистрирован в БД УЧЕТА
            str.append(r_msg.messageData());
            //emit RegisterDocInBase( true, str);
        }
        break;
    case VPrn::Ans_PRINT_ALLOWED:{
            str.append(r_msg.messageData());
            //emit authToDevice( true,str );
        }
        break;
    case VPrn::Ans_PRINT_DENIED:{
            str.append(r_msg.messageData());
            //emit authToDevice( false,str );
        }
        break;
    case VPrn::Ans_PRINTER_NOT_FOUND:{
            str.append(r_msg.messageData());
            //emit authToDevice( false,str );
        }
        break;
    case VPrn::Ans_Convert2PdfFinish:
        emit doc_converted();
        break;
    case VPrn::Ans_PageCounting:{
            str.append(r_msg.messageData()); // Число страниц в str
            int pCnt = str.toInt();
            emit getPagesCount(pCnt);
        }
        break;
    case VPrn::Ans_PRINTER_LIST:{
            str.append(r_msg.messageData());
            savePrintersListToModel( str );
            emit RecivePrintersList();
        }
        break;
    case VPrn::Ans_STAMP_LIST:{
            // Получили названия уровней секретности, сохраним в модель
            str.append(r_msg.messageData());
            //secLevelList << str.split(";:;");
            emit ReciveSecLevelList();
            //Запрос списка принтеров доступных пользоваетелю
            msg.setType(VPrn::Que_GET_PRINTER_LIST);
            sendMessage2LocalSrv(msg);
        }
        break;
    case VPrn::Err_Message:{
            // Получили сообщение об ошибке Формат: КОД~~описание
        }
        break;
    }
}

//-------------------------------------- PRIVATE ----------------------------------------
void Engine::sendMessage2LocalSrv(const Message &s_msg)
{
    if (m_LocalClient){
        m_LocalClient->sendMessage(s_msg);
    }
}

void Engine::afterConnectSteps()
{
    Message msg(this);
    msg.setType( VPrn::Que_GiveMeTemplatesList );
    sendMessage2LocalSrv( msg );

    emit RemoteDemonRegistr();
}

void Engine::savePrintersListToModel(const QString &prn_list)
{
    // Разберем ответ сервера, в формате: Название принтера;:;ip;;:имя очереди###
    QRegExp rx("(.+);:;(.+);:;(.+)");
    //rx.setMinimal(true);
    QString p_name;
    QString p_ip;
    QString p_qqueue;

    QStringList printers_item;
    printers_item = prn_list.split("###");
    for (int i = 0; i < printers_item.size(); i++) {
        if(rx.indexIn( printers_item.at(i) ) != -1){
            p_name   = rx.cap(1);
            p_ip     = rx.cap(2);
            p_qqueue = rx.cap(3);

            m_Prn_model->setItem(i, 0, new QStandardItem(p_name));
            m_Prn_model->setItem(i, 1, new QStandardItem(p_ip));
            m_Prn_model->setItem(i, 2, new QStandardItem(p_qqueue));
        }
    }
}

void Engine::saveTemplatesListToModel(QByteArray tmpl_list)
{
    if ( m_Tmpl_model && !tmpl_list.isEmpty() ){
        //Обработка данных
        QDataStream in(&tmpl_list, QIODevice::ReadOnly );
        in.setVersion(QDataStream::Qt_3_0);
        int rows;
        int columns;
        in >> rows;
        in >> columns;
        QString txt;
        for (int i=0;i<rows;i++){
            for (int j=0;j<columns;j++){
                in >> txt;
                m_Tmpl_model->setItem(i,j,new QStandardItem(txt));
                txt.clear();
            }
        }
    }
}
