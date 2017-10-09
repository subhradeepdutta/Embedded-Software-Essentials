#ifndef NRF_H_
#define NRF_H_

#include "spidriver.h"

extern uint8_t port_number;
extern uint8_t NOP;



#define READ_COMMAND (0x00)
#define WRITE_COMMAND (0x20)

void nrf_chip_enable();
void nrf_chip_disable();

uint8_t nrf_read_register(uint8_t register_address, uint8_t file_desc);
void nrf_write_register(uint8_t register_address, uint8_t file_desc, uint8_t data_value);
uint8_t nrf_read_status(uint8_t file_desc);
void nrf_write_config(uint8_t configuration_value, uint8_t file_desc);
uint8_t nrf_read_config(uint8_t file_desc);
uint8_t nrf_read_rf_setup(uint8_t file_desc);
void nrf_write_rf_setup(uint8_t configuration_value, uint8_t file_desc);
uint8_t nrf_read_rf_ch(uint8_t file_desc);
void nrf_write_rf_ch(uint8_t configuration_value, uint8_t file_desc);
void nrf_read_TX_ADDR(uint8_t file_desc, uint8_t tx_addr_value[]);
void nrf_write_TX_ADDR(uint8_t tx_addr_value[], uint8_t file_desc);
uint8_t nrf_read_fifo_status(uint8_t file_desc);
void nrf_flush_tx_fifo(uint8_t file_desc);
void nrf_flush_rx_fifo(uint8_t file_desc);



#endif