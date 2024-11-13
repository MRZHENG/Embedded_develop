#include "bsp_DMA_mtp.h"

uint8_t SendBuff[SENDBUFF_SIZE];
void MtP_DMA_Config(){
	//开启DMA1的时钟
   RCC_AHBPeriphClockCmd(USART_TX_DMA_CLK,ENABLE);
	 
	 DMA_InitTypeDef DMA_InitStruct;
	 //配置发送数据的外设地址
	 DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)USART_DR_ADR ;
	 //配置接受数据的外设地址
	 DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)SendBuff;
	 //配置数据传输方向 传输到外设
	 DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
	 
	//配置源
	//配置传输数目
	 DMA_InitStruct.DMA_BufferSize = SENDBUFF_SIZE;
	 //增量模式 因为串口只有一个寄存器，不需要递增
	 DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	 //配置数据宽度
	 DMA_InitStruct.DMA_PeripheralDataSize =DMA_PeripheralDataSize_Byte;
	 
	 //配置memory
	 //因为传输多个数据，m的增量模式打开
	 DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	 //串口只接受八位数据
	 DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	 
	 DMA_InitStruct.DMA_Mode = DMA_Mode_Normal; //关闭循环模式
	 DMA_InitStruct.DMA_Priority = DMA_Priority_High; //配置优先级
	 DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	 
	 //配置端口
	 DMA_Init(USART_TX_DMA_CHANNEL,&DMA_InitStruct);
	 
   
	 //使能dma
	 DMA_Cmd(USART_TX_DMA_CHANNEL,ENABLE);
}

