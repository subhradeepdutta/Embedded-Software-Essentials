
#include <stdint.h>
#include "circbuf.h"

#ifndef SOURCES_LOGGER_H_
#define SOURCES_LOGGER_H_

extern CircBuf_t * Tx_Buf;

#define LOGGING_ON
//#define LOGGING_OFF
#define FRDM_WITH_CIRC

#ifdef LOGGING_ON
	#ifdef FRDM1
	#define LOG_RAW_DATA(x,y)	log_data_to_uart((uint8_t *)x,(size_t)y)
	#define LOG_RAW_STRING(x)	log_string_to_uart((uint8_t *)x)
	#define LOG_RAW_INT(x)		log_integer_to_uart((int32_t)x)
	#define LOG_FLUSH()			log_flush()
	#endif

	#ifdef FRDM_WITH_CIRC
	#define	LOG_RAW_DATA(x,y)	log_data(Tx_Buf,(uint8_t *)x,(size_t)y)
	#define LOG_RAW_STRING(x)	log_string(Tx_Buf,(uint8_t *)x)
	#define LOG_RAW_INT(x)		log_integer(Tx_Buf,(int32_t)x)
	#define LOG_ITEM(x)			log_item(Tx_Buf,(LogItem_t *)x)
	#endif

	#ifdef LINUX
	#define LOG_RAW_DATA(x,y)	printmemory((uint8_t *)x,(size_t)y)
	#define LOG_RAW_STRING(x)	printf("%s", x)
	#define LOG_RAW_INT(x)		printf("%d", x)
	#define	LOG_ITEM(x)			log_item(Tx_Buf,(LogItem_t *)x)
	#endif

#endif

#ifdef LOGGING_OFF
	#ifdef FRDM
	#define LOG_RAW_DATA(x,y)
	#define LOG_RAW_STRING(x)
	#define LOG_RAW_INT(x)
	#define LOG_ITEM(x)
	#define LOG_FLUSH()
	#endif

	#ifdef FRDM_WITH_CIRC
	#define	LOG_RAW_DATA(x,y)
	#define LOG_RAW_STRING(x)
	#define LOG_RAW_INT(x)
	#define LOG_ITEM(x)
	#endif

	#ifdef LINUX
	#define LOG_RAW_DATA(x,y)
	#define LOG_RAW_STRING(x)
	#define LOG_RAW_INT(x)
	#define	LOG_ITEM(x)
#endif

#endif

/*********************************************************************
*LG_Ret_t
*	description:Is the typedef enum that deals with all the possible
				return values for the functions in logger.c.
*	parameters:N/A

*	returns:N/A
*********************************************************************/

typedef enum LG_ret
{
	FAIL = -1,
	SUCCESS,
	NULL_POINTER_PASSED,
	ALLOCATION_FAILED,
}LG_Ret_t;

/*********************************************************************
*LG_Enum_t
*	description:Is the typedef enum that deals with all the possible
				Log ID values that can exist for the binary logger
				enumerate the many types of responses the logger can
				feed across the UART to the terminal so that the user
				is able to track the current working condition of the
				embedded system.
*	parameters:N/A

*	returns:N/A
*********************************************************************/

typedef enum LGe
{
	LOGGER_INITIALIZED = (uint32_t)0,
	GPIO_INITIALIZED,
	SYSTEM_INITIALIZED,
	SYSTEM_HALTED,
	INFO,
	WARNING,
	ERROR,
	DATA_RECEIVED,
	DATA_ANALYSIS_STARTED,
	DATA_ALPHA_COUNT,
	DATA_NUMERIC_COUNT,
	DATA_PUNCTUATION_COUNT,
	DATA_MISC_COUNT,
	DATA_ANALYSIS_COMPLETED,
	HEARTBEAT,
}LG_Enum_t;

/*********************************************************************
*LogItem_t
*	description:Is the typedef struct datatype that is related to each
				Log item that is created. The elements of this
				data structure are:
					1. LG_Enum_t log ID that stores the value of the
					   log ID for the current log object
					2. The length of the log payload
					3. Pointer to a uint8_t string that is allocated
					   memory as required and stores the payload

*	parameters:N/A

*	returns:N/A
*********************************************************************/

typedef struct LogItem
{
	LG_Enum_t logID;
	uint32_t T;
	size_t PLLength;
	uint8_t *PL;
} __attribute__((packed))LogItem_t;

