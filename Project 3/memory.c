#include <stdio.h>
#include <stdint.h>
#include "memory.h"

#ifdef KL25Z_PLATFORM
#define printf(...)
#endif

int8_t my_memmove(uint8_t *src, uint8_t *dst, uint32_t length)
{
	int32_t i=0;
	/*Checking for special conditions*/
	if(src==NULL||dst==NULL||length<=0)
	{
		return 1;
	}
	else
	{
		/*Case 1 when source address is higher that destination address*/
		if(dst<src)
		{
			for(i=0;i<length;i++)
			{
				/*Copy data from source to destination*/
				*(dst+i)=*(src+i);
				/*Set source data to zero*/
				*(src+i)=0;				
			}
			return 0;
		}
		/*Case 2 when destination address is higher than source address*/
		else if(dst>src)
		{
			for(i=length-1;i>=0;i--)
			{
				/*Copy data from source to destination*/
				*(dst+i)=*(src+i);
				/*Set source data to zero*/
				*(src+i)=0;
			}
			return 0;
		}
		/*If source and destination address is same then do nothing*/
		else
			return 0;
	}
}

int8_t my_memset(uint8_t *src, uint32_t length, uint8_t value)
{
	/*Checking for special conditions*/
	if(src==NULL||length<=0)
		return 1;
	int32_t i=0;
	for(i=0;i<length;i++)
	{
		*(src+i)=value;
	}
	return 0;
}

int8_t my_memzero(uint8_t *src, uint32_t length)
{
	/*Checking for special conditions*/
	if(src==NULL||length<=0)
		return 1;
	int32_t i=0;
	for(i=0;i<length;i++)
	{
		*(src+i)=0;
	}
	return 0;
}

int8_t my_reverse(uint8_t *src, uint32_t length)
{
	/*Checking for special conditions*/
	if(src==NULL||length<=0)
		return 1;
	/*If length is 1 then nothing to reverse*/
	if(length==1)
		return 0;
	uint8_t temp;//temporary variable to swap bytes
	uint8_t *ptr=src;
	ptr=ptr+(length-1);//Make ptr point to the end of the array

	int8_t i=0;
	for(i=0;i<length/2; i++)
	{
		temp= *(src+i);
		*(src+i)=*ptr;
		*ptr=temp;
		ptr--;
	}
	if(src!=NULL)
		return 0;
	else
		return 1;
}