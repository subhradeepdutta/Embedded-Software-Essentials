#include <stdint.h>

#ifndef SOURCES_PROFILER_H_
#define SOURCES_PROFILER_H_

#define MCGFLLCLK		1		//macro for the MCGFLLCLK
#define OSCERCLK		2		//macro for the OSCERCLK
#define MCGIRCLK 		3		//macro for the MCGIRCLK
#define LOCAL_CLOCK		1
#define PRESCALE		0

extern uint32_t OF_val;
void profiler_config();

uint32_t calculate_time();

#endif /* SOURCES_PROFILER_H_ */
