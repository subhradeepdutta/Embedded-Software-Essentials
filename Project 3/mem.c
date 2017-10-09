#include "mem.h"



size_t length_dma, rem;
uint8_t overlap = 0;

Mem_Enum_t memmove_dma(uint8_t *src, uint8_t *dst, size_t length, size_t byte_size)
{
	if(src == NULL)
	{
		return NULL_SRC;
	}
	else if(dst == NULL)
	{
		return NULL_DST;
	}
	else if(length == 0)
	{
		return DMA_NO_LENGTH;
	}
	else
	{

		if((dst - src > 0) && (dst - src < length))
		{
			overlap = 1;
			overlap_src = src;
			overlap_dst = dst;
			overlap_length = dst - src;

			DMAMUX0_CHCFG2 &= ~(DMAMUX_CHCFG_ENBL_MASK);

			DMA_SAR2 = (uint32_t)dst;
			DMA_DAR2 = (uint32_t)(dst + (dst - src));
			DMA_DSR_BCR2 |= DMA_DSR_BCR_BCR((src + length) - dst);
			DMA_DCR2 |= DMA_DCR_EINT_MASK | DMA_DCR_SINC_MASK | DMA_DCR_DINC_MASK | DMA_DCR_SSIZE(EIGHT_BIT) | DMA_DCR_DSIZE(EIGHT_BIT);
			DMA_DCR2 |= DMA_DCR_START_MASK;

			//set the enable bit for the DMAMUX
			DMAMUX0_CHCFG2 |= DMAMUX_CHCFG_ENBL_MASK;

		}
		else
		{
			if(byte_size == EIGHT_BIT)
			{
				length_dma = length;
			}
			else if(byte_size == SIXTEEN_BIT)
			{
				if(length == 1)
				{
					length_dma = length;
					byte_size = EIGHT_BIT;
				}
				else
				{
					rem = length%2;
					length_dma = length - rem;
				}
			}
			else if(byte_size == THIRTYTWO_BIT)
			{
				if(length == 1 || length == 3)
				{
					length_dma = length;
					byte_size = EIGHT_BIT;
				}
				else if(length == 2)
				{
					rem = length%2;
					length_dma = length;
					byte_size = SIXTEEN_BIT;
				}
				else
				{
					rem = length%4;
					length_dma = length - rem;
				}
			}
			else
			{
				rem = length%4;
				length_dma = length - rem;
			}

			//clear the enable bit for the DMAMUX
			DMAMUX0_CHCFG2 &= ~(DMAMUX_CHCFG_ENBL_MASK);

			DMA_SAR2 = (uint32_t)src;
			DMA_DAR2 = (uint32_t)dst;
			DMA_DSR_BCR2 |= DMA_DSR_BCR_BCR(length);
			DMA_DCR2 |= DMA_DCR_EINT_MASK | DMA_DCR_SINC_MASK | DMA_DCR_DINC_MASK | DMA_DCR_SSIZE(byte_size) | DMA_DCR_DSIZE(byte_size);
			DMA_DCR2 |= DMA_DCR_START_MASK;

			//set the enable bit for the DMAMUX
			DMAMUX0_CHCFG2 |= DMAMUX_CHCFG_ENBL_MASK;
			memmove(src + length_dma, dst + length_dma, rem);
		}


		return DMA_NO_ERROR;
	}
}


Mem_Enum_t memset_dma(uint8_t *dst, size_t length, uint8_t value, size_t byte_size)
{
	if(dst == NULL)
	{
		return NULL_DST;
	}
	else if(length == 0)
	{
		return DMA_NO_LENGTH;
	}
	else
	{
		if(byte_size == EIGHT_BIT)
		{
			length_dma = length;
		}
		else if(byte_size == SIXTEEN_BIT)
		{
			if(length == 1)
			{
				length_dma = length;
				byte_size = EIGHT_BIT;
			}
			else
			{
				rem = length%2;
				length_dma = length - rem;
			}
		}
		else if(byte_size == THIRTYTWO_BIT)
		{
			if(length == 1 || length == 3)
			{
				length_dma = length;
				byte_size = EIGHT_BIT;
			}
			else if(length == 2)
			{
				rem = length%2;
				length_dma = length;
				byte_size = SIXTEEN_BIT;
			}
			else
			{
				rem = length%4;
				length_dma = length - rem;
			}
		}
		else
		{
			rem = length%4;
			length_dma = length - rem;
		}

		//clear the enable bit for the DMAMUX
		DMAMUX0_CHCFG2 &= ~(DMAMUX_CHCFG_ENBL_MASK);

		DMA_SAR2 = (uint32_t)(&value);
		DMA_DAR2 = (uint32_t)dst;
		DMA_DSR_BCR2 |= DMA_DSR_BCR_BCR(length);
		DMA_DCR2 |= DMA_DCR_EINT_MASK | DMA_DCR_DINC_MASK | DMA_DCR_DSIZE(byte_size)| DMA_DCR_SSIZE(byte_size);
		DMA_DCR2 &= ~(DMA_DCR_SINC_MASK);
		DMA_DCR2 |= DMA_DCR_START_MASK;

		//set the enable bit for the DMAMUX
		DMAMUX0_CHCFG2 |= DMAMUX_CHCFG_ENBL_MASK;

		return DMA_NO_ERROR;
	}
}
