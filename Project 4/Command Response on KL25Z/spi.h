#ifndef SOURCES_SPI_H_
#define SOURCES_SPI_H_

#define NOP (0xff)
#include <stdint.h>

/*******************************************************
*Description: This function initializes the SPI control
*	      registers with the Kl25z as a master,
*	      CPHA & CPOL set to 0 and MSB first mode
*
*Parameters : N/A
*Return	    : N/A
*******************************************************/
void SPI_init();

/*******************************************************
*Description: This function reads a single byte from the
*   	      nordic device
*
*Parameters : It takes a pointer to a uint8_t variable
*	      to store the read value
*Return	    : N/A
*******************************************************/
void SPI_read_byte(uint8_t *data);

/*******************************************************
*Description: This function writes a single byte to the
*   	      nordic device
*
*Parameters : It takes a uint8_t variable which needs to
*	      be written
*
*Return	    : It returns the status register value
*******************************************************/
uint8_t SPI_write_byte(uint8_t data);

/*******************************************************
*Description: This function writes multiple bytes of data
*	      on the nordic device
*
*Parameters : It takes a pointer of type uint8_t and length
*	      of data
*
*Return	    : N/A
*******************************************************/
void SPI_send_packet(uint8_t *data_packet, size_t length);

/*******************************************************
*Description: This blocks until transmission is done
*
*Parameters : N/A
*
*Return	    : N/A
*******************************************************/
static void SPI_flush() __attribute__((always_inline));

#endif
