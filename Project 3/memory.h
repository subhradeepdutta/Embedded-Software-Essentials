#ifndef MEMORY_H
#define MEMORY_H
#include <stdint.h>

/*List of function prototypes*/

/*********************************************************************
*my_memmove
*	description:Moves specified number of bytes from a source address
				to a destination address. The function handles 
				situations where the destination and source address
				ranges have overalapping regions.
*	parameters:
		uint8_t *src - Pointer to the start of the source address
		uint8_t *dst - Pointer to the start of the destination address
		uint32_t length - Number of bytes to be moved from source to
							destination
*	returns:
			Returns 0 if successful or 1 if failed
*********************************************************************/
int8_t my_memmove(uint8_t *src, uint8_t *dst, uint32_t length);


/*********************************************************************
*my_memset
*	description:Sets a specified number of bytes in memory to a 
				specified value
*	parameters:
		uint8_t *src - Pointer to the start of the source address
		uint32_t length - Number of bytes to be written with the 
						specified value
		uint8_t value - The value to be written to the specified memory
						locations
*	returns:
			Returns 0 if successful or 1 if failed
*********************************************************************/
int8_t my_memset(uint8_t *src, uint32_t length, uint8_t value);


/*********************************************************************
*my_memzero
*	description:Sets a specified number of bytes in memory to zero

*	parameters:
		uint8_t *src - Pointer to the start of the source address
		uint32_t length - Number of bytes to be overwritten with zero

*	returns:
			Returns 0 if successful or 1 if failed
*********************************************************************/
int8_t my_memzero(uint8_t *src, uint32_t length);

/*********************************************************************
*my_reverse
*	description:Reverses a specified length of bytes

*	parameters:
		uint8_t *src - Pointer to the start of the source address
		uint32_t length - Number of bytes upto which reversal is to be
							performed

*	returns:
			Returns 0 if successful or 1 if failed
*********************************************************************/
int8_t my_reverse(uint8_t *src, uint32_t length);


#endif