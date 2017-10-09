#ifndef SOURCES_MEM_H_
#define SOURCES_MEM_H_
#include <string.h>
#include <stdint.h>
#include "MKL25Z4.h"
#include <stdlib.h>

extern uint8_t overlap;
uint8_t *overlap_src, *overlap_dst;
size_t overlap_length;

typedef enum
{
	THIRTYTWO_BIT = 0,
	EIGHT_BIT,
	SIXTEEN_BIT,
	AUTO,
}DMA_Size_t;


/*********************************************************************
*Mem_Enum_t
*	description:Is the typedef enum that deals with all the possible
				cases of failure/success in functions that are applied
				to the memory functions that use the DMA
*	parameters:N/A

*	returns:N/A
*********************************************************************/

typedef enum
{
	DMA_NO_ERROR = 0,
	NULL_SRC,
	NULL_DST,
	DMA_NO_LENGTH,
}Mem_Enum_t;

/*List of function prototypes*/

/*********************************************************************
*memmove_dma
*	description:Moves specified number of bytes from a source address
				to a destination address using DMA. The function handles
				situations where the destination and source address
				ranges have overalapping regions.
*	parameters:
		uint8_t *src - Pointer to the start of the source address
		uint8_t *dst - Pointer to the start of the destination address
		size_t length - Number of bytes to be moved from source to
							destination
*	returns:
			Returns enum that is determined by the success or failure
			of the operation
*********************************************************************/

Mem_Enum_t memmove_dma(uint8_t *src, uint8_t *dst, size_t length, size_t byte_size);

/*********************************************************************
*memset_dma
*	description:Sets a specified number of bytes in memory to a
				specified value using DMA
*	parameters:
		uint8_t *src - Pointer to the start of the source address
		size_t length - Number of bytes to be written with the
						specified value
		uint8_t value - The value to be written to the specified memory
						locations
*	returns:
			Returns 0 if successful or 1 if failed
*********************************************************************/

Mem_Enum_t memset_dma(uint8_t *dst, size_t length, uint8_t value, size_t byte_size);

#endif /* SOURCES_MEM_H_ */
