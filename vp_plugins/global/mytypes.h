#ifndef MY_TYPES_H
#define MY_TYPES_H

#include <QMetaType>

namespace VPrn{

#define MY_DEBUG 1
#define POINT_TO_CM(cm) ((cm)/28.3465058)
#define POINT_TO_MM(mm) ((mm)/2.83465058)     ////////  0.352777778
#define POINT_TO_DM(dm) ((dm)/283.465058)
#define POINT_TO_INCH(inch) ((inch)/72.0)

#define MM_TO_POINT(mm) ((mm)*2.83465058)
#define CM_TO_POINT(cm) ((cm)*28.3465058)     ///// 28.346456693
#define DM_TO_POINT(dm) ((dm)*283.465058)
#define INCH_TO_POINT(inch) ((inch)*72.0)


static const int ObjectName = 0;


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
        AuthCommonError
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
}


Q_DECLARE_METATYPE(VPrn::trayIcons);
Q_DECLARE_METATYPE(VPrn::trayStatus);
Q_DECLARE_METATYPE(VPrn::pSizeColumnOrder);
Q_DECLARE_METATYPE(VPrn::tInfoColumnOrder);
Q_DECLARE_METATYPE(VPrn::elemColumnOrder);
Q_DECLARE_METATYPE(VPrn::pageDetailColumnOrder);
Q_DECLARE_METATYPE(VPrn::pageNumbers);
#endif

