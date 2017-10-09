#ifndef NRF_H_
#define NRF_H_

#define status_register (0x07)
#define config_register (0x00)
#define rf_setup_register (0x06)
#define rf_ch_register (0x05)
#define tx_addr_register (0x10)
#define fifo_status_register (0x17)
/*Refer to Nordic datasheet Page 55 for register address*/
#define CHIP_SELECT_LOW (GPIOC_PCOR |= (1 << 4))
#define CHIP_SELECT_HIGH (GPIOC_PDOR |= (1 << 4))
#define TX_ADDR_LENGTH (5)
#define flush_tx_fifo (0xE1)
#define flush_rx_fifo (0xE2)
#define nrf_power_up (0x02)
#define nrf_power_down (0x01)
#define READ_COMMAND (0x00)
#define WRITE_COMMAND (0x20)

/*******************************************************
*Description: This function reads a specific register
*	      on the Nordic device
*
*Parameters : The register address is passed as argument
*	      to the function
*Return	    : Returns the value of the register value
*******************************************************/
uint8_t nrf_read_register(uint8_t register_address);

/*******************************************************
*Description: This function writes to a specific register
*	      on the Nordic device
*
*Parameters : The register address is passed as argument
*	      to the function along with the value to be
*	      written
*Return	    : N/A
*******************************************************/
void nrf_write_register(uint8_t register_address, uint8_t value);

/*******************************************************
*Description: This function initializes the Nordic module
*
*Parameters : N/A
*
*Return	    : N/A
*******************************************************/
void nrf_init();

/*******************************************************
*Description: This function reads the value of the status
*	      register
*
*Parameters : N/A
*
*Return	    : Value of the status register
*******************************************************/
uint8_t nrf_read_status();

/*******************************************************
*Description: This function writes a value to the config
*	      register
*
*Parameters : The value to be written to the config register
*
*Return	    : N/A
*******************************************************/
void nrf_write_config(uint8_t configuration_value);

/*******************************************************
*Description: This function reads the value of the config
*	      register
*
*Parameters : N/A
*
*Return	    : Value of the config register
*******************************************************/
uint8_t nrf_read_config();

/*******************************************************
*Description: This function reads the RF_SETUP
*	      register
*
*Parameters : N/A
*
*Return	    : Value of the RF_SETUP register
*******************************************************/
uint8_t nrf_read_rf_setup();

/*******************************************************
*Description: This function writes a value to the RF_SETUP
*	      register
*
*Parameters : The value to be written to the RF_SETUP register
*
*Return	    : N/A
*******************************************************/
void nrf_write_rf_setup(uint8_t configuration_value);

/*******************************************************
*Description: This function reads the RF_CH
*	      register
*
*Parameters : N/A
*
*Return	    : Value of the RF_CH register
*******************************************************/
uint8_t nrf_read_rf_ch();

/*******************************************************
*Description: This function writes a value to the RF_CH
*	      register
*
*Parameters : The value to be written to the RF_CH register
*
*Return	    : N/A
*******************************************************/
void nrf_write_rf_ch(uint8_t configuration_value);

/*******************************************************
*Description: This function reads the TX_ADDR
*	      register
*
*Parameters : N/A
*
*Return	    : Value of the TX_ADDR register
*******************************************************/
void nrf_read_TX_ADDR(uint8_t *tx_addr_ptr);

/*******************************************************
*Description: This function writes a value to the TX_ADDR
*	      register
*
*Parameters : The value to be written to the TX_ADDR register
*
*Return	    : N/A
*******************************************************/
void nrf_write_TX_ADDR(uint8_t *tx_addr_value);

/*******************************************************
*Description: This function reads the FIFO_STATUS
*	      register
*
*Parameters : N/A
*
*Return	    : Value of the FIFO_STATUS register
*******************************************************/
uint8_t nrf_read_fifo_status();

/*******************************************************
*Description: This function flushes the TX_FIFO register
*
*Parameters : N/A
*
*Return	    : N/A
*******************************************************/
void nrf_flush_tx_fifo();

/*******************************************************
*Description: This function flushes the RX_FIFO register
*
*Parameters : N/A
*
*Return	    : N/A
*******************************************************/
void nrf_flush_rx_fifo();


#endif
