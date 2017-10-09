#include <stdint.h>
#include "MKL25Z4.h"
#include "gpio.h"

/**********************************************************************/
void GPIO_nrf_init()
{
	/*Clock Gate control for SPI0 module*/
	SIM->SCGC4 |= SIM_SCGC4_SPI0_MASK;

	/*Enabled Port C pin 4, 5, 6, 7 for SPI0 CS, SCK, MOSI, MISO*/
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;

	/*Port C Pin 4 is configured as GPIO output*/
	GPIOC_PDDR |= GPIO_PDDR_PDD(SET_BIT_FIVE);
	/*Initialize with CS pin high*/
	GPIOC_PSOR |= GPIO_PSOR_PTSO(SET_BIT_FIVE);


	/*Port C Pin 8 is configured as GPIO output*/
	GPIOC_PDDR |= SET_BIT_NINE;

	/*Initialize with CE pin low*/
	GPIOC_PCOR |= SET_BIT_NINE;

	/*Set CS to user control*/
	PORTC_PCR4 = PORT_PCR_MUX(GPIO_MODE);
	/*Configure for serial clock*/
	PORTC_PCR5 = PORT_PCR_MUX(SPI_MODE);
	/*Configure for MOSI*/
	PORTC_PCR6 = PORT_PCR_MUX(SPI_MODE);
	/*Configure for MISO*/
	PORTC_PCR7 = PORT_PCR_MUX(SPI_MODE);
	/*To be use for CE pin*/
	PORTC_PCR8 = PORT_PCR_MUX(GPIO_MODE);
}

/**********************************************************************/

/*This function turns on CE pin*/
inline void nrf_transmit_enable()
{
	GPIOC_PSOR |= SET_BIT_NINE;
}

/**********************************************************************/

/*This function turns off CE pin*/
inline void nrf_transmit_disable()
{
	GPIOC_PCOR |= SET_BIT_NINE;
}

/**********************************************************************/

void indicator()
{
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB_PCR18 = PORT_PCR_MUX(GPIO_MODE);
	GPIOB_PDDR |= SET_BIT_NINETEEN;
}

