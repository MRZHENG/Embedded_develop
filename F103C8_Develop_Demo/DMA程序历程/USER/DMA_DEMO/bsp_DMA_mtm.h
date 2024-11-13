#ifndef __BSP_DMA_MTM_H
#define    __BSP_DMA_MTM_H

#include "led.h"
#include "stm32f10x.h"
#include "stm32f10x_dma.h"
#define    bsp_DMA_CLK   RCC_AHBPeriph_DMA1
#define    MTM_DMA_CHANEL DMA1_Channel6
#define BUFFER_SIZE 32


void MtM_DMA_Config();

uint8_t BufferCmp(const uint32_t* pBuffer,const uint32_t* pBuffer1,uint16_t size);

#endif


