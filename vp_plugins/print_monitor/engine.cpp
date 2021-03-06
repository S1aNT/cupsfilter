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
#include <QtGui/QStringListModel>
#include <QtGui/QStandardItemModel>
#include <QtCore/QModelIndex>
#include <QtCore/QDataStream>
#include <QPair>

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

void Engine::needRestart()
{
    QString mb;
    QString cur_copy;
    QString total_copy;
    QModelIndex indx;

    //Получим данные из модели
    indx = m_DC_model->index(0,VPrn::cards_MB_NUMBER);
    mb   = m_DC_model->data(indx,Qt::DisplayRole).toString() ;

    indx = m_DC_model->index(0,VPrn::cards_CURRENT_COPY);
    cur_copy = m_DC_model->data(indx,Qt::DisplayRole).toString();

    indx = m_DC_model->index(0,VPrn::cards_COPY_COUNT);
    total_copy = m_DC_model->data(indx,Qt::DisplayRole).toString();

    QString str = tr("%1;:;%2;:;%3;:;")
                  .arg(mb).arg(cur_copy).arg(total_copy);

    indx = m_DC_model->index(0,VPrn::cards_SELECT_ALL_COPY);
    if (m_DC_model->data(indx,Qt::DisplayRole).toBool()){
        str.append("true");
    }else{
        str.append("false");
    }

    //Отправим киперу запрос на перезапуск процесса
    Message msg(this);
    msg.setType( VPrn::Que_UserDemands2Restart );
    msg.setMessageData( str.toUtf8() );
    sendMessage2LocalSrv( msg );
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
        emit error(VPrn::ConfigError,
                   QObject::trUtf8("Ошибка в файле конфигурации. Проверьте параметры:\n"
                                   " Раздел [SERVICE] параметр link_name\n"
                                   " Раздел [USED_DIR_FILE] параметр gatekeeper_bin\n%1")
                   .arg(QString(Q_FUNC_INFO))
                   );
    }
}

void Engine::authUserToPrinter()
{
    // Запись в сокет сообщения запрос авторизации пользователя на доступ к ресурсу
    Message msg(this);
    msg.setType( VPrn::Que_AUTHOR_USER  );

    // Определим  какой принтер выбрал юзер на начальном шаге мастера
    msg.setMessageData( findPrinterInModel().toUtf8() ); // Пробразуем в QByteArray
    sendMessage2LocalSrv(msg);
}

void Engine::checkMB()
{
    QString sql_query;
    /**
      * @brief Формируем sql запрос проверяющий:
      * - данный документ (МБ,номер экз.) не зарегистрирован в БД
      * - данный документ (МБ,номер экз.) зарегистрирован в БД, но помечен как брак
      */
    Message msg(this);
    msg.setType( VPrn::Que_IS_MB_EXIST  );
    msg.setMessageData( sql_query.toUtf8() );
    sendMessage2LocalSrv(msg);
}

void Engine::registerMB()
{
    QString sql_query;
    /**
      * @brief Формируем sql запрос регистрирующий документ в БД учета
      * - данный документ (МБ,номер экз.) не зарегистрирован в БД
      * - данный документ (МБ,номер экз.) зарегистрирован в БД, но помечен как брак
      */
    Message msg(this);
    msg.setType( VPrn::Que_RegisterDocInBase  );
    msg.setMessageData( sql_query.toUtf8() );
    sendMessage2LocalSrv(msg);
}

void Engine::do_select_mode(int mode)
{
    QByteArray field_data;
    Message msg(this);

    // Формируем набор данных из модели для передачи локальному серверу
    field_data = getAllFieldData();
    bool Ok =true;
    {

        switch (mode){
        case VPrn::pre_FullMode:
            msg.setType( VPrn::Que_CreateFormatedFullDoc );
            break;
        case VPrn::pre_PartMode:
            msg.setType( VPrn::Que_CreateFormatedPartDoc );
            break;
        case VPrn::pre_ClearPrintMode:
            msg.setType( VPrn::Que_CreateFormatedFullDocAndPrint );
            break;
        default:
            emit error(VPrn::notDefainedMode,
                       QObject::trUtf8("Выбран не существующий режим предпросмотра/печати.\n%1")
                       .arg(QString(Q_FUNC_INFO))
                       );
            Ok &= false;
            break;
        }
    }
    if (Ok){
        msg.setMessageData( field_data );
        sendMessage2LocalSrv(msg);
    }
}

