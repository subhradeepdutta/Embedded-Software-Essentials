#include "led.h"

void LED_init()
{
	//Clocks to GPIO
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;

	//Set clock source for TPM module
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
	SIM->SOPT2 &= ~(SIM_SOPT2_PLLFLLSEL_MASK);

	//Clock to TPM
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK | SIM_SCGC6_TPM2_MASK;

	//Disable clock while configuring
	TPM0->SC = 0;
	TPM2->SC = 0;

	//Edge aligned PWM with high true pulses
	TPM0->CONTROLS[1].CnSC |= TPM_CnSC_MSB(1) | TPM_CnSC_ELSB(1);
	TPM2->CONTROLS[0].CnSC |= TPM_CnSC_MSB(1) | TPM_CnSC_ELSB(1);
	TPM2->CONTROLS[1].CnSC |= TPM_CnSC_MSB(1) | TPM_CnSC_ELSB(1);

	//Set MOD to be maximum
	TPM0->MOD = 65535;
	TPM2->MOD = 65535;

	TPM0->SC = TPM_SC_PS(0) | TPM_SC_CMOD(1);
	TPM2->SC = TPM_SC_PS(0) | TPM_SC_CMOD(1);

	//Set up GPIO
	PORTB_PCR18 |= PORT_PCR_MUX(3);
//	GPIOB_PDDR = 1 << 18;
	PORTB_PCR19 |= PORT_PCR_MUX(3);
//	GPIOB_PDDR = 1 << 19;
	PORTD_PCR1 |= PORT_PCR_MUX(4);
//	GPIOD_PDDR = 1 << 1;

	//Switch LEDs off by default
	TPM2->CONTROLS[0].CnV = 65535;
	TPM2->CONTROLS[1].CnV = 65535;
	TPM0->CONTROLS[1].CnV = 65535;
	status->red_s = 0;
	status->green_s = 0;
	status->blue_s = 0;
}

CL_Ret_t red_on(void *arg)
{
	if(status->red_s == 0)
	{
		TPM2->CONTROLS[0].CnV = 0;
		status->red_s = 1;
	}
	return CS_SUCCESS;
}

CL_Ret_t green_on(void *arg)
{
	if(status->green_s == 0)
	{
		TPM2->CONTROLS[1].CnV = 0;
		status->green_s = 1;
	}
	return CS_SUCCESS;
}

CL_Ret_t blue_on(void *arg)
{
	if(status->blue_s == 0)
	{
		TPM0->CONTROLS[1].CnV = 0;
		status->blue_s = 1;
	}
	return CS_SUCCESS;
}

CL_Ret_t red_off(void *arg)
{
	if(status->red_s == 1)
	{
		TPM2->CONTROLS[0].CnV = 65535;
		status->red_s = 0;
	}
	return CS_SUCCESS;
}

CL_Ret_t green_off(void *arg)
{
	if(status->green_s == 1)
	{
		TPM2->CONTROLS[1].CnV = 65535;
		status->green_s = 0;
	}
	return CS_SUCCESS;
}

CL_Ret_t blue_off(void *arg)
{
	if(status->blue_s == 1)
	{
		TPM0->CONTROLS[1].CnV = 65535;
		status->blue_s = 0;
	}
	return CS_SUCCESS;
}

CL_Ret_t red_intensity(void *arg)
{
//	red.intensity = val;
	if(status->red_s == 1)
	{
		uint16_t val = 0;
		val |= *(((Message_t *)arg)->data) << 8 | *(((Message_t *)arg)->data + 1);
		TPM2->CONTROLS[0].CnV = val;
	}
	return CS_SUCCESS;
}

CL_Ret_t green_intensity(void *arg)
{
	if(status->green_s == 1)
	{
		uint16_t val = 0;
		val |= *(((Message_t *)arg)->data) << 8 | *(((Message_t *)arg)->data + 1);
		TPM2->CONTROLS[1].CnV = val;
	}
	return CS_SUCCESS;
}

CL_Ret_t blue_intensity(void *arg)
{
	if(status->blue_s == 1)
	{
		uint16_t val = 0;
		val |= *(((Message_t *)arg)->data) << 8 | *(((Message_t *)arg)->data + 1);
		TPM0->CONTROLS[1].CnV = val;
	}
	return CS_SUCCESS;
}
