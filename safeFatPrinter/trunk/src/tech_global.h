#ifndef TECH_GLOBAL_H
#define TECH_GLOBAL_H


typedef enum{ 
   GET_PRINTER_LIST_CMD = 100

}commands_t;

typedef enum {
	    NO_ERROR ,
	    HOSTNAME_EMPTY_ERR ,
	    INVALID_PORT_RANGE_ERR ,
	    HOST_NOT_FOUND_ERR ,
	    CONNECTION_REFUSED_ERR
}error_t;

#endif
