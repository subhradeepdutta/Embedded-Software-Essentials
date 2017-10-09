#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include "spidriver.h"
#include "nrf.h"
#include <errno.h>
#include <stdlib.h>

uint8_t port_number = 117;
uint8_t NOP = 0xff;
extern uint8_t status_register;
extern uint8_t config_register;
extern uint8_t rf_setup_register;
extern uint8_t rf_ch_register;
extern uint8_t tx_addr_register;
extern uint8_t fifo_status_register;


uint8_t file_desc;

int main()
{
	uint8_t return_value;
	uint8_t value_txaddr[] = {0x11,0x22,0x33,0x44,0x55};
	uint8_t returned_data[5];

	file_desc = SPI_init(file_desc);
	GPIO_init();
	uint8_t status = nrf_read_status(file_desc);
	printf("Status %x\n", status);
	nrf_write_config(0x7f,file_desc);
	uint8_t config = nrf_read_config(file_desc);
	printf("Config = %x\n",config);
	nrf_write_rf_setup(0x11,file_desc);
	uint8_t rf_setup = nrf_read_rf_setup(file_desc);
	printf("RF Setup = %x\n",rf_setup);
	nrf_write_rf_ch(0x7f,file_desc);
	uint8_t rf_ch = nrf_read_rf_ch(file_desc);
	printf("Rf_channel = %x\n",rf_ch);
	nrf_write_TX_ADDR(value_txaddr,file_desc);
	nrf_read_TX_ADDR(file_desc, returned_data);
	printf("TX_ADDR - %x %x %x %x %x \n",returned_data[0],returned_data[1],returned_data[2],returned_data[3],returned_data[4]);
	uint8_t fifo_status = nrf_read_fifo_status(file_desc);
	
	uint8_t fifo_st = nrf_read_fifo_status(file_desc);
	printf("Fifo Status %x\n",fifo_st);

	SPI_close(file_desc);
	return 0;



}

