#ifndef TECH_GLOBAL_H
#define TECH_GLOBAL_H

#include <QString>

namespace SafeVirtualPrinter{

#define POINT_TO_CM(cm) ((cm)/28.3465058)
#define POINT_TO_MM(mm) ((mm)/2.83465058)     ////////  0.352777778
#define POINT_TO_DM(dm) ((dm)/283.465058)
#define POINT_TO_INCH(inch) ((inch)/72.0)

#define MM_TO_POINT(mm) ((mm)*2.83465058)
#define CM_TO_POINT(cm) ((cm)*28.3465058)     ///// 28.346456693
#define DM_TO_POINT(dm) ((dm)*283.465058)
#define INCH_TO_POINT(inch) ((inch)*72.0)

typedef struct{
    int version; 	// версия шаблона
    QString date_time;      // дата и время создания
    QString t_author;         // автор шаблона
    QString t_name; // имя шаблона
    QString t_desc;    // описание шаблона
    QString p_size;     // размер бумаги (Для человека)
    bool  page_orient;      // ориентация страницы Книжная/альбомная true/false
    qreal page_height;      // высота листа в  [мм -> point] (для компьютера)
    qreal page_width;       // ширина листа в  [мм -> point] (для компьютера)
    qreal m_top;       // отступ сверху в [мм -> point] (для компьютера)
    qreal m_bottom;    // отступ снизу в [мм -> point] (для компьютера)
    qreal m_left;      // отступ слева в [мм -> point] (для компьютера)
    qreal m_right;     // отступ справа в [мм -> point] (для компьютера)
    int firstPageElemCount;    // число элементов на первой странице шаблона
    int secondPageElemCount;   // число элементов на второй странице шаблона
    int thirdPageElemCount;    // число элементов на третьей странице шаблона
    int fourthPageElemCount;   // число элементов на четвертой странице шаблона

}templates_info_t;

static const int ObjectName = 0;

typedef enum{
   REGISTER_CMD		= 10,

   AUTHOR_USER          = 100,
   GET_MB_PRINTS_TODAY_CMD =200,

   RAW_SQL_CMD		= 202,
   USER_SAY_DOC_GOOD	= 203,
   USER_SAY_DOC_BAD	= 204,

   GET_SEC_LEVEL_CMD	= 300, //запрос к демону на получение списка уровней секретности

   GET_PRINTER_LIST_CMD = 400,
   IS_MB_EXIST_CMD	= 500,
   GET_MANDAT_LIST_CMD	= 600,
   DISCONNECT		= 5000

};

typedef enum{
   REGISTER_ANS	    = 1010,
   PRINT_ALLOWED    = 1100, // печать разрешена
   PRINT_DENIED     = 1101, // печать запрещена
   PRINTER_NOT_FOUND   = 1102, //  принтер не найден


   PRINTER_LIST_ANS = 1400,
   PRINTER_LIST_EMPTY=1401,
   MB_LIST_ANS = 1200,
   MB_EXIST_AND_BRAK_ANS =1205,
   MB_EXIST_AND_NOT_BRAK_ANS =1210,
   MB_NOT_EXIST_ANS=1220,
   STAMP_LIST_ANS	= 1300,	// Список названий уровней секретности
   MANDAT_LIST_ANS	= 1600, // Список мандатов к которым допущен пользоватль
   MANDAT_LIST_EMPTY_ANS= 1601  // У данного пользователя нет ни одного мадата

};

typedef enum TaskState{
        converted,
        merged,
        splitted_first,
        splitted_other,
        printed,
        previewedPage,
        pdfMarkAdded
};

typedef enum WorkStep {
        netPluginInit,
        authPluginInit,
        gsPluginInit,
	tmplPluginInit,
        filledPrinterList,
        psToPdfConverted
};
typedef enum WorkMode{
    Accounting,
    PrintOverAccountPaper,
    PrintWithAccounting
};


}
#endif // TECH_GLOBAL_H
