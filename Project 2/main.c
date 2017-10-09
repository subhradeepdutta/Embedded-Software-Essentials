
#include "MKL25Z4.h"			//include register definition file
#include "uart.h"				//include uart declarations
#include "circbuf.h"			//include circular buffer files
#include "logger.h"				//include logger files
#include "data.h"				//include data manipulation files (itoa)

#define BASE_10 (10)			//set macro for use as the number 10

static int32_t i = 0;			//For delay function

static uint32_t count = 0;		//To count the number of bytes received

CircBuf_t * Tx_Buf, * Rx_Buf;

uint8_t received_char;
uint32_t prev_count;

void parse_uart_contents(CircBuf_t *circ)		//parse received byte
{
	uint32_t iter = 0;			//counts form 0 to 16
	uint8_t c;					//stores each character
	size_t alpha = 0, num = 0, punc = 0, misc = 0;	//count values for the alphabets
	LogItem_t *test;			//pointer to a log item for creating logs

	uint8_t info[] = "16 byte of data received";

	test = (LogItem_t *)malloc(sizeof(LogItem_t));
	create_log_item(test, INFO, sizeof(info), info);
	LOG_ITEM(test);
	//	log_item(Tx_Buf, test);
	destroy_log_item(test);

	test = (LogItem_t *)malloc(sizeof(LogItem_t));	//allocate memory
	create_log_item(test, DATA_ANALYSIS_STARTED, 0, 0);					//log analysis started
	LOG_ITEM(test);
	//	log_item(Tx_Buf, test);
	destroy_log_item(test);

	while(iter != 16)			//checks for what kind of ascii value there is
	{
		circbuf_remove(circ, &c);		//alphabets
		if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		{
			alpha++;
		}
		else if(c >= '0' && c <= '9')	//numeric
		{
			num++;
		}
		else if(c == 0x21 || c == 0x22 || c == 0x2c || c == 0x2e || c == 0x2d || c == 0x3a || c == 0x3b || c == 0x27 || c == 0x5b || c == 0x5d || c == 0x28 || c == 0x29)
		{
			punc++;
		}
		else							//miscellaneous
		{
			misc++;
		}
		iter++;
	}

	uint8_t *data_ptr;

	if(alpha > 0)
	{
		test = (LogItem_t *)malloc(sizeof(LogItem_t));
		data_ptr = (uint8_t *)malloc(11);
		data_ptr = my_itoa(data_ptr, alpha, BASE_10);
		create_log_item(test, DATA_ALPHA_COUNT, sizeof(data_ptr), data_ptr);	//log number of alphabets
		LOG_ITEM(test);
//		log_item(Tx_Buf, test);
		destroy_log_item(test);
	}

	if(num > 0)
	{
		test = (LogItem_t *)malloc(sizeof(LogItem_t));
		data_ptr = (uint8_t *)malloc(11);
		data_ptr = my_itoa(data_ptr, num, BASE_10);
		create_log_item(test, DATA_NUMERIC_COUNT, sizeof(data_ptr), data_ptr);	//log nuber of numerics
		LOG_ITEM(test);
//		log_item(Tx_Buf, test);
		destroy_log_item(test);
	}

	if(punc > 0)
	{
		test = (LogItem_t *)malloc(sizeof(LogItem_t));
		data_ptr = (uint8_t *)malloc(11);
		data_ptr = my_itoa(data_ptr, punc, BASE_10);
		create_log_item(test, DATA_PUNCTUATION_COUNT, sizeof(data_ptr), data_ptr);	//log nuber of punctuations
		LOG_ITEM(test);
//		log_item(Tx_Buf, test);
		destroy_log_item(test);
	}

	if(misc > 0)
	{
		test = (LogItem_t *)malloc(sizeof(LogItem_t));
		data_ptr = (uint8_t *)malloc(11);
		data_ptr = my_itoa(data_ptr, misc, BASE_10);
		create_log_item(test, DATA_MISC_COUNT, sizeof(data_ptr), data_ptr);		//log nuber of miscellaneous characters
		LOG_ITEM(test);
//		log_item(Tx_Buf, test);
		destroy_log_item(test);
	}

	test = (LogItem_t *)malloc(sizeof(LogItem_t));
	create_log_item(test, DATA_ANALYSIS_COMPLETED, 0, 0);					//log analysis completion
	LOG_ITEM(test);
//	log_item(Tx_Buf, test);
	destroy_log_item(test);
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
			count++;

			circbuf_add(Rx_Buf, c);					//add to the receive buffer
			if(count == 16)
			{
				Rx_Buf->send_status = 1;
				count = 0;
			}
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

int main(void)
{
	#ifdef FRDM_WITH_CIRC
	uart_configure();		//configure the UART
	#endif
	LogItem_t *testing;

	Tx_Buf = (CircBuf_t *)malloc(sizeof(CircBuf_t));	//allocate memory to transmit buffer
	int8_t res = circbuf_init(Tx_Buf, 100);

	Rx_Buf = (CircBuf_t *)malloc(sizeof(CircBuf_t));	//allocate memory to receive buffer
	res = circbuf_init(Rx_Buf, 100);

	testing = (LogItem_t *)malloc(sizeof(LogItem_t));
	create_log_item(testing, SYSTEM_INITIALIZED, 0, 0);	//log that system has beeen initailized
	LOG_ITEM(testing);
//	log_item(Tx_Buf, testing);
	destroy_log_item(testing);

	testing = (LogItem_t *)malloc(sizeof(LogItem_t));
	create_log_item(testing, GPIO_INITIALIZED, 0, 0);	//log that GPIO has beeen initailized
	LOG_ITEM(testing);
//	log_item(Tx_Buf, testing);
	destroy_log_item(testing);

	testing = (LogItem_t *)malloc(sizeof(LogItem_t));
	create_log_item(testing, LOGGER_INITIALIZED, 0, 0);	//log that logger has beeen initailized
	LOG_ITEM(testing);
//	log_item(Tx_Buf, testing);
	destroy_log_item(testing);

	while(1)
	{
		for (uint32_t a = 0;a < 640000; a++)
		{
			;
		}

		if(Rx_Buf->send_status == 1)
		{
			Rx_Buf->send_status = 0;
			parse_uart_contents(Rx_Buf);
		}



		if(Tx_Buf->num_items >= (Tx_Buf->size - 10))
		{
			uint8_t warn[] = "Buffer almost full";
			testing = (LogItem_t *)malloc(sizeof(LogItem_t));
			create_log_item(testing, WARNING, sizeof(warn), warn);
			LOG_ITEM(testing);
		//	log_item(Tx_Buf, testing);
			destroy_log_item(testing);
		}
		else if(circbuf_full(Tx_Buf) == BUFFER_FULL)
		{
			uint8_t warn[] = "Buffer full";
			testing = (LogItem_t *)malloc(sizeof(LogItem_t));
			create_log_item(testing, WARNING, sizeof(warn), warn);
			LOG_ITEM(testing);
		//	log_item(Tx_Buf, testing);
			destroy_log_item(testing);
		}
		else if(Rx_Buf->num_items >= (Rx_Buf->size - 10))
		{
			uint8_t warn[] = "Buffer almost full";
			testing = (LogItem_t *)malloc(sizeof(LogItem_t));
			create_log_item(testing, WARNING, sizeof(warn), warn);
			LOG_ITEM(testing);
		//	log_item(Tx_Buf, testing);
			destroy_log_item(testing);
		}
		else if(circbuf_full(Rx_Buf) == BUFFER_FULL)
		{
			uint8_t warn[] = "Buffer full";
			testing = (LogItem_t *)malloc(sizeof(LogItem_t));
			create_log_item(testing, WARNING, sizeof(warn), warn);
			LOG_ITEM(testing);
		//	log_item(Tx_Buf, testing);
			destroy_log_item(testing);
		}
	}
    /* Never leave main */
    return 0;
}
