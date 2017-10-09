
#include "fsl_device_registers.h"
#include "led.h"
#include "uart.h"
#include "circbuf.h"
#include "controllib.h"
#include "led.h"
#include "rtc.h"
#include <stdint.h>

CircBuf_t *Rx_Buf, *Tx_Buf;
uint8_t count, reps, counter, data_rep;
Message_t *rcv;
uint8_t ch;
Color_t *status;

static void* (*AppInterface[11])(void *) = {&red_on, &red_off, &green_on, &green_off, &blue_on, &blue_off, &red_intensity, &green_intensity, &blue_intensity, &software_reset, &send_rtc};

void UART0_IRQHandler()
{
	__disable_irq();
	uint8_t c;
	if((UART0->C2 & UART0_C2_TIE_MASK) == UART0_C2_TIE_MASK)		//capture Tx interrupt
	{
		if(Tx_Buf->num_items > 0)
		{
			circbuf_remove(Tx_Buf, &c);					//transmit a byte from the circular buffer
			uart_send_byte(c);
		}
		else
		{
			UART0->C2 &= ~UART0_C2_TIE_MASK;			//if entire length of bytes is sent then clear transmit interrupt flag
			Tx_Buf->send_status = 0;
		}
	}
	else if((UART0->C2 & UART0_C2_RIE_MASK) == UART0_C2_RIE_MASK)	//capture Rx interrupt
	{
		if(Rx_Buf->num_items < Rx_Buf->size)
		{
			uart_receive_byte(&c);
			count++;
			if(count == 2)
			{
				data_rep = c;
			}
			if(count == data_rep)
			{
				count = 0;
				Rx_Buf->send_status = 1;
			}
			circbuf_add(Rx_Buf, c);					//add to the receive buffer
		}
		else
		{
			UART0->C2 &= ~UART0_C2_RIE_MASK;			//clear receive interrupt flag
		}
	}
	else
		return;
	__enable_irq();
}

void RTC_Seconds_IRQHandler()
{
	int i = 0;
	i++;
}

int main(void)
{

    /* Write your code here */

	uart_configure();
	rtc_config();

	status = (Color_t *)malloc(sizeof(Color_t));
	LED_init();

	Tx_Buf = (CircBuf_t *)malloc(sizeof(CircBuf_t));	//allocate memory to transmit buffer
	circbuf_init(Tx_Buf, 100);

	Rx_Buf = (CircBuf_t *)malloc(sizeof(CircBuf_t));	//allocate memory to receive buffer
	circbuf_init(Rx_Buf, 100);

	rcv = (Message_t *)malloc(sizeof(Message_t));

//	software_reset(NULL);
	while(1)
	{
		if(Rx_Buf->send_status == 1)
		{
			circbuf_remove(Rx_Buf, &ch);
			rcv->com = ch;
			circbuf_remove(Rx_Buf, &ch);
			rcv->len = ch;
			reps++;
			rcv->data = (uint8_t *)malloc(rcv->len - 4);
			counter = 0;
			while(counter < rcv->len - 4)
			{
				circbuf_remove(Rx_Buf, &ch);
				*(rcv->data + counter) = ch;
				counter++;
			}
			uint8_t ch1, ch2;
			circbuf_remove(Rx_Buf, &ch1);
			*((uint8_t*)(&rcv->checksum) + 1) = ch1;
			circbuf_remove(Rx_Buf, &ch2);
			*((uint8_t*)(&rcv->checksum)) = ch2;
			Rx_Buf->send_status = 0;
			if(check_sum(rcv) == CS_SUCCESS)
				AppInterface[rcv->com](rcv);
			free(rcv->data);
		}
	}
    /* Never leave main */
    return 0;
}

