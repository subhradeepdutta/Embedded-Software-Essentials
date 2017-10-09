#ifndef _SPI_DRIVER_H_
#define _SPI_DRIVER_H_

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <stdlib.h>

#define SPI_freq 1000000
#define CLK_MODE 0
#define ONE_BYTE 1
#define EIGHT_BITS 8

extern uint8_t port_number;
extern uint8_t NOP;

extern uint8_t status_register;
extern uint8_t config_register;
extern uint8_t rf_setup_register;
extern uint8_t rf_ch_register;
extern uint8_t tx_addr_register;
extern uint8_t fifo_status_register;



/// Buffer size at least large enough to fit the max length of "/dev/spidevX.Y"
#define SPIDEV_PATH_LEN 20
 /// Maximum transfer size set to standard page size of 4096 bytes
#define MAX_TRANSFER_SIZE 4096

int SPI_open(uint8_t bus, uint8_t cs);

void SPI_close(int spidev_fd);

typedef enum { 
  SPI_MSBFIRST, ///< Most significant bit first
  SPI_LSBFIRST  ///< Least significant bit first
} SPI_bit_order;


 int SPI_setBitOrder(int spidev_fd, SPI_bit_order bit_order);

int SPI_setBitsPerWord(int spidev_fd, uint8_t bits_per_word);

int SPI_getBitsPerWord(int spidev_fd);

int SPI_setMaxFrequency(int spidev_fd, uint32_t frequency);

int SPI_getMaxFrequency(int spidev_fd);

int SPI_setClockMode(int spidev_fd, uint8_t clock_mode);

int SPI_getClockMode(int spidev_fd);

int SPI_setCSActiveLow(int spidev_fd);

int SPI_setCSActiveHigh(int spidev_fd);

int SPI_enableCS(int spidev_fd);

int SPI_disableCS(int spidev_fd);

int SPI_enableLoopback(int spidev_fd);

int SPI_disableLoopback(int spidev_fd);

int SPI_enable3Wire(int spidev_fd);

int SPI_disable3Wire(int spidev_fd);

int SPI_setMode(int spidev_fd, uint8_t mode);

int SPI_getMode(int spidev_fd);

uint8_t SPI_init(uint8_t file_desc);

void GPIO_init();


#endif