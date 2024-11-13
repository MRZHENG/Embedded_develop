
#ifndef __DMA__H
#define __DMA__H
#include "string.h"
#include <stdarg.h>
#include "stm32f10x.h"
#include "stm32f10x_dma.h"
#include "misc.h"
#include "stm32f10x_usart.h"

#define USART1_MAX_TX_LEN 512
#define USART1_MAX_RX_LEN 512


void DMA_USART1_Tx_Data(u8 *buffer, u32 size);
void DMA1_Channel4_IRQHandler(void);
void DMA1_Channel5_IRQHandler(void);
void DMA1_USART1_Init(void);
void USART1_IRQHandler(void); 
void USART1_printf(char *format, ...);
#endif