/*********************************************************************
*log_data
*	description:Logs data to the transmit circular buffer so that when
*				an interrupt is called to transmit the data it read it
*				sends the data across to UART channel to the terminal
*	parameters:
*		*circ:This is a pointer to the transmit buffer that is used to
*		send data across the UART
*		*data_ptr:This pointer points to the data that is needed to be
*		logged into the circular buffer so that it can be sent
*		length:The length of the data that is to be logged

*	returns: LG_Ret_t type enumeration
*********************************************************************/

LG_Ret_t log_data(CircBuf_t *circ, uint8_t *data_ptr, size_t length);

/*********************************************************************
*log_string
*	description:Logs a string to the transmit circular buffer so that
*			when an interrupt is called to transmit the data it read
*			it sends the data across to UART channel to the terminal
*	parameters:
*		*circ:This is a pointer to the transmit buffer that is used to
*		send data across the UART
*		*data_ptr:This pointer points to the data that is needed to be
*		logged into the circular buffer so that it can be sent

*	returns: LG_Ret_t type enumeration
*********************************************************************/


LG_Ret_t log_string(CircBuf_t *circ, uint8_t *data_ptr);

/*********************************************************************
*log_integer
*	description:Logs an integer to the transmit circular buffer so
*				that whenan interrupt is called to transmit the data
*				it read it sends the data across to UART channel to
*				the terminal
*	parameters:
*		*circ:This is a pointer to the transmit buffer that is used to
*		send data across the UART
*		data:This pointer points to the data that is needed to be
*		logged into the circular buffer so that it can be sent

*	returns: LG_Ret_t type enumeration
*********************************************************************/

LG_Ret_t log_integer(CircBuf_t *circ, int32_t data);

/*********************************************************************
*log_flush
*	description:Blocks until the transmit buffer is completely empty
*	parameters:
*		*circ:This is a pointer to the transmit buffer that is used to
*		send data across the UART
*
*	returns: LG_Ret_t type enumeration
*********************************************************************/

LG_Ret_t log_flush();

/*********************************************************************
*log_data_to_UART
*	description:Logs data directly to the UART
*	parameters:
*		*data_ptr:This pointer points to the data that is needed to be
*		logged into the circular buffer so that it can be sent
*		length:The length of the data that is to be logged

*	returns: LG_Ret_t type enumeration
*********************************************************************/

LG_Ret_t log_data_to_UART(uint8_t *data_ptr, size_t length);

/*********************************************************************
*log_string_to_UART
*	description:Logs a string directly to the UART
*	parameters:
*		*data_ptr:This pointer points to the data that is needed to be
*		logged into the circular buffer so that it can be sent

*	returns: LG_Ret_t type enumeration
*********************************************************************/

LG_Ret_t log_string_to_UART(uint8_t *data_ptr);

/*********************************************************************
*log_integer_to_UART
*	description:Logs an integer directly to the UART
*	parameters:
*		*circ:This is a pointer to the transmit buffer that is used to
*		send data across the UART
*		data:This pointer points to the data that is needed to be
*		logged into the circular buffer so that it can be sent

*	returns: LG_Ret_t type enumeration
*********************************************************************/

LG_Ret_t log_integer_to_UART(int32_t data);

/*********************************************************************
*create_log _item
*	description:This function creates a log item
*	parameters:The parameters passed into this function are:
*		*Item:Is a pointer to to a data strcuture of LogItem_t type
*		ID:Is a macro that points to what kind of data is being sent
*		*payload:Is a string that the information payload will consist
*		of.

*	returns:		[PLACEHOLDER]
*********************************************************************/

LG_Ret_t create_log_item(LogItem_t *Item , LG_Enum_t ID, size_t length, uint8_t *payload);

/*********************************************************************
*log_item
*	description:This function logs the data structure to the transmit
*				buffer so that it can be logged across the UART to the
*				terminal software being used.
*	parameters:The function takes in the following parameters:
*		*Item:Is a pointer to to a data strcuture of LogItem_t type
*		*circ:Is a pointer to the transmission buffer

*	returns:		[PLACEHOLDER]
*********************************************************************/

LG_Ret_t log_item(CircBuf_t *circ, LogItem_t *Item);

/*********************************************************************
*destroy_log_item
*	description:This function frees the log item data structure so
*				that it maty be reused
*	parameters:The function takes in the following parameters:
*		*Item:Is a pointer to to a data strcuture of LogItem_t type
*
*	returns:		[PLACEHOLDER]
*********************************************************************/

LG_Ret_t destroy_log_item(LogItem_t *Item);

#endif
