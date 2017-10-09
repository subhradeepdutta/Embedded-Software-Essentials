#include <stdio.h>
#include <stdint.h>
#include "MKL25Z4.h"
#include "spi.h"
#define SPI0_C1_CONFIG (0x56)


/**********************************************************************/
void SPI_init()
{

	/*SPI0 Control Register 1
	 * Enable SPI system
	 * Configured as master SPI device
	 * Selected active high SPI clock
	 * CPHA = 0
	 * CPOL = 0
	 * MSB first*/
	SPI0_C1 = SPI_C1_SPE_MASK | SPI_C1_MSTR_MASK;

	/*Baud Rate = 20.97 (Bus Clock)/16*/
	SPI0_BR &= ~( SPI_BR_SPR_MASK | SPI_BR_SPPR_MASK);
}


/**********************************************************************/
uint8_t SPI_write_byte(uint8_t data_value)
{
	uint8_t return_data;
	/*Wait for transmitter to be free*/
	while (!(SPI0_S & SPI_S_SPTEF_MASK));
	/*Write data to SPI data register*/
	SPI0_D = data_value;
	SPI_flush();
	/*Wait for data to be received from the Nordic device*/
	return_data= SPI0_D;
	/*Read the returned data*/
	return return_data;
}


/**********************************************************************/

void SPI_read_byte(uint8_t *data)
{
	/*Wait for the transmitter to be available*/
	while (!(SPI0_S & SPI_S_SPTEF_MASK));
	/*Write NOP to the data register*/
	SPI0_D = NOP;
	/*Wait for data to be received from the Nordic device*/
	SPI_flush();
	/*Read the data register*/
	*data= SPI0_D;

}

/**********************************************************************/

void SPI_send_packet(uint8_t *data_packet, size_t length)
{
	while(length)
	{
		/*Send the packet one byte at a time*/
		SPI_write_byte(*data_packet);
		length--;
		data_packet++;
	}
}


/**********************************************************************/
static inline void SPI_flush()
{
	while (!(SPI0_S & SPI_S_SPRF_MASK));
}
