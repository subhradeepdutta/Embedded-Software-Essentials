#include <stdio.h>
#include <stdint.h>
#include "feature.h"

int8_t *feature(int8_t *src, int32_t size_array, int8_t *search_terms, int32_t number_of_terms)
{
	int8_t *return_array;
	int32_t x = 0;
	for(int32_t i = 0; i<number_of_terms; i++)
	{
		for(int32_t j = 0; j<size_array; j++)
		{
			if(*(src + j) == *(search_terms + i))
			{
				*(return_array + x) = j;
				x++;
				break;
			}
		}
	}
	return return_array;
}
