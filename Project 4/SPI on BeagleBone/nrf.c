#include <stdio.h>
#include <stdlib.h>
#include "spidriver.h"
#include "nrf.h"

uint8_t status_register = (0x07);
uint8_t config_register = (0x00);
uint8_t rf_setup_register = (0x06);
uint8_t rf_ch_register = (0x05);
uint8_t tx_addr_register = (0x10);
uint8_t fifo_status_register = (0x17);

uint8_t flush_tx_fifo = (0xE1);
uint8_t flush_rx_fifo = (0xE2);

void nrf_chip_enable()
{
	FILE *gpio_pin;
	char filename[150];
	sprintf(filename, "/sys/class/gpio/gpio%d/value", port_number);
	gpio_pin = fopen(filename, "w");
	fprintf(gpio_pin, "0\n");
	fclose(gpio_pin);

}

void nrf_chip_disable()
{
	FILE *gpio_pin;
	char filename[150];
	sprintf(filename, "/sys/class/gpio/gpio%d/value", port_number);
	gpio_pin = fopen(filename, "w");
	fprintf(gpio_pin, "1\n");
	fclose(gpio_pin);

}

uint8_t nrf_read_register(uint8_t register_address, uint8_t file_desc)
{
	uint8_t command_value, register_value;
	command_value = (READ_COMMAND | register_address);
	if(!file_desc)
		printf("Failed\n");
	write(file_desc,&command_value,ONE_BYTE);
	write(file_desc,&NOP,ONE_BYTE);
	read(file_desc,&register_value,ONE_BYTE);
	return register_value;
}

void nrf_write_register(uint8_t register_address, uint8_t file_desc, uint8_t data_value)
{
	uint8_t command_value;
	command_value = (WRITE_COMMAND | register_address);
	write(file_desc,&command_value,ONE_BYTE);
	write(file_desc,&data_value,ONE_BYTE);
}


uint8_t nrf_read_status(uint8_t file_desc)
{
	nrf_chip_enable();
	uint8_t received_data;
	uint8_t command_value = status_register | READ_COMMAND;
	write(file_desc,&command_value,ONE_BYTE);
	read(file_desc,&received_data,1);
	nrf_chip_disable();
	return received_data;
}

void nrf_write_config(uint8_t configuration_value, uint8_t file_desc)
{

	nrf_chip_enable();
	nrf_write_register(config_register, file_desc, configuration_value);
	nrf_chip_disable();

}

uint8_t nrf_read_config(uint8_t file_desc)
{

	nrf_chip_enable();
	uint8_t received_data = nrf_read_register(config_register, file_desc);
	nrf_chip_disable();
	return received_data;

}

uint8_t nrf_read_rf_setup(uint8_t file_desc)
{

	nrf_chip_enable();
	uint8_t received_data = nrf_read_register(rf_setup_register, file_desc);
	nrf_chip_disable();
	return received_data;

}

void nrf_write_rf_setup(uint8_t configuration_value, uint8_t file_desc)
{

	nrf_chip_enable();
	nrf_write_register(rf_setup_register, file_desc, configuration_value);
	nrf_chip_disable();

}

uint8_t nrf_read_rf_ch(uint8_t file_desc)
{

	nrf_chip_enable();
	uint8_t received_data = nrf_read_register(rf_ch_register, file_desc);
	nrf_chip_disable();
	return received_data;

}
void nrf_write_rf_ch(uint8_t configuration_value, uint8_t file_desc)
{

	nrf_chip_enable();
	nrf_write_register(rf_ch_register, file_desc, configuration_value);
	nrf_chip_disable();

}

void nrf_read_TX_ADDR(uint8_t file_desc, uint8_t tx_addr_value[])
{
	int8_t register_value = 0; 
	uint8_t i = 1;
	nrf_chip_enable();
	register_value = 0x10| 0x00;
	write(file_desc,&register_value,ONE_BYTE);
	for(i = 1;i<9;i++)
	{
		write(file_desc,&NOP,ONE_BYTE);
	}
	read(file_desc,tx_addr_value,5);
	nrf_chip_disable();

}

void nrf_write_TX_ADDR(uint8_t tx_addr_value[], uint8_t file_desc)
{
	uint8_t register_value = 0;
	nrf_chip_enable();
	register_value = tx_addr_register| WRITE_COMMAND;
	write(file_desc,&register_value,ONE_BYTE);
	write(file_desc,tx_addr_value,5);
	nrf_chip_disable();
}

uint8_t nrf_read_fifo_status(uint8_t file_desc)
{
	uint8_t received_data;
	nrf_chip_enable();
	received_data = nrf_read_register(fifo_status_register, file_desc);
	nrf_chip_disable();
	return received_data;

}

void nrf_flush_tx_fifo(uint8_t file_desc)
{
	nrf_chip_enable();
	write(file_desc,&flush_tx_fifo,ONE_BYTE);
	nrf_chip_disable();
}


void nrf_flush_rx_fifo(uint8_t file_desc)
{
	nrf_chip_enable();
	write(file_desc,&flush_rx_fifo,ONE_BYTE);
	nrf_chip_disable();
}