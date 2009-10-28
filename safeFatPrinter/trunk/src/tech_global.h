#ifndef TECH_GLOBAL_H
#define TECH_GLOBAL_H

#define splitter =":;:";

typedef enum{ 
   REGISTER_CMD		= 10,
   AUTHOR_USER          = 100,
   GET_MB_PRINTS_TODAY_CMD =200,

   RAW_SQL_CMD		= 202,
   GET_SEC_LEVEL	= 300, //запрос к демону на получение списка уровней секретности

   GET_PRINTER_LIST_CMD = 400,

   IS_MB_EXIST	=500,
   DISCONNECT		= 5000	

}commands_t;

typedef enum{ 
   REGISTER_ANS	    = 1010,
   PRINT_ALLOWED    = 1100, // печать разрешена
   PRINT_DENIED     = 1101, // печать запрещена
   PRINTER_NOT_FOUND   = 1102, //  принтер не найден


   PRINTER_LIST_ANS = 1400,
   MB_LIST_ANS = 1200,
   IS_MB_EXIST_AND_BRAK_ANS =1205,
   IS_MB_EXIST_AND_NOT_BRAK_ANS =1210,
   IS_MB_NOT_EXIST_ANS=1300
}answer_t;

typedef enum {
	    NO_ERROR,
	    HOSTNAME_EMPTY_ERR,
	    INVALID_PORT_RANGE_ERR,
	    HOST_NOT_FOUND_ERR,
	    CONNECTION_REFUSED_ERR
}error_t;

#endif
