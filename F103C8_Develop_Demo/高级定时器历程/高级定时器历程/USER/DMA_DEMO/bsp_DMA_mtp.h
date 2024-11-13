#ifndef __BSP_DMA_MTP_H
#define __BSP_DMA_MTP_H

#include "usart1.h"
#include "stm32f10x.h"
#include "stm32f10x_dma.h"

//���ڶ�Ӧ��DMA����ͨ��
#define   USART_TX_DMA_CLK       RCC_AHBPeriph_DMA1
#define   USART_TX_DMA_CHANNEL   DMA1_Channel4
//����Ĵ�����ַ
#define   USART_DR_ADR       (USART1_BASE+0x04)

#define   SENDBUFF_SIZE      5000


void MtP_DMA_Config(void);

#endif 
