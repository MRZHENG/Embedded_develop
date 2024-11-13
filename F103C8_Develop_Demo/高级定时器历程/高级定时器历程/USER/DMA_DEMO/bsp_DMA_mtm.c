#include "bsp_DMA_mtm.h"

//发送数据的大小


//定义数组作为数据源，存储在内部的flash中

const uint32_t  aSRC_Const_Buffer[BUFFER_SIZE] ={
                   1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                   11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                   21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};

//定义DMA传输目标存储器
//存储在内部的SRAM中
									 
uint32_t aDST_Buffer[BUFFER_SIZE];

void MtM_DMA_Config(){
	//开启DMA1的时钟
   RCC_AHBPeriphClockCmd(bsp_DMA_CLK,ENABLE);
	 
	 DMA_InitTypeDef DMA_InitStruct;
	 //配置发送数据的外设地址
	 DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)aSRC_Const_Buffer;
	 //配置接受数据的外设地址
	 DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)aDST_Buffer;
	 //配置数据传输方向
	 DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
	 
	//配置源
	//配置传输数目
	 DMA_InitStruct.DMA_BufferSize = BUFFER_SIZE;
	 //增量模式
	 DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	 //配置数据宽度
	 DMA_InitStruct.DMA_PeripheralDataSize =DMA_PeripheralDataSize_Word;
	 
	 //配置memory
	 DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	 DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	 DMA_InitStruct.DMA_Mode = DMA_Mode_Normal; //关闭循环模式
	 DMA_InitStruct.DMA_Priority = DMA_Priority_High; //配置优先级
	 DMA_InitStruct.DMA_M2M = DMA_M2M_Enable;
	 
	 //配置端口
	 DMA_Init(MTM_DMA_CHANEL,&DMA_InitStruct);
	 //使能dma
	 DMA_Cmd(MTM_DMA_CHANEL,ENABLE);
}

//判断数据是否传输成功
uint8_t BufferCmp(const uint32_t* pBuffer,const uint32_t* pBuffer1,uint16_t size){
  while(size--){
	  if(* pBuffer!=* pBuffer1){
		  return 0;
		}
		pBuffer++;
		pBuffer1++;
	}
	return 1;
}

