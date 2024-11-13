#include "DMA.h"


int Rx1Buf[Bufsize]={0};
int Rx2Buf[Bufsize]={0};


void DMA1_Recive_Config(void){
   DMA_InitTypeDef  DMA_Str;           //DMA配置结构体
   NVIC_InitTypeDef NVIC_InitStruct;   //中断配置
	
	 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	 DMA_Str.DMA_BufferSize = Bufsize;
	 DMA_Str.DMA_DIR=DMA_DIR_PeripheralSRC;
	 DMA_Str.DMA_M2M =DMA_M2M_Disable;
	 DMA_Str.DMA_MemoryBaseAddr = (u32)Rx1Buf;  
	 DMA_Str.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	 DMA_Str.DMA_MemoryInc = DMA_MemoryInc_Enable;
	 DMA_Str.DMA_Mode = DMA_Mode_Normal;
	 DMA_Str.DMA_PeripheralBaseAddr = (u32)&USART1->DR;
	 DMA_Str.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	 DMA_Str.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	 DMA_Str.DMA_Priority = DMA_Priority_High;
	 DMA_Init(DMA1_Channel5,&DMA_Str);
	 DMA_Cmd(DMA1_Channel5,ENABLE);
	 
}


