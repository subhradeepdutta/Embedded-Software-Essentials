#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "circbuf.h"

CB_Enum_t circbuf_init(CircBuf_t *circ, size_t size_of_buffer)
{
	if(circ == NULL)
	{
		return NULL_POINTER;
	}

	circ->size = size_of_buffer;
	circ->buffer = (uint8_t *)malloc(size_of_buffer);
	circ->head = circ->buffer;
	circ->tail = circ->buffer;
	circ->send_status = 0;
	circ->num_items = 0;

	if(circ->buffer == NULL)
	{
		return BUFFER_ALLOCATION_FAILED;
	}
	else
	{
		return NO_ERROR;	
	}
}

CB_Enum_t circbuf_add(CircBuf_t *circ, uint8_t data)
{
	if(circ == NULL)
	{
		return NULL_POINTER;
	}
	else if(circ->num_items == circ->size)
	{
		return BUFFER_FULL;
	}

	if(circ->num_items == 0)
	{
		*circ->head = data;
		circ->num_items++;
		return NO_ERROR;
	}
	else
	{
		if((circ->head - circ->buffer) == circ->size - 1)
		{
			circ->head = circ->buffer;
			*(circ->head) = data;
			circ->num_items++;
		}
		else
		{
			circ->head++;
			*(circ->head) = data;
			circ->num_items++;
		}

		return NO_ERROR;
	}
}

CB_Enum_t circbuf_remove(CircBuf_t *circ, uint8_t *data_ptr)
{
	if(circ == NULL)
	{
		return NULL_POINTER;
	}
	else if(circ->num_items == 0)
	{
		return BUFFER_EMPTY;
	}

	if(circ->num_items == 1)
	{
		*data_ptr = *(circ->tail);
		circ->num_items--;
		return NO_ERROR;
	}
	else
	{
		if((circ->tail - circ->buffer) == circ->size - 1)
		{
			*data_ptr = *(circ->tail);
			circ->tail = circ->buffer;
			circ->num_items--;
		}
		else
		{
			*data_ptr = *(circ->tail);
			circ->tail++;
			circ->num_items--;
		}
		return NO_ERROR;
	}	
}

static inline CB_Enum_t circbuf_full(CircBuf_t *circ)
{
	if(circ == NULL)
	{
		return NULL_POINTER;
	}

	if(circ->num_items == circ->size)
	{
		return BUFFER_FULL;
	}
	else
	{
		return NO_ERROR;
	}
}

static inline CB_Enum_t circbuf_empty(CircBuf_t *circ)
{
	if(circ == NULL)
	{
		return NULL_POINTER;
	}

	if(circ->num_items == 0)
	{
		return BUFFER_EMPTY;
	}
	else
	{
		return NO_ERROR;
	}
}

CB_Enum_t circbuf_peek(CircBuf_t *circ, size_t index, uint8_t *data_element)
{
	uint8_t *ptr = NULL;
	ptr=circ->tail;
	if(circ == NULL)
	{
		return NULL_POINTER;
	}
	else if(index > (circ->num_items)-1)
	{
		return NO_LENGTH;
	}
	while(index >= 0)
	{
		if(index == 0)
		{
			*data_element = *(ptr);
			ptr=NULL;
			return NO_ERROR;
		}
		if(ptr - circ->buffer == (circ->size - 1))
		{
			ptr = circ->buffer;
			index--;
			continue;
		}


		ptr++;
		index--;
	}

}

CB_Enum_t circbuf_destroy(CircBuf_t *circ)
{
	if(circ == NULL)
	{
		return NULL_POINTER;
	}

	free(circ->buffer);
	free(circ);

	return NO_ERROR;
}
