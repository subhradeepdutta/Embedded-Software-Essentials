/******************************************************************
* This program uses function definitions from the following source
https://github.com/graycatlabs/serbus/blob/master/src/spidriver.c
******************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include "spidriver.h"




int SPI_open(uint8_t bus, uint8_t cs) {
  char device[SPIDEV_PATH_LEN];
  sprintf(device, "/dev/spidev%d.%d", bus, cs);
  return open(device, O_RDWR, 0);
}

void SPI_close(int spidev_fd) {
  close(spidev_fd);
}


int SPI_setBitOrder(int spidev_fd, SPI_bit_order bit_order) {
  uint8_t order = (uint8_t) bit_order; // Just to be safe
  if (ioctl(spidev_fd, SPI_IOC_WR_LSB_FIRST, &order) < 0) return -1;
  return 0;
}

int SPI_setBitsPerWord(int spidev_fd, uint8_t bits_per_word) {
  if (ioctl(spidev_fd, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word) < 0) {
    return -1;
  }
  return 0;
}

int SPI_getBitsPerWord(int spidev_fd) {
  uint8_t bits_per_word;
  if (ioctl(spidev_fd, SPI_IOC_RD_BITS_PER_WORD, &bits_per_word) < 0) {
    return -1;
  } 
  return bits_per_word == 0 ? 8 : bits_per_word;
}

int SPI_setMaxFrequency(int spidev_fd, uint32_t frequency) {
  if (ioctl(spidev_fd, SPI_IOC_WR_MAX_SPEED_HZ, &frequency) < 0) return -1;
  return 0;
}

int SPI_getMaxFrequency(int spidev_fd) {
  uint32_t frequency;
  if (ioctl(spidev_fd, SPI_IOC_RD_MAX_SPEED_HZ, &frequency) < 0) return -1;
  return frequency;
}

int SPI_setClockMode(int spidev_fd, uint8_t clock_mode) {
  uint8_t mode;
  mode = SPI_getMode(spidev_fd);
  if (mode < 0) return mode;
  mode &= ~0x3;
  mode |= clock_mode & 0x3;
  return SPI_setMode(spidev_fd, mode);
}

int SPI_getClockMode(int spidev_fd) {
  uint8_t clock_mode;
  clock_mode = SPI_getMode(spidev_fd);
  if (clock_mode < 0) return clock_mode;
  return clock_mode & 0x3;
}

int SPI_setCSActiveLow(int spidev_fd) {
  int mode = SPI_getMode(spidev_fd);
  if (mode < 0) return -1;
  return SPI_setMode(spidev_fd, mode & ~SPI_CS_HIGH);
}

int SPI_setCSActiveHigh(int spidev_fd) {
  int mode = SPI_getMode(spidev_fd);
  if (mode < 0) return -1;
  return SPI_setMode(spidev_fd, mode | SPI_CS_HIGH);
}

int SPI_enableCS(int spidev_fd) {
  int mode = SPI_getMode(spidev_fd);
  if (mode < 0) return -1;
  return SPI_setMode(spidev_fd, mode & ~SPI_NO_CS);
}

int SPI_disableCS(int spidev_fd) {
  int mode = SPI_getMode(spidev_fd);
  if (mode < 0) return -1;
  return SPI_setMode(spidev_fd, mode | SPI_NO_CS);
}

int SPI_enableLoopback(int spidev_fd) {
  int mode = SPI_getMode(spidev_fd);
  if (mode < 0) return -1;
  return SPI_setMode(spidev_fd, mode | SPI_LOOP);
}

int SPI_disableLoopback(int spidev_fd) {
  int mode = SPI_getMode(spidev_fd);
  if (mode < 0) return -1;
  return SPI_setMode(spidev_fd, mode & ~SPI_LOOP);
}

int SPI_enable3Wire(int spidev_fd) {
  int mode = SPI_getMode(spidev_fd);
  if (mode < 0) return -1;
  return SPI_setMode(spidev_fd, mode | SPI_3WIRE);
}

int SPI_disable3Wire(int spidev_fd) {
  int mode = SPI_getMode(spidev_fd);
  if (mode < 0) return -1;
  return SPI_setMode(spidev_fd, mode & ~SPI_3WIRE);
}

int SPI_setMode(int spidev_fd, uint8_t mode) {
  if (ioctl(spidev_fd, SPI_IOC_WR_MODE, &mode) < 0) return -1;
  return 0;
}

int SPI_getMode(int spidev_fd) {
  uint8_t mode;
  if (ioctl(spidev_fd, SPI_IOC_RD_MODE, &mode) < 0) return -1;
  return mode;
}

uint8_t SPI_init(uint8_t file_desc)
{
  file_desc = SPI_open(1,0);
  if (file_desc < 0)
  {
    printf("Cannot Open Device");
    exit(0);
  }
  else
  {
    printf("SPI device opened successfully\n");
  }
  SPI_setMaxFrequency(file_desc,SPI_freq);
  SPI_setClockMode(file_desc,CLK_MODE);
  SPI_enable3Wire(file_desc);
  SPI_setBitOrder(file_desc,SPI_MSBFIRST);
  SPI_setBitsPerWord(file_desc,EIGHT_BITS);
  return file_desc;
}


void GPIO_init()
{
  FILE *file_ptr;
  file_ptr = fopen("/sys/class/gpio/export", "w");
  if(file_ptr == NULL)
  {
    printf("Attempt to open file pointer failed");
  }
  fprintf(file_ptr, "%d\n", port_number);
  fclose(file_ptr);

  char filename[150];
  sprintf(filename, "/sys/class/gpio/gpio%d/direction", port_number);
  file_ptr = fopen(filename, "w");
  if(file_ptr == NULL)
  {
    printf("Attempt to open file pointer failed");
  } 
  fprintf(file_ptr, "out\n");
  fclose(file_ptr);
}

