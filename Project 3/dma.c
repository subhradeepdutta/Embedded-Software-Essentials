#include "dma.h"

#define SRC60	(60)



void dma_config()
{
	//Add clock to the DMAMUX
	SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;

	//clear the enable bit
	DMAMUX0_CHCFG2 &= ~(DMAMUX_CHCFG_ENBL_MASK);

	//clear the trigger bit
	DMAMUX0_CHCFG2 &= ~(DMAMUX_CHCFG_TRIG_MASK);

	//set the DMAMUX source to be always enabled i.e. source 60
	DMAMUX0_CHCFG2 |= DMAMUX_CHCFG_SOURCE(SRC60);

	//Configure the DMA
	//Add clock to the DMA
	SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;

	//Enhable IRQ for DMA
	NVIC_EnableIRQ(DMA2_IRQn);

	//Set priority for interrupt
	NVIC_SetPriority(DMA2_IRQn, 0);

	__enable_irq();

}
