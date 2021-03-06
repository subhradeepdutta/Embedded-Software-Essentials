#include "uart.h"
#include "MKL25Z4.h"
#include "core_cmFunc.h"
#include "core_cm0plus.h"

#define MCGFLLCLK		1		//macro for the MCGFLLCLK
#define OSCERCLK		2		//macro for the OSCERCLK
#define MCGIRCLK 		3		//macro for the MCGIRCLK

#define CLEAR			0		//macro to clear register

#define PORTA_ALT_2	2			//macro for port alternative 2

#define OSR_VAL		 	15		//macro for the value of oversampling ratio

#define BDH_VAL			0		//macro for higher bytes of the baud rate setting register
#define BDL_VAL			23		//macro for higher bytes of the baud rate setting register

#define DATA_CLEAR		0x80	//macro that checks TDRE
#define DATA_ARRIVED	0x20	//macro that checks RDRF

#define SET_ENABLE_UART0_INT (uint32_t)(1 << 12)		//macro that sets UART0 interrupt enable
#define SET_PRIORITY_UART0 (0 << 6)		//macro that sets prior

UART_Ret_t uart_configure()
{
	//Set up clock to the UARTO
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;

	//Select clock source
	SIM->SOPT2 |= (MCGFLLCLK << SIM_SOPT2_UART0SRC_SHIFT);

	//Clear all
	UART0->C2 &= CLEAR;

	//Set Baud rate
	UART0->BDH |= (BDH_VAL << UART0_BDH_SBR_SHIFT);
	UART0->BDL |= (BDL_VAL << UART0_BDL_SBR_SHIFT);
	UART0->C4 |= (OSR_VAL << UART0_C4_OSR_SHIFT);

	//Set stop bit
//	UART0->BDH |= UART_BDH_SBNS_MASK;
	UART0->BDH |= 0;

	//Set the data to be on 8-bit size
	UART0->C1 |= CLEAR;

	//Clear the interrupt flag register
	UART0->C2 = 0;


	//Set transmit enable and receive enable
//	UART0->C2 |= UART0_C2_TE_MASK | UART0_C2_RE_MASK | UART0_C2_TIE_MASK | UART0_C2_RIE_MASK;
//	UART0->C2 |= UART0_C2_TE_MASK | UART0_C2_RE_MASK;
	UART0->C2 |= UART0_C2_TE_MASK | UART0_C2_RE_MASK | UART0_C2_RIE_MASK;

	//Set up clock for PORT A
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

	//Set PORT A pin 1 and 2 to Alternative 2
	PORTA_PCR1 |= (PORTA_ALT_2 << PORT_PCR_MUX_SHIFT);
	PORTA_PCR2 |= (PORTA_ALT_2 << PORT_PCR_MUX_SHIFT);


//	NVIC->ISER[0] = SET_ENABLE_UART0_INT;
	NVIC_EnableIRQ(UART0_IRQn);

	NVIC->IP[3] = SET_PRIORITY_UART0;

	__enable_irq();

	return SUCCEEDED;
}

UART_Ret_t uart_send_byte(uint8_t data)
{
//	while((UART0->S1 & UART_S1_TDRE_MASK) != DATA_CLEAR);
	UART0->D = data;

	return SUCCEEDED;
}

UART_Ret_t uart_send_byte_n(uint8_t *data_ptr, size_t length)
{
	if(data_ptr == NULL)
	{
		return NULL_PASSED;
	}

	while(length)
	{
		uart_send_byte(*data_ptr);
		length--;
		data_ptr++;
	}

	return SUCCEEDED;
}

UART_Ret_t inline uart_receive_byte(uint8_t *data_ptr)
{
	if(data_ptr == NULL)
	{
		return NULL_PASSED;
	}

//	while((UART0->S1 & UART_S1_RDRF_MASK) == 0);
	*data_ptr = UART0->D;

	return SUCCEEDED;
}
