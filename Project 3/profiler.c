
#include "profiler.h"
#include "MKL25Z4.h"
#include <stdint.h>

#ifdef BBB
#include <time.h>
#define	CALCULATE_TIME
#endif

uint32_t OF_val = 0;


void profiler_config()
{
	//Set up clock to the timer module
	SIM->SOPT2 |= (MCGFLLCLK << SIM_SOPT2_TPMSRC_SHIFT);

	//Enable the clock to the timer module
	SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK;

	//Set up the CMOD, TOIE, TOF, and prescalar value
	TPM0_SC |= TPM_SC_CMOD(LOCAL_CLOCK) | TPM_SC_TOIE_MASK | TPM_SC_TOF_MASK | TPM_SC_PS(PRESCALE);

	//Clear the count register since writing anyvalue to it clears the register
	TPM0_CNT = 1;

	//Set a realy low modulo to simulate the interrupt
	TPM0_MOD = TPM_MOD_MOD(65535);

	//Enhable IRQ for TPM
	NVIC_EnableIRQ(TPM0_IRQn);

	//Set priority for interrupt
	NVIC_SetPriority(TPM0_IRQn, 1);

	//Enable global interrupt
	__enable_irq();

}

uint32_t calculate_time()
{
	return ((OF_val == 0) ? (TPM0_CNT) : (OF_val)*(65536) + (TPM0_CNT));
}
