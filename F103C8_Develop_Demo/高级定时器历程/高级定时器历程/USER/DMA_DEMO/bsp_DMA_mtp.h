#ifndef __BSP_DMA_MTP_H
#define __BSP_DMA_MTP_H

#include "usart1.h"
#include "stm32f10x.h"
#include "stm32f10x_dma.h"

//串口对应的DMA请求通道
#define   USART_TX_DMA_CLK       RCC_AHBPeriph_DMA1
#define   USART_TX_DMA_CHANNEL   DMA1_Channel4
//外设寄存器地址
#define   USART_DR_ADR       (USART1_BASE+0x04)

#define   SENDBUFF_SIZE      5000


void MtP_DMA_Config(void);

#endif 
