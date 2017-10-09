#include <stdio.h>
#include <stdint.h>
#include "MKL25Z4.h"
#include "spi.h"
#include "gpio.h"
#include "nrf.h"

uint8_t nrf_read_register(uint8_t register_address)
{

	CHIP_SELECT_LOW;
	uint8_t register_value;
	/*Send command to read register along with register address*/
	register_value = SPI_write_byte(READ_COMMAND | register_address);
	/*Send NOP to push to value from the Nordic device to the SPI data register*/
	register_value = SPI_write_byte(NOP);
	CHIP_SELECT_HIGH;
	return register_value;
}

void nrf_write_register(uint8_t register_address, uint8_t value)
{
	CHIP_SELECT_LOW;
	/*Bit 6 in W_REGISTER needs to be high to indicate it is a write operation
	 * Address to be ORed with the last 5 bits*/
	SPI_write_byte(WRITE_COMMAND | register_address);
	/*Write back the desired register value*/
	SPI_write_byte(value);
	CHIP_SELECT_HIGH;

}

uint8_t nrf_read_status()
{
	CHIP_SELECT_LOW;
	uint8_t received_data;
	received_data = SPI_write_byte(NOP);
	CHIP_SELECT_HIGH;
	return received_data;
}


void nrf_write_config(uint8_t configuration_value)
{

	/*Send the address of the configuration register and the value to be written*/
	nrf_write_register(config_register, configuration_value);

}

uint8_t nrf_read_config()
{

	uint8_t received_data;
	/*Send the address of the status register to the Nordic device to read back the register value*/
	received_data = nrf_read_register(config_register);

	return received_data;
}

uint8_t nrf_read_rf_setup()
{

	uint8_t received_data;
	/*Send the address of the RF setup register for slave to respond with the data*/
	received_data = nrf_read_register(rf_setup_register);
	return received_data;

}

void nrf_write_rf_setup(uint8_t configuration_value)
{

	/*First send the address of the RF setup and the value to be written*/
	nrf_write_register(rf_setup_register, configuration_value);

}

uint8_t nrf_read_rf_ch()
{

	uint8_t received_data;
	/*Send the address of the RF channel register for slave to respond with the data*/
	received_data = nrf_read_register(rf_ch_register);
	return received_data;

}

void nrf_write_rf_ch(uint8_t configuration_value)
{

	/*First send the address of the RF channel and the value to be written*/
	nrf_write_register(rf_ch_register, configuration_value);

}

void nrf_read_TX_ADDR(uint8_t *tx_addr_ptr)
{
	/*Datasheet says LSB is written first for 5 byte register*/
	volatile int8_t i = 0;
	CHIP_SELECT_LOW;
	/*Send the read command ORed with the TX_ADDR register*/
	SPI_write_byte(READ_COMMAND | tx_addr_register);
	/*Now send NOP 5 times to read each byte at a time*/
	while(i < TX_ADDR_LENGTH)
	{
		*(tx_addr_ptr+i) = SPI_write_byte(NOP);
		i++;
	}
	CHIP_SELECT_HIGH;

}

void nrf_write_TX_ADDR(uint8_t *tx_addr_value)
{
	volatile int8_t i = 0;
	CHIP_SELECT_LOW;
	/*Send the write command ORed with the TX_ADDR register*/
	SPI_write_byte(WRITE_COMMAND | tx_addr_register);
	/*Send the values to be written one byte at a time*/
	while(i < TX_ADDR_LENGTH)
	{
		SPI_write_byte(*(tx_addr_value+i));
		i++;
	}
	CHIP_SELECT_HIGH;
}


uint8_t nrf_read_fifo_status()
{
	uint8_t received_data;
	received_data = nrf_read_register(fifo_status_register);
	return received_data;
}

void nrf_flush_tx_fifo()
{
	CHIP_SELECT_LOW;
	SPI_write_byte(flush_tx_fifo);
	CHIP_SELECT_HIGH;
}

void nrf_flush_rx_fifo()
{
	CHIP_SELECT_LOW;
	SPI_write_byte(flush_rx_fifo);
	CHIP_SELECT_HIGH;
}
