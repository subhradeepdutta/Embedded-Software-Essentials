#include "MKL25Z4.h"
#include <stdio.h>
#include <stdlib.h>
#include "gpio.h"
#include "spi.h"
#include "nrf.h"
#include <stdint.h>
#include "dma.h"
#include "rtc.h"
#include "mem.h"
#include "memory.h"
#include "profiler.h"
#include "uart.h"				//include uart declarations
#include "circbuf.h"			//include circular buffer files
#include "logger.h"				//include logger files
#include "data.h"				//include data manipulation files (itoa)

#define TEST_VALUE1 (0x0f)
#define TEST_VALUE2 (0x00)
#define TEST_VALUE3 (0x7A)

//extern uint32_t OF_val;
//extern uint8_t overlap;
uint32_t sec;
LogItem_t *testing;
CircBuf_t * Tx_Buf, * Rx_Buf;

/*Variable to store the data returned from SPI reads and writes*/
static uint8_t return_data;
uint32_t start = 0, stop = 0;

void RTC_Seconds_IRQHandler()
{
	__disable_irq();
	testing = (LogItem_t *)malloc(sizeof(LogItem_t));
	create_log_item(testing, HEARTBEAT, 0, 0);	//log that system has been initialized
	LOG_ITEM(testing);
//	log_item(Tx_Buf, testing);
	destroy_log_item(testing);
	sec++;
	__enable_irq();
}

void DMA2_IRQHandler()
{
	__disable_irq();
	DMA_DSR_BCR2 |= DMA_DSR_BCR_DONE_MASK;
	if(overlap == 1)
	{
		overlap = 0;
		DMAMUX0_CHCFG2 &= ~(DMAMUX_CHCFG_ENBL_MASK);

		DMA_SAR2 = (uint32_t)overlap_src;
		DMA_DAR2 = (uint32_t)overlap_dst;
		DMA_DSR_BCR2 |= DMA_DSR_BCR_BCR(overlap_length);
		DMA_DCR2 |= DMA_DCR_EINT_MASK | DMA_DCR_SINC_MASK | DMA_DCR_DINC_MASK | DMA_DCR_SSIZE(EIGHT_BIT) | DMA_DCR_DSIZE(EIGHT_BIT);
		DMA_DCR2 |= DMA_DCR_START_MASK;

		//set the enable bit for the DMAMUX
		DMAMUX0_CHCFG2 |= DMAMUX_CHCFG_ENBL_MASK;
	}
	else
	{
		stop = calculate_time();
	}
	__enable_irq();

}

void TPM0_IRQHandler()
{
	__disable_irq();
	TPM0_SC |= TPM_SC_TOF_MASK;
	OF_val++;
	__enable_irq();
}

void UART0_IRQHandler()
{
	__disable_irq();
	uint8_t c;
	LogItem_t *data_received;
	if((UART0->C2 & UART0_C2_TIE_MASK) == UART0_C2_TIE_MASK)		//capture Tx interrupt
	{
		if(Tx_Buf->num_items > 0)
		{
			circbuf_remove(Tx_Buf, &c);					//transmit a byte from the circular buffer
			uart_send_byte(c);
		}
		else
		{
//			UART0->C2 &= ~UART0_C2_TCIE_MASK;
			UART0->C2 &= ~UART0_C2_TIE_MASK;			//if entire length of bytes is sent then clear transmit interrupt flag
			Tx_Buf->send_status = 0;
		}
	}
	else if((UART0->C2 & UART0_C2_RIE_MASK) == UART0_C2_RIE_MASK)	//capture Rx interrupt
	{
		if(Rx_Buf->num_items < Rx_Buf->size)
		{
			uart_receive_byte(&c);
//			count++;

			circbuf_add(Rx_Buf, c);					//add to the receive buffer
//			if(count == 16)
//			{
//				Rx_Buf->send_status = 1;
//				count = 0;
//			}
			data_received = (LogItem_t *)malloc(sizeof(LogItem_t));
			create_log_item(data_received, DATA_RECEIVED, 1, &c);		//log that data has been received
			log_item(Tx_Buf, data_received);
			destroy_log_item(data_received);


		}
		else
		{
//			UART0->C2 &= ~UART0_C2_TCIE_MASK;
			UART0->C2 &= ~UART0_C2_RIE_MASK;			//clear receive interrupt flag
//			Tx_Buf->send_status = 0;
		}
	}
	else
		return;
	__enable_irq();
}


