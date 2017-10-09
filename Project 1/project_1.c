#include <stdio.h>
#include <stdint.h>
#include "project_1.h"
#include "data.h"
#include "memory.h"
#include "feature.h"

#define MAXSIZE 32
#define MAX_STRING_SIZE 65
#define COPY_MAX_SIZE 3
#define BASE 10

#ifdef KL25Z_PLATFORM
#define printf(...)
#endif

uint8_t set1[MAXSIZE];
uint8_t set2[MAXSIZE];

void project_1_report()
{
	uint8_t i=0;
	uint8_t hex_value=0x00;
	int8_t capture_array[2];
	int8_t search_array[2] = {6,7};
	for (i = 0; i < MAXSIZE/2; i++)
	{
		*(set1+i)=hex_value++;
	}

	hex_value=0x00;
	for (i = 0; i < MAXSIZE/2; i++)
	{
		*(set1+16+i)=hex_value++;
	}

	uint8_t dec_value=1;
	for (i = 0; i < MAXSIZE; i++)
	{
		*(set2+i)=dec_value++;
	}

	test_data1();
	test_data2();
	test_memory();

}

void test_data1()
{
	uint8_t num=0;
	print_memory(set1,MAXSIZE);
	num=big_to_little((uint32_t*)set1,MAXSIZE);
	if(num==0)
		print_memory(set1,MAXSIZE);
	else
		printf("Conversion Failed\n");
	num=little_to_big((uint32_t*)set1,MAXSIZE);
	if(num==0)
		print_memory(set1,MAXSIZE);
	else
		printf("Conversion Failed\n");
}

void test_data2()
{
	int8_t string[MAX_STRING_SIZE] = {0};
	int8_t midway[COPY_MAX_SIZE] = {0};
	int8_t final[MAXSIZE] = {0};
	int8_t j=0;
	uint8_t i = 0;
	// uint32_t base=10;
	if(final == 0);
	printf("\n\n\n");
	printf("\nThe base for integer to ASCII conversion is: %d\n", BASE);
	
	#ifdef TYPE_1

	for(i=0;i<MAXSIZE;i+=4)
	{
		my_itoa(string,*(int32_t *)(set2+i),BASE);
		print_memory(string, 8);
		result = my_atoi(string);
		print_memory(result, 1);
	}

	#else

	for(i=0;i<MAXSIZE;i++)
	{
		my_itoa(string+(2*i),(int32_t)(*(set2+i)),BASE);
	}

	*(string + 1) = '\0';

	print_memory((uint8_t *)string, MAX_STRING_SIZE);

	for(i=0;i<MAX_STRING_SIZE;i+=2)
	{
		for(int8_t n=0;n<COPY_MAX_SIZE-1;n++)
		{
			*(midway+n) = *(string+i+n);
		}
		final[j] = my_atoi(midway);
		j++;
	}
	print_memory((uint8_t *)final, MAXSIZE);
	printf("\n\n\n");
	#endif
}

void test_memory()
{
	int8_t status=0;
	print_memory(set2,32);
	status=my_reverse(set2, 12);
	if(status==0)
		print_memory(set2,32);
	else
		printf("Conversion Failed");
	status=my_memset((set2 + 16), 4, 0xEE);
	if(status==0)
		print_memory(set2,32);
	else
		printf("Conversion Failed");
	status=my_memmove((set2 + 25), (set2 + 19), 6);
	if(status==0)
		print_memory(set2,32);
	else
		printf("Conversion Failed");
	status=my_memzero((set2 + 11), 5);
	if(status==0)
		print_memory(set2,32);
	else
		printf("Conversion Failed");
	status=my_memmove((set2), (set2 + 8), 8);
	if(status==0)
		print_memory(set2,32);
	else
		printf("Conversion Failed");
	printf("\n");
}

