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

enum pSizeColumnOrder{
        pSize_id     = 0, // ID
        pSize_page   = 1, // Размер листа
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
            tInfo_mright  = 11 //Отступ справа (мм)

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
        InternalPluginError
};

enum pageType{
     FirstPage  = 1,
     SecondPage = 2,
     ThirdPage  = 3,
     FourthPage = 4
};

}


Q_DECLARE_METATYPE(VPrn::pSizeColumnOrder);
Q_DECLARE_METATYPE(VPrn::tInfoColumnOrder);

#endif

