#include <stdint.h>

#include "data.h"
#include "uart.h"
#include "circbuf.h"
#include "logger.h"

#define BASE_10	(10)
//#define BIN_LOG

LG_Ret_t log_data(CircBuf_t *circ, uint8_t *data_ptr, size_t length)
{
	if(circ == NULL || data_ptr == NULL)
	{
		return	NULL_POINTER_PASSED;
	}

	while(length)
	{
		circbuf_add(circ, *data_ptr);
//		uart_send_byte(c);
		length--;
		data_ptr++;
	}
	circ->send_status = 1;
	if(circ->send_status == 1)
	{
		//Placeholder for software call for SWI
		UART0->C2 |= UART0_C2_TIE_MASK;
//		UART0_IRQHandler();
	}

	return SUCCESS;
}

LG_Ret_t log_string(CircBuf_t *circ, uint8_t *data_ptr)
{
	if(circ == NULL || data_ptr == NULL)
	{
		return	NULL_POINTER_PASSED;
	}

	while(*data_ptr != '\0')
	{
		circbuf_add(circ, *data_ptr);
		data_ptr++;
	}
	circ->send_status = 1;
	if(circ->send_status == 1)
	{
		UART0->C2 |= UART0_C2_TIE_MASK;
	}

	return SUCCESS;
}

LG_Ret_t log_integer(CircBuf_t *circ, int32_t data)
{
	if(circ == NULL)
	{
		return	NULL_POINTER_PASSED;
	}

	uint8_t *data_ptr = (uint8_t *)malloc(11);
	data_ptr = my_itoa(data_ptr, data, BASE_10);
	while(*data_ptr != '\0')
	{
		circbuf_add(circ, *data_ptr);
		data_ptr++;
	}
	circ->send_status = 1;
	{
		UART0->C2 |= UART0_C2_TIE_MASK;
	}

	return SUCCESS;
}


LG_Ret_t log_data_to_UART(uint8_t *data_ptr, size_t length)
{
	if(data_ptr == NULL)
	{
		return	NULL_POINTER_PASSED;
	}

	uart_send_byte_n(data_ptr, length);

	return SUCCESS;
}

LG_Ret_t log_string_to_UART(uint8_t *data_ptr)
{
	if(data_ptr == NULL)
	{
		return	NULL_POINTER_PASSED;
	}

	while(*data_ptr != '\0')
	{
		uart_send_byte(*data_ptr);
		data_ptr++;
	}

	return SUCCESS;
}

LG_Ret_t log_integer_to_UART(int32_t data)
{
	int8_t *data_ptr = my_itoa(data_ptr, data, BASE_10);
	while(*data_ptr != '\0')
	{
		uart_send_byte(*data_ptr);
		data_ptr++;
	}

	return SUCCESS;
}

LG_Ret_t create_log_item(LogItem_t *Item , LG_Enum_t ID, size_t length, uint8_t *payload)
{
	if(Item == NULL)
	{
		return	NULL_POINTER_PASSED;
	}

	Item->logID = ID;
	Item->PLLength = length;

	Item->PL = (uint8_t *)malloc(length);

	uint32_t i = length;

	while(i != 0)
	{
		*(Item->PL + length - i) = *(payload + length - i);
		i--;
	}

	if(Item->PL == NULL)
	{
		return ALLOCATION_FAILED;
	}
	else
	{
		return SUCCESS;
	}
}

#ifndef BIN_LOG
LG_Ret_t log_item(CircBuf_t *circ, LogItem_t *Item)
{
	if(circ == NULL || Item == NULL)
	{
		return	NULL_POINTER_PASSED;
	}

	uint8_t ID_indicator[] = "LOG ID: ", PL_indicator[] = "Payload: ";
	LOG_RAW_DATA(ID_indicator, 8);
//	log_data(circ, ID_indicator, 8);
	LOG_RAW_INT((int32_t)Item->logID);
//	log_integer(circ, (int32_t)Item->logID);

	if(Item->PLLength != 0)
	{
		LOG_RAW_DATA(PL_indicator, 9);
//		log_data(circ, PL_indicator, 9);
		LOG_RAW_DATA(Item->PL, Item->PLLength);
//		log_data(circ, (Item->PL), Item->PLLength);
	}

	return SUCCESS;
}
#endif

#ifdef BIN_LOG

LG_Ret_t log_item(CircBuf_t *circ, LogItem_t *Item)
{
	if(circ == NULL || Item == NULL)
	{
		return	NULL_POINTER_PASSED;
	}

	size_t len = sizeof(Item->logID) + sizeof(Item->PLLength) + Item->PLLength;

	uint8_t * pt = (uint8_t *)(Item);

	while(len != 0)
	{
		circbuf_add(circ, *pt);
		len--;
		pt++;
	}

	UART0->C2 |= UART0_C2_TIE_MASK;

	return SUCCESS;
}

#endif
LG_Ret_t destroy_log_item(LogItem_t *Item)
{
	if(Item == NULL || Item->PL == NULL)
	{
		return	NULL_POINTER_PASSED;
	}

	free(Item->PL);
	free(Item);

	return SUCCESS;
}

LG_Ret_t log_flush(CircBuf_t *circ)
{
	if(circ == NULL)
	{
		return NULL_POINTER_PASSED;
	}

	__disable_irq();
	while(circ->num_items != 0);
	__enable_irq();

	return SUCCESS;
}