void Engine::do_printCurrentDoc()
{
    Message msg(this);
    msg.setType( VPrn::Que_PrintCurrentFormatedDoc );
    //msg.setMessageData ( findPrinterInModel().toUtf8() );
    sendMessage2LocalSrv( msg );
}

void Engine::markCurrentDocBrack()
{
    Message msg(this);
    msg.setType( VPrn::Ans_UserNeedMarkCopies );
    msg.setMessageData ( QString("BRAK").toUtf8() );
    sendMessage2LocalSrv( msg );

}
void Engine::markCurrentDocClear()
{
    Message msg(this);
    msg.setType( VPrn::Ans_UserNeedMarkCopies );
    msg.setMessageData ( QString("CLEAR").toUtf8() );
    sendMessage2LocalSrv( msg );

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
            emit error(VPrn::appNotStarted,QString("%1\n%2").arg(proc.errorString()).arg(QString(Q_FUNC_INFO)) );
        }
        QTimer::singleShot(3000,this,SLOT(launchAndConnect()));
    }
}

void Engine::parseMessage(const Message &r_msg)
{
    Message msg(this);
    msg.clear();
    QString str;

    MessageType mType = r_msg.getType();
    switch (mType){
    case VPrn::Que_BeginPrintCopies:{	
            str = QObject::trUtf8("Начата печать документа: ");
            str.append(r_msg.getMessageData());

            emit showInfoMsg(str);
            qDebug() << "Recive msg: VPrn::Que_BeginPrintCopies";
            // продолжим печать
            do_printCurrentDoc();
        }
        break;
    case VPrn::Que_UserNeedFlipPages:{
            qDebug() << "Recive msg: VPrn::Que_UserNeedFlipPages";
            str = QObject::trUtf8("Окончена печать лицевых сторон, необходимо перевернуть листы!");
            emit showInfoMsg(str);
            emit UserNeedFlipPages();
        }
        break;
    case VPrn::Que_UserNeedCheckLastPage:{
            qDebug() << "Recive msg: VPrn::Que_UserNeedCheckLastPage";
            str = QObject::trUtf8("Окончена печать обратных сторон, необходимо проверить последний лист для печати фонарика!");
            emit showInfoMsg(str);
            emit UserNeedCheckLastPage();
        }
        break;
    case VPrn::Que_UserNeedMarkCopies:{
            str = QObject::trUtf8("Окончена печать, необходимо утвердить документ!");
            emit showInfoMsg(str);
            emit UserNeedMarkCopies( r_msg.getMessageData() );
        }
        break;
    case VPrn::Ans_Register: {
            // Клиент зарегистрирован сохраним полученный от GateKeeper уникальный номер
            client_uuid.clear();
            client_uuid.append(r_msg.getMessageData());
            if (m_DC_model){
                m_DC_model->setItem(0,VPrn::cards_ID,
                                    new QStandardItem(client_uuid)
                                    );
            }
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
            str.append(r_msg.getMessageData());
            // Разберем ответ сервера, в формате : имя;:;мандат
            QRegExp rx("(.+);:;(.+)");
            //rx.setMinimal(true);
            if(rx.indexIn(str) != -1){
                currentUserName   = rx.cap(1);
                currentUserMandat = rx.cap(2);
                //Запрос уровней секретности для мандата
                setAuthData(currentUserName,currentUserMandat );
                afterConnectSteps();
            }else{
                emit error(VPrn::SocketError,
                           QObject::trUtf8("Ошибка при получении имени и мандата текщего пользователя")
                           .arg(QString(Q_FUNC_INFO))
                           );
            }
        }
        break;
    case VPrn::Ans_SrvStatusNotReady:{
            str.append(r_msg.getMessageData());
            emit gk_notReady(QObject::trUtf8("GateKeeper не готов к работе по причине [%1]")
                             .arg(str)
                             );
        }
        break;
    case VPrn::Ans_SrvStatusPartReady:{
            str.append(r_msg.getMessageData());
            emit needAuthUser(str);
            afterConnectSteps();
        }
        break;
    case VPrn::Ans_TemplateNotFound:{
            str.append( r_msg.getMessageData() );
            emit MergeDocWithTemplates( false, str);
        }
        break;
    case VPrn::Ans_GiveMeTemplatesList:{
            // Получили данные запишем в модель
            saveTemplatesListToModel( r_msg.getMessageData() );
        }
        break;
    case Ans_SourceDocNotFound:{
            // Исходный документ не найден или не верного формата, в теле сообщения подробности
            str.append( r_msg.getMessageData() );
            emit MergeDocWithTemplates( false, str);
        }
        break;
    case VPrn::Ans_CreateFormatedDoc:{
            emit MergeDocWithTemplates( true,
                                        QObject::trUtf8("Применение шаблона к текущему документу, успешно завершено!"));
        }
        break;
    case VPrn::Ans_ConvertFormatedDocToPng:{
            emit PicturesList( r_msg.getMessageDataList() );
        }
        break;
    case VPrn::GoodBay:{
            str.append(r_msg.getMessageData());
            emit reciveGoodBayMsg(str);
        }
        break;
    case VPrn::Ans_MB_NOT_EXIST:{
            //Документ не зарегистрирован в БД УЧЕТА
            // Формируем запрос на регистрацию документа
            registerMB();
        }
        break;
    case VPrn::Ans_MB_EXIST_AND_BRAK:{
            //Документ зарегистрирован в БД УЧЕТА
            str.append(r_msg.getMessageData());
            emit RegisterDocInBase( true, str);
        }
        break;
    case VPrn::Ans_MB_EXIST_AND_NOT_BRAK:{
            //Документ зарегистрирован в БД УЧЕТА
            str.append(r_msg.getMessageData());
            emit RegisterDocInBase( false, str);
        }
        break;
    case VPrn::Ans_RegisterDocInBase:{
            //Документ зарегистрирован в БД УЧЕТА
            str.append(r_msg.getMessageData());
            emit RegisterDocInBase( true, str);
        }
        break;
    case VPrn::Ans_MB_LIST:{
        }
        break;
    case VPrn::Ans_PRINT_ALLOWED:{
            str.append(r_msg.getMessageData());
            emit authToDevice( true,str );
        }
        break;
                case VPrn::Ans_PRINT_DENIED:{
                        str.append(r_msg.getMessageData());
                        emit authToDevice( false,str );
                    }
                    break;
                case VPrn::Ans_PRINTER_NOT_FOUND:{
                        str.append(r_msg.getMessageData());
                        emit authToDevice( false,str );
                    }
                    break;
                case VPrn::Ans_Convert2PdfFinish:{
                        emit doc_converted();
                    }
                    break;
                case VPrn::Ans_PageCounting:{
                        str.append(r_msg.getMessageData()); // Число страниц в str
                        int pCnt = str.toInt();
                        setPageCountInModel(pCnt);
                    }
                    break;
                case VPrn::Ans_PRINTER_LIST:{
                        str.append(r_msg.getMessageData());
                        savePrintersListToModel( str );
                        emit RecivePrintersList();
                    }
                    break;
                case VPrn::Ans_STAMP_LIST:{
                        // Получили названия уровней секретности, сохраним в модель
                        str.append(r_msg.getMessageData());
                        setSecLevelList(QStringList() << str.split(";:;") );
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
                default:
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
    // Разберем ответ сервера, в формате: Название принтера;:;Название принтера2;;:Название принтера_
    
    QStringList printers_item = prn_list.split(";:;");
    for (int i = 0; i < printers_item.size(); i++) {
	m_Prn_model->setItem(i, 0, new QStandardItem(printers_item.at(i)));
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

void Engine::setSecLevelList(QStringList  &list )
{
    if (list.size() >0  && m_Stamp_model !=0){
        m_Stamp_model->setStringList( list );
    }
}

void Engine::setPageCountInModel(int pCnt)
{
    m_DC_model->setItem(0,VPrn::cards_PAGE_COUNT,
                        new QStandardItem(QObject::trUtf8("%1").arg(pCnt,0,10))
                        );
}

QString Engine::findPrinterInModel()
{
    QString device_profile;
    for (int i=0; i< m_Prn_model->rowCount();i++){
        QModelIndex index = m_Prn_model->index(i,3);
        bool sel_flag = m_Prn_model->data(index,Qt::DisplayRole).toBool();
        if (sel_flag){
            QModelIndex index_name = m_Prn_model->index(i, 0);
            //QModelIndex index_qqueue = m_Prn_model->index(i, 2);

            device_profile = m_Prn_model->data(index_name, Qt::DisplayRole).toString();
            break;

        }
    }
    return device_profile;
}

QByteArray Engine::getAllFieldData()
{
    QMap <int,QString> m_tagValue;
    QModelIndex indx;
    QByteArray fields_data;
    QList <int> doc_copyes;
    QString t_fileName;

    for (int j=0;j<m_DC_model->columnCount();j++){
        indx = m_DC_model->index(0,j);
        QVariant cell_data = m_DC_model->data(indx,Qt::DisplayRole);
        QString tmp_item = cell_data.toString();
        //qDebug() <<Q_FUNC_INFO << "\ncell_data: " << tmp_item;
        if (!cell_data.isNull()){
            switch (j){
            case VPrn::cards_DOC_NAME:
                m_tagValue[VPrn::cards_DOC_NAME] = cell_data.toString();
                break;
            case VPrn::cards_MB_NUMBER:
                m_tagValue[VPrn::cards_MB_NUMBER] = cell_data.toString();
                break;
            case VPrn::cards_STAMP:
                m_tagValue[VPrn::cards_STAMP] = findStampInModel(cell_data.toInt());
                break;
            case VPrn::cards_PUNKT:
                m_tagValue[VPrn::cards_PUNKT] = cell_data.toString();
                break;
            case VPrn::cards_PAGE_COUNT:
                m_tagValue[VPrn::cards_PAGE_COUNT] = cell_data.toString();
                break;
            case VPrn::cards_EXECUTOR:
                m_tagValue[VPrn::cards_EXECUTOR] = cell_data.toString();
                break;
            case VPrn::cards_PRINTMAN:
                m_tagValue[VPrn::cards_PRINTMAN] = cell_data.toString();
                break;
            case VPrn::cards_PHONE:
                m_tagValue[VPrn::cards_PHONE] = cell_data.toString();
                break;
            case VPrn::cards_INV_NUMBER:
                m_tagValue[VPrn::cards_INV_NUMBER] = cell_data.toString();
                break;
            case VPrn::cards_PRINT_DATE:
                m_tagValue[VPrn::cards_PRINT_DATE] = cell_data.toString();
                break;
            case VPrn::cards_RECIVER_1:
                m_tagValue[VPrn::cards_RECIVER_1] = cell_data.toString();
                doc_copyes.append(VPrn::FirstPage);
                break;
            case VPrn::cards_RECIVER_2:
                m_tagValue[VPrn::cards_RECIVER_2] = cell_data.toString();
                doc_copyes.append(VPrn::FirstPageN2);
                break;
            case VPrn::cards_RECIVER_3:
                m_tagValue[VPrn::cards_RECIVER_3] = cell_data.toString();
                doc_copyes.append(VPrn::FirstPageN3);
                break;
            case VPrn::cards_RECIVER_4:
                m_tagValue[VPrn::cards_RECIVER_4] = cell_data.toString();
                doc_copyes.append(VPrn::FirstPageN4);
                break;
            case VPrn::cards_RECIVER_5:
                m_tagValue[VPrn::cards_RECIVER_5] = cell_data.toString();
                doc_copyes.append(VPrn::FirstPageN5);
                break;
            case VPrn::cards_TEMPLATE_NAME:
                t_fileName =  findTemplatesFilePathInModel(cell_data.toInt());
                break;
            case VPrn::cards_CURRENT_COPY:
                m_tagValue[VPrn::cards_CURRENT_COPY] = cell_data.toString();
                break;
            default:
                break;
            }
        }
    }

    m_tagValue[VPrn::cards_COPY_COUNT]= QString("%1")
                                        .arg(doc_copyes.count(),0,10) ;


    //Очистим массивчик
    fields_data.clear();
    QDataStream out(&fields_data, QIODevice::WriteOnly );
    out.setVersion(QDataStream::Qt_3_0);
    //Запишем пару выбранный пользователем экземпляр,всего экземпляров
    //out << qMakePair(cur_copyes,all_copyes);
    out <<  doc_copyes;
    // Запишем выбранный пользователем шаблон
    out << t_fileName;
    // save map
    out << m_tagValue;

    return fields_data;
}


QString Engine::findTemplatesFilePathInModel ( int id )
{
    QModelIndex indx = m_Tmpl_model->index(id,VPrn::metaInfo_fname);
    return m_Tmpl_model->data(indx,Qt::DisplayRole).toString();
}

QString Engine::findStampInModel ( int id )
{
    QModelIndex indx = m_Stamp_model->index(id,0);
    return m_Stamp_model->data(indx,Qt::DisplayRole).toString();
}
