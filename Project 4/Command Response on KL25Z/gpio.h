/*************************************
 * Header file containing the
 * functions corresponding to
 * GPIO pins
 *************************************/
#ifndef GPIO_H_
#define GPIO_H_

/*Macro used to set the 5th bit of a register*/
#define SET_BIT_FIVE (0x10)
/*Macro used to set the 9th bit of a register*/
#define SET_BIT_NINE (1 << 8)
/*Macro used to set the 18th bit of a register*/
#define SET_BIT_NINETEEN (1 << 18)
/*Configure pin for GPIO mode*/
#define GPIO_MODE (0x1)
/*Configure pin for SPI mode*/
#define SPI_MODE (0x2)

/*******************************************************
*Description: This function initializes the various
*	      GPIO pins required for SPI communication
*	      with the Nordic device and also enables
*	      clock to the required modules
*
*Parameters : N/A
*Return	    : N/A
*******************************************************/
void GPIO_nrf_init();


/*******************************************************
*Description: This function sets the CE pin of the Nordic
*	      device to high
*
*Parameters : N/A
*Return	    : N/A
*******************************************************/
void nrf_transmit_enable() __attribute__((always_inline));

/*******************************************************
*Description: This function sets the CE pin of the Nordic
*	      device to low
*
*Parameters : N/A
*Return	    : N/A
*******************************************************/
void nrf_transmit_disable()  __attribute__((always_inline));

/*******************************************************
*Description: This function can be used as an indicator
*	      to turn on the red LEd on the KL25z when
*	      we want to verify/test certain conditions
*	      or values on the KL25z
*	      Mainly for debugging purposes
*
*Parameters : N/A
*Return	    : N/A
*******************************************************/
void indicator();


#endif
