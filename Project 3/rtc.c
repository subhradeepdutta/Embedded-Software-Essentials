/*
* rtc.c
 *
 *  Created on: Apr 11, 2017
 *      Author: sameer
 */

#include "rtc.h"

void rtc_config()
{
	//Setup internal reference clock
	MCG_C1 |= MCG_C1_IRCLKEN_MASK;

	MCG_C2 &= ~(MCG_C2_IRCS_MASK);

	//Clock to port c
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;

	//Pin alternate 1
	PORTC_PCR1 |= PORT_PCR_MUX(1);

	//Set clock to RTC as RTC CLKIN
	SIM_SOPT1 |= SIM_SOPT1_OSC32KSEL(2);

	//Select clock for RTC CLKOUT as MCGIRCLK
	SIM_SOPT2 |= SIM_SOPT2_CLKOUTSEL(4);

	//Set up GPIO PTC pins for RTC
	PORTC_PCR3 |= PORT_PCR_MUX(5);

	//RTC Access
	SIM_SCGC6 |= SIM_SCGC6_RTC_MASK;

	//Software reset for RTC
	RTC_CR |= RTC_CR_SWR_MASK;
	RTC_CR &= ~(RTC_CR_SWR_MASK);

	RTC_TSR = TIME;
//	RTC_TSR = 0;
	//Set compensation register to check every 2 seconds and take 32768 ticks per second
//	RTC_TCR |= RTC_TCR_TCR(0xFF) | RTC_TCR_CIR(1);

	NVIC_EnableIRQ(RTC_Seconds_IRQn);

	NVIC_SetPriority(RTC_Seconds_IRQn, 2);

	//Enable interrupt to occur every second to test if RTC works
	RTC_IER = 0;
	RTC_IER |= RTC_IER_TSIE_MASK;


	//Start counter
	RTC_SR |= RTC_SR_TCE_MASK;
}
