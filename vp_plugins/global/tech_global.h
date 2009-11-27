#ifndef TECH_GLOBAL_H
#define TECH_GLOBAL_H

// timeouts are in ms
static const int i_timeout_connect	= 5000;
static const int i_timeout_read		= 5000;
static const int i_timeout_write	= 5000;

typedef enum{
    CONN_STEP,
    CONVERT_STEP,
    GET_FROM_FTP_STEP
}steps_t;

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

}commands_t;

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

}answer_t;

typedef enum{
   DOC_PRINTED =-1
}doc_state_t;

typedef enum {
            ERROR,
            HOSTNAME_EMPTY_ERR,
            INVALID_PORT_RANGE_ERR,
            HOST_NOT_FOUND_ERR,
            CONNECTION_REFUSED_ERR,
            FILE_NOT_FOUND
}error_t;

#endif // TECH_GLOBAL_H
