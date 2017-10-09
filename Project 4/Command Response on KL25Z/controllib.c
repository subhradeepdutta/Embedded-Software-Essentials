
#include "controllib.h"
#include "logger.h"

CL_Ret_t software_reset(void *arg)
{
	NVIC_SystemReset();
	return CS_SUCCESS;
}

CL_Ret_t send_rtc(void *arg)
{
	uint32_t val = RTC_TSR;
	LOG_RAW_INT(val);
	LOG_RAW_STRING("\r\n");
	return CS_SUCCESS;
}

CS_Ret_t check_sum(void *arg)
{
	uint32_t sum = 0;
	sum += ((Message_t *)arg)->com + ((Message_t *)arg)->len;
	uint8_t count = 0;
	while(count < ((Message_t *)arg)->len - 4)
	{
		sum += *(((Message_t *)arg)->data + count);
		count++;
	}
	uint16_t ck = (uint16_t)(sum) % 65536;
	if(ck == ((Message_t *)arg)->checksum)
		return CS_SUCCESS;
	else
		return CS_FAIL;
}
