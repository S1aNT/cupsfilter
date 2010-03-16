#ifndef MY_TYPES_H
#define MY_TYPES_H

#include <QMetaType>

namespace VPrn{

#define POINT_TO_CM(cm) ((cm)/28.3465058)
#define POINT_TO_MM(mm) ((mm)/2.83465058)     ////////  0.352777778
#define POINT_TO_DM(dm) ((dm)/283.465058)
#define POINT_TO_INCH(inch) ((inch)/72.0)

#define MM_TO_POINT(mm) ((mm)*2.83465058)
#define CM_TO_POINT(cm) ((cm)*28.3465058)     ///// 28.346456693
#define DM_TO_POINT(dm) ((dm)*283.465058)
#define INCH_TO_POINT(inch) ((inch)*72.0)  

    static const int ObjectName = 0;
    static const int format = 13;

    static const char PARTSEPARATOR = ':';
    static const char MESSAGESEPARATOR = '|';

    enum pSizeColumnOrder{
        pSize_id     = 0, // ID
        pSize_page   = 1, // размер листа
        pSize_code   = 2, // Код
        pSize_width  = 3, // Ширина (мм)
        pSize_height = 4  // Высота (мм)                       
                   };

    enum tInfoColumnOrder{
        tInfo_id     = 0,  //ID
        tInfo_name   = 1,  //Имя шаблона
        tInfo_desc   = 2,  //Описание шаблона;
        tInfo_pageID = 3,  //Ссылка на таблицу размеры страниц
        tInfo_angle  = 4,  //Поворот (град.)
        tInfo_ctime  = 5,  //Время создания
        tInfo_mtime  = 6,  //Время изменения
        tInfo_author = 7, //Автор
        tInfo_mtop   = 8, //Отступ сверху (мм)
        tInfo_mbottom = 9,//Отступ снизу (мм)
        tInfo_mleft   = 10,//Отступ слева (мм)
        tInfo_mright  = 11, //Отступ справа (мм)
        tInfo_p_width = 12,
        tInfo_p_height= 13
                    };

    enum pageDetailColumnOrder{
        PD_id         = 0,  // ID
        PD_p_number   = 1,  // Порядковый номер в шаблоне
        PD_p_name     = 2,   // Имя страницы
        PD_p_visible  = 3   // Видна страница да/нет 1/0
                    };

    enum elemColumnOrder{
        elem_id    = 0,  //ID
        elem_text  = 1,  //Текст отображаемый на экране и на принтере
        elem_tag   = 2,  //Тэг с которым работает программа
        elem_pos_x = 3,  //Позиция элемента на сцене х
        elem_pos_y = 4,  //Позиция элемента на сцене у
        elem_color = 5,  //Цвет шрифта
        elem_font  = 6,  //Шрифт
        elem_angle = 7,  //Угол поворота относиттельно сцены
        elem_border= 8,  // исовать границу элемента, да/нет
        elem_img_data = 9, // Если эемент картинка то тут будет записанны данные в виде бинарных данных
        elem_always_view = 10, // Всегда отображаемый Да/нет
        elem_p_number    = 11,  // Порядковый номер страницы 0-7 на которой виден элемент
        elem_text_img    = 12  // Тип элемента текст или картинка(1/0)
                       };

    enum pluginsError {
        NoError,
        FileNotFound,
        FileIOError,
        UnknownError,
        DriverNotLoad,
        NotConnectedToDB,
	DBOpeningError,
        SQLQueryError,
        SQLCommonError,
        InternalPluginError,
        AuthCommonError,
        NetworkTransError
    };

    enum pageNumbers {
        FirstPage    = 0,
        FirstPageN2  = 1,
        FirstPageN3  = 2,
        FirstPageN4  = 3,
        FirstPageN5  = 4,
        SecondPage   = 5,
        ThirdPage    = 5,
        FourthPage   = 7,
    };

    enum trayIcons {
        InfoType,
        WarnType,
        CritType
    };

    enum trayStatus {
        gk_Started,
        gk_UserAuthorized,
        gk_UserLogin,
        gk_DoPrintJob,
        gk_DoReportJob,
        gk_WarningState,
        gk_ErrorState
    };

    enum MessageType {
        Que_Register     = 5010,     // Запрос на регистрацию
        Ans_Register     = 5015,     // Ответ о регистрации
        Que_ServerStatus = 5020,     // Запрос у сервера его состояния
        Ans_ServerStatus = 5025,     // Ответ сервера его состояние
        GiveAuthData     = 5030,     // Запрос индетификационных данных от сервера
        NoType           = 99999

    };
    
    enum ProtocolError { 
        InvalidFormat =0, 
        InvalidVersion = 1 
    };

    enum LocalServerState{
        InitStep,
        ReadyForJob,        // Готов к работе
        DoPrintJob,         //
        DoSQLJob,           //


        NotListenError,     // Не могу занять порт на прослушивание, значит уже есть копия
        PluginNotLoadError, //
        NetworkCommonError,   //
        DemonFreeSpaceError,//

        DemonResponceFailed,//
        DemonAuthFailed,

    };

    enum LocalClientState{
        Connected,
        Disconnected,
        Auhtorized,
        HostNotFound,
        InternalError
    };


    enum{
       REGISTER_ANS     = 1010,
       PRINT_ALLOWED    = 1100, // печать разрешена
       PRINT_DENIED     = 1101, // печать запрещена
       PRINTER_NOT_FOUND   = 1102, //  принтер не найден


       PRINTER_LIST_ANS = 1400,
       PRINTER_LIST_EMPTY=1401,
       MB_LIST_ANS = 1200,
       MB_EXIST_AND_BRAK_ANS =1205,
       MB_EXIST_AND_NOT_BRAK_ANS =1210,
       MB_NOT_EXIST_ANS=1220,
       STAMP_LIST_ANS       = 1300, // Список названий уровней секретности
       MANDAT_LIST_ANS      = 1600, // Список мандатов к которым допущен пользоватль
       MANDAT_LIST_EMPTY_ANS= 1601  // У данного пользователя нет ни одного мадата

    };

    enum{
       REGISTER_CMD         = 10,

       AUTHOR_USER          = 100,
       GET_MB_PRINTS_TODAY_CMD =200,

       RAW_SQL_CMD          = 202,
       USER_SAY_DOC_GOOD    = 203,
       USER_SAY_DOC_BAD     = 204,

       GET_SEC_LEVEL_CMD    = 300, //запрос к демону на получение списка уровней секретности

       GET_PRINTER_LIST_CMD = 400,
       IS_MB_EXIST_CMD      = 500,
       GET_MANDAT_LIST_CMD  = 600,
       DISCONNECT           = 5000

    };


}

Q_DECLARE_METATYPE(VPrn::LocalServerState);
Q_DECLARE_METATYPE(VPrn::LocalClientState);
Q_DECLARE_METATYPE(VPrn::ProtocolError);
Q_DECLARE_METATYPE(VPrn::MessageType);
Q_DECLARE_METATYPE(VPrn::trayIcons);
Q_DECLARE_METATYPE(VPrn::trayStatus);
Q_DECLARE_METATYPE(VPrn::pSizeColumnOrder);
Q_DECLARE_METATYPE(VPrn::tInfoColumnOrder);
Q_DECLARE_METATYPE(VPrn::elemColumnOrder);
Q_DECLARE_METATYPE(VPrn::pageDetailColumnOrder);
Q_DECLARE_METATYPE(VPrn::pageNumbers);
#endif