/*Writes and reads to all the registers specified in the requirement document
 *  to verify the ability to write and read back from various registers on
 *  the Nordic device */
void nordic_demo()
{
	/*Intialize variables to test Nordic device functionality*/

	uint8_t tx_addr_value_write[5]={0xAA,0x55,0xAA,0x55,0xAA};
	uint8_t tx_addr_value_read[5]={0};
	int8_t loop = 0;

	/*Read the value of status register on the Nordic device*/
	return_data = nrf_read_status();

	/*******************************************/
	LOG_RAW_INT(return_data);
	/*******************************************/

	/*Write the value of config register on the Nordic device*/
	nrf_write_config(TEST_VALUE1);
	/*Read the value of config register on the Nordic device*/
	return_data = nrf_read_config();

	/*******************************************/
	LOG_RAW_INT(return_data);
	/*******************************************/
	/*Write the value of TX_ADDR and then read it back*/
	nrf_write_TX_ADDR(tx_addr_value_write);
	nrf_read_TX_ADDR(tx_addr_value_read);

	for(loop=0; loop<5; loop++)
	{
		LOG_RAW_INT(*(tx_addr_value_read+loop));
	}

	/*Write the value of rf_setup on the Nordic device*/
	nrf_write_rf_setup(TEST_VALUE2);
	/*Read the value of rf_setup on the Nordic device*/
	return_data = nrf_read_rf_setup();

	/*******************************************/
	LOG_RAW_INT(return_data);
	/*******************************************/

	/*Write the value of rf_ch on the Nordic device*/
	nrf_write_rf_ch(TEST_VALUE3);
	/*Read the value of rf_ch on the Nordic device*/
	return_data = nrf_read_rf_ch();

	/*******************************************/
	LogItem_t *test;
	test = (LogItem_t *)malloc(sizeof(LogItem_t));
	uint8_t info[] = " ";
	create_log_item(test, INFO, sizeof(info), info);
	LOG_ITEM(test);
	destroy_log_item(test);

	LOG_RAW_INT(return_data);
	/*******************************************/

	/*Read the value of fifo_status on the Nordic device*/
	return_data = nrf_read_fifo_status();

	/*******************************************/
	LOG_RAW_INT(return_data);
	/*******************************************/
}

int main(void)
{
	Tx_Buf = (CircBuf_t *)malloc(sizeof(CircBuf_t));	//allocate memory to transmit buffer
	int8_t res = circbuf_init(Tx_Buf, 100);

	Rx_Buf = (CircBuf_t *)malloc(sizeof(CircBuf_t));	//allocate memory to receive buffer
	res = circbuf_init(Rx_Buf, 100);


	dma_config();
	profiler_config();
	rtc_config();
	uart_configure();
//	uint32_t start1 = 0, stop1 = 0;
//	uint32_t start2 = 0, stop2 = 0;
	uint8_t a[5000], b[5000];
	start = calculate_time();
	//my_memmove(a, b, 5000);
	memset_dma(a,10,0xFF,THIRTYTWO_BIT);
	//stop = calculate_time();
	LOG_RAW_INT(start);

	LogItem_t *test;
	test = (LogItem_t *)malloc(sizeof(LogItem_t));
	uint8_t info[] = "Timer Stopped";
	create_log_item(test, INFO, sizeof(info), info);
	LOG_ITEM(test);
	destroy_log_item(test);

	LOG_RAW_INT(stop);

//	GPIO_nrf_init();
//	SPI_init();
//	nordic_demo();
	return 0;
}

