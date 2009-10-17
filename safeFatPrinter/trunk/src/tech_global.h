#ifndef TECH_GLOBAL_H
#define TECH_GLOBAL_H


typedef enum{ 
   GET_PRINTER_LIST_CMD = 100,
   GET_MB_PRINTS_TODAY_CMD =200,
   DISCONNECT		= 5000	

}commands_t;

typedef enum{ 
   REGISTER_ANS	    = 1000,
   PRINTER_LIST_ANS = 1100,
   MB_LIST_ANS = 1200


}answer_t;

typedef enum {
	    NO_ERROR,
	    HOSTNAME_EMPTY_ERR,
	    INVALID_PORT_RANGE_ERR,
	    HOST_NOT_FOUND_ERR,
	    CONNECTION_REFUSED_ERR
}error_t;

#endif
