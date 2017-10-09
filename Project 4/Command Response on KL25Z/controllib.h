
#include <stdint.h>
#include "MKL25Z4.h"
#include "core_cm0plus.h"

#ifndef SOURCES_CONTROLLIB_H_
#define SOURCES_CONTROLLIB_H_

typedef enum
{
	RED_ON = 0,
	RED_OFF,
	GREEN_ON,
	GREEN_OFF,
	BLUE_ON,
	BLUE_OFF,
	RED_INTENSITY,
	GREEN_INTENSITY,
	BLUE_INTENSITY,
	REMOTE_RESET,
	GET_RTC,
}Command_t;

typedef struct
{
	Command_t com;
	uint8_t len;
	uint8_t *data;
	uint16_t checksum;
}Message_t;

typedef struct
{
	uint8_t red_s, green_s, blue_s;
}Color_t;

typedef enum
{
	CL_FAIL = -1,
	CL_SUCCESS = 0,
}CL_Ret_t;

typedef enum
{
	CS_FAIL = -1,
	CS_SUCCESS = 0,
}CS_Ret_t;

CL_Ret_t software_reset(void *arg);

CL_Ret_t send_rtc(void *arg);

CS_Ret_t check_sum(void *arg);

#endif /* SOURCES_CONTROLLIB_H_ */
