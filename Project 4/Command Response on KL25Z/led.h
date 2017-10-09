
#include "controllib.h"
#include <MKL25Z4.h>


#ifndef SOURCES_LED_H_
#define SOURCES_LED_H_

//typedef struct
//{
//	uint8_t state;
//	uint16_t intensity;
//}LED_t;
//
//LED_t red, green, blue;

extern Color_t * status;

void LED_init();

CL_Ret_t red_on(void *);

CL_Ret_t red_off(void *);

CL_Ret_t green_on(void *);

CL_Ret_t green_off(void *);

CL_Ret_t blue_on(void *);

CL_Ret_t blue_off(void *);

CL_Ret_t red_intensity(void *);

CL_Ret_t green_intensity(void *);

CL_Ret_t blue_intensity(void *);

#endif /* SOURCES_LED_H_ */
