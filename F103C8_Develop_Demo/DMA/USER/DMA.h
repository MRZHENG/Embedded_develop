
#ifndef __DMA__H
#define __DMA__H

#include "stm32f10x.h"
#include "stm32f10x_dma.h"
#include "misc.h"
#include "stm32f10x_usart.h"


#define  Bufsize 256

extern int Rx1Buf[Bufsize];
extern int Rx2Buf[Bufsize];



void DMA1_Recive_Config(void);

#endif

